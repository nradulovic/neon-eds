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
 * @brief       Port core
 * @defgroup    port_core_intf Port core
 * @brief       Port core
 *********************************************************************//** @{ */

#ifndef NEON_PORT_CORE_H_
#define NEON_PORT_CORE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "shared/config.h"
#include "arch/p_core.h"

/*===============================================================  MACRO's  ==*/

#define NCORE_TIME_TICK_MAX             UINT32_MAX

/**@brief       Number of valid bits in system lock level
 * @note        This macro is usually defined in port.
 */
#ifndef NCORE_LOCK_LEVEL_BITS
# define NCORE_LOCK_LEVEL_BITS           8u
#endif

/**@brief       Convert generic 0-255 level to port code
 * @note        This macro is usually defined in port.
 */
#ifndef NCORE_LOCK_TO_CODE
# define NCORE_LOCK_TO_CODE(level)       (level)
#endif

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Core timer hardware register type.
 */
typedef uint32_t                ncore_time_tick;

/**@brief       System lock type
 * @note        Structure 'nsys_lock' is defined in port sys_lock.h header.
 */
typedef struct ncore_lock       ncore_lock;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Initialise the CPU
 * @details     This function is port specific. It may setup default execution
 *              model for a given CPU, set the performance to default etc.
 *
 *              This function should be called early in boot process.
 */
void ncore_init(void);



void ncore_term(void);



uint_fast8_t ncore_log2(
    ncpu_reg                    value);


ncpu_reg ncore_exp2(
    uint_fast8_t                value);



void ncore_sat_increment(
    ncpu_reg *                  value);



void ncore_sat_decrement(
    ncpu_reg *                  value);



/**@brief       Enter critical code section
 * @param       lock
 *              Interrupt resource lock, pointer to portable type variable which
 *              will hold the interrupt context state during the critical code
 *              section.
 */
void ncore_lock_enter(
    struct ncore_lock *          lock);



/**@brief       Exit critical code section
 * @param       lock
 *              Interrupt resource lock, portable type variable which is holding
 *              a previously saved interrupt context state.
 */
void ncore_lock_exit(
    struct ncore_lock *          lock);



/**@brief       Enable the system timer
 */
void ncore_timer_enable(void);



/**@brief       Disable the system timer
 */
void ncore_timer_disable(void);



/**@brief       User System Timer ISR
 */
extern void ncore_timer_isr(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of core.h
 ******************************************************************************/
#endif /* NEON_PORT_CORE_H_ */
