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
 * @brief       System timer
 * @defgroup    system_timer_intf System timer
 * @brief       System timer
 *********************************************************************//** @{ */

#ifndef NEON_PORT_SYSTIMER_H_
#define NEON_PORT_SYSTIMER_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "shared/config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Core timer one tick value
 */
#define NSYSTIMER_ONE_TICK(clock)                                               \
    (((clock) + (CONFIG_SYSTIMER_EVENT_FREQ / 2)) / CONFIG_SYSTIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the core timer
 */
#define NSYSTIMER_MAX_TICKS                                                     \
    (UINT32_MAX / NSYSTIMER_ONE_TICK)

#define NSYSTIMER_TICK_MAX              UINT32_MAX

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Core timer hardware register type.
 */
typedef uint32_t nsystimer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


void nsystimer_init(
    nsystimer_tick              val);


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



/**@brief       Disable the system timer interrupt
 */
void nsystimer_isr_enable(void);



/**@brief       Enable the system timer interrupt
 */
void nsystimer_isr_disable(void);



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
/** @endcond *//** @} *//** @} *//*********************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* NEON_PORT_SYSTIMER_H_ */
