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
 * @brief       Interface of System Control port - Template
 * @addtogroup  template_sysctrl
 * @brief       System Control interface
 *********************************************************************//** @{ */

#if !defined(SYSCTRL_H__)
#define SYSCTRL_H__

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "arch/compiler.h"
#include "arch/cpu.h"
#include "arch/sysctrl_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        System timer constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       System timer reload value for one tick
 * @details     This is a calculated value for one system tick period
 */
#define PORT_DEF_SYSTMR_ONE_TICK                                                \
    (PORT_CFG_SYSTMR_CLOCK_FREQ / CFG_SYSTMR_EVENT_FREQUENCY)

/**@brief       Maximum number of ticks without overflowing the system timer
 * @details     This macro expects that CPU_DEF_SYSTMR_MAX_VAL is set in CPU
 *              profile
 */
#define PORT_DEF_SYSTMR_MAX_TICKS                                               \
    (PORT_DEF_SYSTMR_MAX_VAL / PORT_DEF_SYSTMR_ONE_TICK)

/**@} *//*----------------------------------------------------------------*//**
 * @name        System timer management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize system timer and associated interrupt
 * @param       val
 *              Value of system timer which will be loaded into the register
 * @details     This macro will only initialize system timer and associated
 *              interrupt. The macro is called from esKernStart() function.
 *              Responsibility:
 *              - initialize system timer
 *              - initialize system timer interrupt
 * @note        This macro MUST NOT enable system timer events. System timer
 *              events are enabled/disabled by PORT_SYSTMR_ISR_ENABLE() and
 *              PORT_SYSTMR_ISR_DISABLE() macros.
 */
#define PORT_SYSTMR_INIT(val)           (void)0

/**@brief       Stop the timer if it is running and disable associated interrupt.
 * @details     Responsibility:
 *              - disable system timer interrupt
 *              - stop and disable system timer
 */
#define PORT_SYSTMR_TERM()              (void)0

/**@brief       Get system timer reload value
 */
#define PORT_SYSTMR_GET_RVAL()          (void)0

/**@brief       Get system timer current value
 */
#define PORT_SYSTMR_GET_CVAL()          (void)0

/**@brief       Reload the system timer with specified number
 * @param       val
 *              Value of system timer which will be reloaded into the register
 * @details     Responsibility:
 *              - stop the system timer
 *              - reload the system timer
 *              - start the system timer
 */
#define PORT_SYSTMR_RLD(val)            (void)0

/**@brief       Enable the system timer
 * @details     Responsibility:
 *              - enable (run) the system timer counter
 */
#define PORT_SYSTMR_ENABLE()            (void)0

/**@brief       Disable the system timer
 * @details     Responsibility:
 *              - disable (stop) the system timer counter
 */
#define PORT_SYSTMR_DISABLE()           (void)0

/**@brief       Enable the system timer interrupt
 * @details     Responsibility:
 *              - allow system timer interrupt to occur
 */
#define PORT_SYSTMR_ISR_ENABLE()        (void)0

/**@brief       Disable the system timer interrupt
 * @details     Responsibility:
 *              - disallow system timer interrupt to occur
 */
#define PORT_SYSTMR_ISR_DISABLE()       (void)0

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       System timer hardware register type.
 */
typedef uint32_t portSysTmrReg_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of systmr.h
 ******************************************************************************/
#endif /* SYSCTRL_H__ */
