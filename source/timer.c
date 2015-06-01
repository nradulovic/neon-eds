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
 * @brief       Virtual timer implementation
 * @addtogroup  timer_timer
 *********************************************************************//** @{ */
/**@defgroup    timer_timer_impl Implementation
 * @brief       Virtual timer implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "port/core.h"
#include "base/component.h"
#include "base/debug.h"
#include "timer/timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define NODE_TO_TIMER(node)                                                     \
    PORT_C_CONTAINER_OF(node, struct ntimer, list)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Virtual timer");

static struct ntimer g_timer_sentinel =
{
    NDLIST_INIT(&g_timer_sentinel.list),
    NCORE_TIME_TICK_MAX,
    0,
    NULL,
    NULL,
#if (CONFIG_API_VALIDATION == 1)
    NSIGNATURE_TIMER
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static void insert_timer(
    struct ntimer *         timer)
{
    struct ntimer *         current;

    current = NODE_TO_TIMER(ndlist_next(&g_timer_sentinel.list));

    while (current->rtick < timer->rtick) {
        timer->rtick -= current->rtick;
        current       = NODE_TO_TIMER(ndlist_next(&current->list));
    }
    ndlist_add_before(&current->list, &timer->list);

    if (&g_timer_sentinel != current) {
        current->rtick -= timer->rtick;
    }
}



static void remove_timer(
    struct ntimer *         timer)
{
    ndlist_remove(&timer->list);
    ndlist_init(&timer->list);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void ntimer_init(
    struct ntimer *             timer)
{
    NREQUIRE(NAPI_POINTER, timer != NULL);
    NREQUIRE(NAPI_OBJECT,  timer->signature != NSIGNATURE_TIMER);

    ndlist_init(&timer->list);

    NOBLIGATION(timer->signature = NSIGNATURE_TIMER);
}



void ntimer_term(
    struct ntimer *             timer)
{
    NREQUIRE(NAPI_OBJECT, N_IS_TIMER_OBJECT(timer));

    ntimer_cancel(timer);

    NOBLIGATION(timer->signature = ~NSIGNATURE_TIMER);
}



void ntimer_cancel_i(
    struct ntimer *             timer)
{
    NREQUIRE(NAPI_OBJECT, N_IS_TIMER_OBJECT(timer));

    if (ntimer_is_running_i(timer)) {

        if (&g_timer_sentinel != NODE_TO_TIMER(ndlist_next(&timer->list))) {
            NODE_TO_TIMER(ndlist_next(&timer->list))->rtick += timer->rtick;
        }
        remove_timer(timer);
    }
}



void ntimer_cancel(
    struct ntimer *             timer)
{
    ncore_lock                  sys_lock;

    ncore_lock_enter(&sys_lock);
    ntimer_cancel_i(timer);
    ncore_lock_exit(&sys_lock);
}



void ntimer_start_i(
    struct ntimer *             timer,
    ncore_time_tick             tick,
    void                     (* fn)(void *),
    void *                      arg,
    uint8_t                     flags)
{
    NREQUIRE(NAPI_OBJECT, N_IS_TIMER_OBJECT(timer));
    NREQUIRE(NAPI_RANGE,   tick > 0);
    NREQUIRE(NAPI_POINTER, fn != NULL);
    NREQUIRE(NAPI_USAGE, !ntimer_is_running_i(timer));

    timer->fn    = fn;
    timer->arg   = arg;
    timer->rtick = tick;

    if (flags & NTIMER_ATTR_REPEAT) {
        timer->itick = tick;
    } else {
        timer->itick = 0u;
    }
    insert_timer(timer);
}



void ntimer_start(
    struct ntimer *             timer,
    ncore_time_tick              tick,
    void                     (* fn)(void *),
    void *                      arg,
    uint8_t                     flags)
{
    ncore_lock                   sys_lock;

    ncore_lock_enter(&sys_lock);
    ntimer_start_i(timer, tick, fn, arg, flags);
    ncore_lock_exit(&sys_lock);
}



bool ntimer_is_running_i(
    const struct ntimer *       timer)
{
    NREQUIRE(NAPI_OBJECT, N_IS_TIMER_OBJECT(timer));

    if (!ndlist_is_empty(&timer->list)) {
        return (true);
    } else {

        return (false);
    }
}



ncore_time_tick ntimer_remaining(
    const struct ntimer *       timer)
{
    ncore_lock                   sys_lock;
    ncore_time_tick              remaining;

    NREQUIRE(NAPI_OBJECT, N_IS_TIMER_OBJECT(timer));

    remaining = 0u;
    ncore_lock_enter(&sys_lock);

    if (ntimer_is_running_i(timer)) {

        do {
            remaining += timer->rtick;
            timer      = NODE_TO_TIMER(ndlist_prev(&timer->list));
        } while (timer != &g_timer_sentinel);
    }
    ncore_lock_exit(&sys_lock);

    return (remaining);
}



void ncore_timer_isr(void)
{
    if (!ndlist_is_empty(&g_timer_sentinel.list)) {
        struct ntimer *         current;

        current = NODE_TO_TIMER(ndlist_next(&g_timer_sentinel.list));
        NASSERT_INTERNAL("corrupted timer list", current->signature == NSIGNATURE_TIMER);
        --current->rtick;

        while (current->rtick == 0u) {
            struct ntimer *     tmp;

            remove_timer(current);

            if (current->itick != 0u) {
                current->rtick = current->itick;
                insert_timer(current);
            }
            tmp     = current;
            current = NODE_TO_TIMER(ndlist_next(&g_timer_sentinel.list));
            NASSERT_INTERNAL("corrupted timer list", current->signature == NSIGNATURE_TIMER);
            tmp->fn(tmp->arg);
        }
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of timer.c
 ******************************************************************************/
