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
 * @brief       Event timer implementation
 * @addtogroup  event_timer
 *********************************************************************//** @{ */
/**@defgroup    event_timer_impl Implementation
 * @brief       Event timer Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/debug.h"
#include "ep/epa.h"
#include "ep/event.h"
#include "ep/etimer.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void etimer_handler(void * arg);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void etimer_handler(void * arg)
{
    struct netimer *            timer = arg;

    nepa_send_event_i(timer->epa, &timer->event);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void netimer_init(
    struct netimer *            timer)
{
	NREQUIRE(!N_IS_ETIMER_OBJECT(timer));

    ntimer_init(&timer->timer);
    timer->event  = g_default_event;
    timer->epa = nepa_get_current();

    NOBLIGATION(NSIGNATURE_IS(timer, NSIGNATURE_ETIMER));
}



void netimer_term(
    struct netimer *            timer)
{
    NREQUIRE(N_IS_ETIMER_OBJECT(timer));

    ntimer_term(&timer->timer);

    NOBLIGATION(NSIGNATURE_IS(timer, ~NSIGNATURE_ETIMER));
}



void netimer_after(struct netimer * timer, uint32_t tick, uint16_t event_id)
{
    ncore_lock                  lock;

    NREQUIRE(N_IS_ETIMER_OBJECT(timer));

    ncore_lock_enter(&lock);
    ntimer_cancel_i(&timer->timer);
    timer->event.id = event_id;
    ntimer_start_i(&timer->timer, tick, etimer_handler, timer,
            NTIMER_ATTR_ONE_SHOT);
    ncore_lock_exit(&lock);
}



void netimer_every(struct netimer * timer, uint32_t tick, uint16_t event_id)
{
    ncore_lock                  lock;

    NREQUIRE(N_IS_ETIMER_OBJECT(timer));

    ncore_lock_enter(&lock);
    ntimer_cancel_i(&timer->timer);
    timer->event.id = event_id;
    ntimer_start_i(
            &timer->timer,
            tick,
            etimer_handler,
            timer,
            NTIMER_ATTR_REPEAT);
    ncore_lock_exit(&lock);
}



void netimer_cancel(struct netimer * timer)
{
	NREQUIRE(N_IS_ETIMER_OBJECT(timer));

    /* Make this event NULL event. This is used to NULLify event even if it was
     * delivered to EPA.
     */
    timer->event.id = NSM_NULL;
    ntimer_cancel(&timer->timer);
}



bool netimer_is_running_i(const struct netimer * timer)
{
	NREQUIRE(N_IS_ETIMER_OBJECT(timer));

    return (ntimer_is_running_i(&timer->timer));
}



uint32_t netimer_remaining(const struct netimer * timer)
{
	NREQUIRE(N_IS_ETIMER_OBJECT(timer));

    return (ntimer_remaining(&timer->timer));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of equeue.c
 ******************************************************************************/
