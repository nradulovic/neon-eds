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

#include "eds/epa.h"
#include "eds/event.h"
#include "eds/etimer.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

void etimer_handler(
	void * 						arg);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

void etimer_handler(
	void * 						arg)
{
	struct netimer *			timer = arg;
	struct nevent *				event;

	event = nevent_create_i(sizeof(struct nevent), timer->event_id);

	if (event) {
		nepa_send_event_i(timer->client, event);
	}
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void netimer_init(
	struct netimer * 			timer)
{
	ntimer_init(&timer->timer);
}


void netimer_after(
	struct netimer * 			timer,
	nsystimer_tick              tick,
	uint16_t     				event_id)
{
	timer->client 	= nepa_get_current();
	timer->event_id = event_id;
	ntimer_start(&timer->timer, tick, etimer_handler, timer, NTIMER_ATTR_ONE_SHOT);
}



void netimer_every(
	struct netimer *			timer,
	nsystimer_tick				tick,
	uint16_t    				event_id)
{
	timer->client 	= nepa_get_current();
	timer->event_id = event_id;
	ntimer_start(&timer->timer, tick, etimer_handler, timer, NTIMER_ATTR_REPEAT);
}



void netimer_cancel(
	struct netimer *			timer)
{
	ntimer_cancel(&timer->timer);
}



bool netimer_is_running_i(
    const struct netimer *      timer)
{
	return (ntimer_is_running_i(&timer->timer));
}



nsystimer_tick netimer_remaining(
    const struct netimer *      timer)
{
	return (ntimer_remaining(&timer->timer));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of equeue.c
 ******************************************************************************/
