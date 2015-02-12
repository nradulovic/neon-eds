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

#include <plat/compiler.h>
#include <stdint.h>

#include "shared/config.h"
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
    (UINT64_MAX / NSYSTIMER_ONE_TICK)

#define NSYSTIMER_TICK_MAX              INT64_MAX


/**@} *//*----------------------------------------------------------------*//**
 * @name        System timer management
 * @{ *//*--------------------------------------------------------------------*/



/** @} *//*---------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       System timer hardware register type.
 */
typedef signed long nsystimer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Initialise and start the system timer
 */
void nsystimer_init(
	nsystimer_tick            	val);



/**@brief       Stop and terminate the system timer
 */
void nsystimer_term(void);



/**@brief       Get free counter value
 */
nsystimer_tick nsystimer_get_current(void);



/**@brief       Get reload counter value
 */
nsystimer_tick nsystimer_get_reload(void);



/**@brief       Load the system timer Reload value register
 */
void nsystimer_load(
    nsystimer_tick              tick);


/**@brief       Enable the system timer
 */
void nsystimer_enable(void);



/**@brief       Disable the system timer
 */
void nsystimer_disable(void);



/**@brief       Initialize port
 */
void nsystimer_module_init(void);



/**@brief       Terminate port
 */
void nsystimer_module_term(void);



/**@brief       User System Timer ISR
 */
extern void nsystimer_isr(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* ES_ARCH_SYSTIMER_H_ */
