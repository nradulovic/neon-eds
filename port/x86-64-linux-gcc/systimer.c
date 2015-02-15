/*
 * systimer.c
 *
 *  Created on: Dec 14, 2014
 *      Author: nenad
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "port/systimer.h"
#include "port/sys_lock.h"
#include "shared/debug.h"
#include "shared/component.h"

static NCOMPONENT_DEFINE("System timer", "Nenad Radulovic");

static struct sigaction         sa;
static sigset_t                 mask;
static struct sigevent          sev;
static struct itimerspec        its;
static timer_t                  timerid;

static void handler(int sig, siginfo_t *si, void *uc)
{
    (void)si;
    (void)uc;

    nsys_lock_enter(NULL);
    nsystimer_isr();
    nsys_lock_exit(NULL);
    signal(sig, SIG_IGN);
}

void nsystimer_init(
    nsystimer_tick              val)
{


    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        NASSERT_ALWAYS("Failed to set handler");
    }
    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
        NASSERT_ALWAYS("Failed to mask timer");
    }
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;

    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        NASSERT_ALWAYS("Failed to create timer");
    }

    nsystimer_load(val);
}

void nsystimer_term(void)
{
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
        NASSERT_ALWAYS("Failed to terminate timer");
    }
}



nsystimer_tick nsystimer_get_current(void)
{
    return (its.it_value.tv_nsec);
}



nsystimer_tick nsystimer_get_reload(void)
{
    return (its.it_interval.tv_nsec);
}



void nsystimer_load(
    nsystimer_tick              tick)
{
    its.it_value.tv_sec     = 0;
    its.it_value.tv_nsec    = tick;
    its.it_interval.tv_sec  = 0;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        NASSERT_ALWAYS("Failed to setup timer");
    }
}



void nsystimer_enable(void)
{
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        NASSERT_ALWAYS("Failed to unmask timer");
    }
}



void nsystimer_disable(void)
{
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
        NASSERT_ALWAYS("Failed to mask timer");
    }
}



