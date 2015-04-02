/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       x86 64 Linux port core source
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include "port/core.h"
#include "shared/config.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static void timer_handler(int signal);



static void * timer_thread(void * arg);



static void timer_init(void);



static void timer_term(void);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct itimerval         g_timer;
static pthread_t                g_timer_thread;
static pthread_mutex_t          g_timer_lock;

/*======================================================  GLOBAL VARIABLES  ==*/

pthread_mutex_t                 g_global_lock;

const uint_fast8_t              g_log2_lookup[256] =
{
    0u, 0u, 1u, 1u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u,
    4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
    5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
    5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
    6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
    6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
    6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
    6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
    7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


/**@brief       Timer signal handler for signaling to timer thread
 */
static void timer_handler(int signal)
{
    (void)signal;

    pthread_mutex_unlock(&g_timer_lock);
}



/**@brief       Timer thread which will execute the core timer isr
 */
static void * timer_thread(void * arg)
{
    (void)arg;

    for (;;) {
        pthread_mutex_lock(&g_timer_lock);
        ncore_lock_enter(NULL);
        ncore_timer_isr();
        ncore_lock_exit(NULL);
    }

    return NULL;
}



/**@brief       Setup timer thread and start the timer
 */
static void timer_init(void)
{
    static struct timeval       timer_value    = {0, 1000000 / CONFIG_CORE_TIMER_EVENT_FREQ};
    static struct timeval       timer_interval = {0, 1000000 / CONFIG_CORE_TIMER_EVENT_FREQ};

    g_timer.it_interval = timer_interval;
    g_timer.it_value    = timer_value;

    pthread_create(&g_timer_thread, NULL, timer_thread, NULL) ;
    setitimer(ITIMER_REAL, &g_timer, 0);
}



/**@brief       Stop and terminate the system timer
 */
static void timer_term(void)
{
    signal(SIGALRM, SIG_DFL);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


/**@brief       Enable the system timer
 */
void ncore_timer_enable(void)
{
    signal(SIGALRM, timer_handler);
}



/**@brief       Disable the system timer
 */
void ncore_timer_disable(void)
{
    signal(SIGALRM, SIG_DFL);
}



void ncore_init(void)
{
    timer_init();
}



void ncore_term(void)
{
    timer_term();
}


/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of p_sys_lock.c
 ******************************************************************************/
