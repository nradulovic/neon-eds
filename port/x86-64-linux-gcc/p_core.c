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
 * @brief       Core portable layer
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


static void timer_init(void);



static void timer_term(void);

/*=======================================================  LOCAL VARIABLES  ==*/

struct timeval my_value = {0,10000};
struct timeval my_interval = {0,10000};
struct itimerval my_timer;

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



static void handler(int signal)
{
    (void)signal;

    //ncore_lock_enter(NULL);
    ncore_timer_isr();
    //ncore_lock_exit(NULL);
}

static void timer_init(void)
{
    my_timer.it_interval = my_interval;
    my_timer.it_value    = my_value;

    setitimer(ITIMER_REAL, &my_timer, 0);
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
    signal(SIGALRM, handler);
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
