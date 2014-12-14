/*
 * This file is part of eSolid.
 *
 * Copyright (C) 2010 - 2013 Nenad Radulovic
 *
 * eSolid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with eSolid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Interface of ARM Cortex-M3 System Timer port.
 * @addtogroup  arm-none-eabi-gcc-v7-m
 *********************************************************************//** @{ */

#ifndef ES_ARCH_SYSTIMER_H_
#define ES_ARCH_SYSTIMER_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "plat/compiler.h"
#include "arch/systimer_config.h"
#include "family/profile.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Core timer one tick value
 */
#define NSYSTIMER_ONE_TICK(clock)                                               \
    (((clock) + (CONFIG_SYSTIMER_EVENT_FREQ / 2)) / CONFIG_SYSTIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the core timer
 */
#define NSYSTIMER_MAX_TICKS                                                     \
    (UINT32_MAX / NSYSTIMER_ONE_TICK)

#define NSYSTIMER_TICK_MAX              UINT32_MAX


/**@} *//*----------------------------------------------------------------*//**
 * @name        System timer management
 * @{ *//*--------------------------------------------------------------------*/

#define ES_SYSTIMER_INIT_EARLY()        (void)0

#define ES_SYSTIMER_INIT(val)           (void)0

#define ES_SYSTIMER_TERM()              (void)0

#define ES_SYSTIMER_GET_RVAL()          (void)0

#define ES_SYSTIMER_GET_CVAL()          0

#define ES_SYSTIMER_RELOAD(val)         (void)0

#define ES_SYSTIMER_ENABLE()            (void)0

#define ES_SYSTIMER_DISABLE()           (void)0

#define ES_SYSTIMER_ISR_ENABLE()        (void)0

#define ES_SYSTIMER_ISR_DISABLE()       (void)0

#define ES_SYSTIMER_SET_HANDLER(handler, level)                                 \
    (void)0

/** @} *//*---------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       System timer hardware register type.
 */
typedef unsigned int nsystimer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* ES_ARCH_SYSTIMER_H_ */
