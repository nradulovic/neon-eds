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
 * @brief       x86 64 Linux port core
 *********************************************************************//** @{ */

#ifndef NEON_ARCH_P_CORE_H_
#define NEON_ARCH_P_CORE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <pthread.h>

#include "port/compiler.h"
#include "base/config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Specifies bit-width of general purpose registers
 */
#define NCPU_DATA_WIDTH                     64u

/**@brief       Specifies data alignment for optimal performance
 */
#define NCPU_DATA_ALIGNMENT                 8u

#define NCPU_REG_MAX                        UINT64_MAX

#define NCPU_SIZE_MAX                       UINT64_MAX

#define NCPU_SSIZE_MAX                      INT64_MAX

/**@brief       Specify the number of bits used in ISR priority mask (NOT USED).
 * @note        Since the Linux port is not using interrupts then define this
 *              macro to zero.
 */
#define NCORE_LOCK_LEVEL_BITS           	0u

/**@brief       Convert Neon priority level to port code (NOT USED)
 * @note        Since the Linux port is not using interrupts then define this
 *              macro to zero.
 */
#define NCORE_LOCK_TO_CODE(level)           0u

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       General purpose registers are 64bit wide.
 */
typedef uint64_t ncore_reg;

typedef uint64_t ncpu_size;

typedef int64_t  ncpu_ssize;

struct PORT_C_ALIGN(NCPU_DATA_ALIGNMENT) ncore_ref
{
	uint32_t					value;
};

/**@brief       Interrupt context structure - (not used in this port)
 */
struct ncore_lock
{
    unsigned int                dummy;
};

struct PORT_C_ALIGN(NCPU_DATA_ALIGNMENT) ncore_atomic
{
	int32_t						value;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern pthread_mutex_t          g_idle_lock;
extern pthread_mutex_t          g_global_lock;

/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Computes integer logarithm base 2
 */
PORT_C_INLINE_ALWAYS
uint_fast8_t ncore_log2(
    ncore_reg                    value)
{
    extern const uint_fast8_t   g_log2_lookup[256];

    if (value > 255) {
        return ((uint_fast8_t)(g_log2_lookup[value >> 8] + 8u));
    } else {
        return (g_log2_lookup[value]);
    }
}



/**@brief       Computes integer exponent base 2
 */
PORT_C_INLINE_ALWAYS
ncore_reg ncore_exp2(
    uint_fast8_t                value)
{
    return (0x1u << value);
}



PORT_C_INLINE_ALWAYS
void ncore_ref_write(
    struct ncore_ref *          ref,
	uint32_t					value)
{
	ref->value = value;
}



PORT_C_INLINE_ALWAYS
int32_t ncore_ref_read(
	struct ncore_ref *          ref)
{
    return (ref->value);
}



PORT_C_INLINE_ALWAYS
void ncore_ref_increment(
	struct ncore_ref *      	ref)
{
    if (ref->value != UINT32_MAX) {
        ref++;
    }
}



PORT_C_INLINE_ALWAYS
void ncore_ref_decrement(
	struct ncore_ref *          ref)
{
    if (ref->value != 0u) {
        ref--;
    }
}



PORT_C_INLINE
void ncore_os_ready(void * thread)
{
    (void)thread;

    pthread_mutex_unlock(&g_idle_lock);
}



PORT_C_INLINE
void ncore_os_block(void * thread)
{
    (void)thread;
}



PORT_C_INLINE
void ncore_lock_enter(
    struct ncore_lock *          lock)
{
    (void)lock;

    pthread_mutex_lock(&g_global_lock);
}



PORT_C_INLINE
void ncore_lock_exit(
    struct ncore_lock *          lock)
{
    (void)lock;

    pthread_mutex_unlock(&g_global_lock);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_core.h
 ******************************************************************************/
#endif /* NEON_ARCH_P_CORE_H_ */
