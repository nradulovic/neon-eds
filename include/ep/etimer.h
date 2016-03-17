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
 * @brief       Event timer
 * @defgroup    event_timer Event timer
 * @brief       Event timer
 *********************************************************************//** @{ */

#ifndef NEON_EP_ETIMER_H_
#define NEON_EP_ETIMER_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

#include "base/config.h"
#include "timer/timer.h"
#include "ep/event.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Validate the pointer to Event Timer object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#define N_IS_ETIMER_OBJECT(etimer_obj)                                          \
    (((etimer_obj) != NULL) && ((etimer_obj)->signature == NSIGNATURE_ETIMER))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nepa;

struct netimer
{
    struct ntimer               timer;
    struct nevent				event;
    struct nepa *               client;
#if (CONFIG_API_VALIDATION == 1)
    unsigned int                signature;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


void netimer_init(
    struct netimer *            timer);



void netimer_term(
    struct netimer *            timer);



void netimer_after(
    struct netimer *            timer,
    ncore_time_tick             tick,
    uint16_t                    event_id);



void netimer_every(
    struct netimer *            timer,
    ncore_time_tick             tick,
    uint16_t                    event_id);



void netimer_cancel(
    struct netimer *            timer);



bool netimer_is_running_i(
    const struct netimer *      timer);



ncore_time_tick netimer_remaining(
    const struct netimer *      timer);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of etimer.h
 ******************************************************************************/
#endif /* NEON_EP_ETIMER_H_ */
