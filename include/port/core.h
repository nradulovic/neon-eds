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
#include <stdbool.h>

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
# define NCORE_LOCK_TO_CODE(level)      (level)
#endif

#define NCORE_ATOMIC_INIT(v)			{v}

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Core timer hardware register type.
 */
typedef uint32_t                ncore_time_tick;

/**@brief       System lock type
 * @note        Structure 'ncore_lock' is defined in port p_core.h header.
 */
typedef struct ncore_lock       ncore_lock;

/**@brief		Atomic type
 * @note        Structure 'ncore_atomic' is defined in port p_core.h header.
 */
typedef struct ncore_atomic		ncore_atomic;

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
    ncore_reg                   value);


ncore_reg ncore_exp2(
    uint_fast8_t                value);



/**@brief       Write the value to reference integer value
 */
void ncore_ref_write(
    struct ncore_ref *          ref,
	uint32_t					value);



/**@brief       Read the value from reference integer value
 */
int32_t ncore_ref_read(
	struct ncore_ref *          ref);



/**@brief       Increment reference integer value
 */
void ncore_ref_increment(
	struct ncore_ref *      	ref);



/**@brief       Decrement reference integer value
 */
void ncore_ref_decrement(
	struct ncore_ref *          ref);



/**@brief		Atomically read the integer value of v
 */
int32_t ncore_atomic_read(struct ncore_atomic * v);



/**@brief		Atomically set v equal to i
 */
void ncore_atomic_write(struct ncore_atomic * v, int32_t i);



/**@brief		Atomically increment v by one
 */
void ncore_atomic_inc(struct ncore_atomic * v);


/**@brief		Atomically decrement v by one
 */
void ncore_atomic_dec(struct ncore_atomic * v);


/**@brief		Atomically increment v by one and return true if zero; false
 * 				otherwise
 */
bool ncore_atomic_inc_and_test(struct ncore_atomic * v);


/**@brief		Atomically decrement v by one and return true if zero; false
 * 				otherwise
 */
bool ncore_atomic_dec_and_test(struct ncore_atomic * v);



void ncore_idle(void);



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
