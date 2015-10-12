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

#if __STDC_VERSION__ >= 199901L
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include "port/core.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static void lock_init(void);



static void lock_term(void);



static void timer_handler(int signal);



static void * timer_thread(void * arg);



static void timer_init(void);



static void timer_term(void);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct sigaction         g_sigaction;
static pthread_t                g_timer_thread;
static pthread_mutex_t          g_timer_lock;

/*======================================================  GLOBAL VARIABLES  ==*/

pthread_mutex_t                 g_idle_lock;
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



static void lock_init(void)
{
    pthread_mutexattr_t         attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&g_global_lock, &attr);
}



static void lock_term(void)
{
    pthread_mutex_destroy(&g_global_lock);
}



/**@brief       Timer signal handler for signaling to timer thread
 */
static void timer_handler(int sig_no)
{
    (void)sig_no;

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
    pthread_mutex_init(&g_timer_lock, NULL);
    pthread_create(&g_timer_thread, NULL, timer_thread, NULL);

    memset (&g_sigaction, '\0', sizeof(g_sigaction));

    g_sigaction.sa_handler = &timer_handler;

    if (sigaction(SIGALRM, &g_sigaction, NULL) < 0) {
        perror ("sigaction");
        exit(1);
    }
}



/**@brief       Stop and terminate the system timer
 */
static void timer_term(void)
{
    ncore_timer_disable();
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void ncore_init(void)
{
    pthread_mutexattr_t         attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&g_idle_lock, NULL);

    lock_init();
    timer_init();
}

#include <errno.h>


void ncore_term(void)
{
    fflush(stdout);
    while (pthread_mutex_trylock(&g_idle_lock) == EBUSY) {
        pthread_mutex_unlock(&g_idle_lock);
    }
    pthread_mutex_unlock(&g_idle_lock);
    pthread_mutex_destroy(&g_idle_lock);
    timer_term();
    lock_term();
}



void ncore_idle(void)
{
    pthread_mutex_lock(&g_idle_lock);
}



/**@brief       Enable the system timer
 */
void ncore_timer_enable(void)
{
    struct itimerval            timer;

    timer.it_value.tv_sec     = 0;
    timer.it_value.tv_usec    = 1000000 / CONFIG_CORE_TIMER_EVENT_FREQ;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_usec = 1000000 / CONFIG_CORE_TIMER_EVENT_FREQ;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }
}



/**@brief       Disable the system timer
 */
void ncore_timer_disable(void)
{
    struct itimerval            timer;

    memset(&timer, 0, sizeof(timer));

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of p_sys_lock.c
 ******************************************************************************/
