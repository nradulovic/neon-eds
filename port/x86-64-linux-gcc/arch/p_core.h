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
 * @defgroup    x86_64_linux_port_core_intf x86 64 Linux port core
 * @brief       x86 64 Linux port core
 *********************************************************************//** @{ */

#ifndef NEON_ARCH_P_CORE_H_
#define NEON_ARCH_P_CORE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <pthread.h>

#include "port/compiler.h"
#include "shared/config.h"

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

/**@brief       Specify the number of bits used in ISR priority mask. Since the 
 *              Linux port is not using interrupts then define this to zero.
 */
#define NCORE_LOCK_LEVEL_BITS           	1u

#define NCORE_LOCK_TO_CODE(level)           (255 - (level)) 

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief General purpose registers are 64bit wide.
 */
typedef uint64_t ncpu_reg;

typedef uint64_t ncpu_size;

typedef int64_t  ncpu_ssize;

/**@brief       Interrupt context structure - (not used in this port)
 */
struct ncore_lock
{
    unsigned int                dummy;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern pthread_mutex_t          g_global_lock;

/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Computes integer logarithm base 2
 */
PORT_C_INLINE_ALWAYS
uint_fast8_t ncore_log2(
    ncpu_reg                    value)
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
ncpu_reg ncore_exp2(
    uint_fast8_t                value)
{
    return (0x1u << value);
}



PORT_C_INLINE_ALWAYS
void ncore_sat_increment(
    ncpu_reg *                  value)
{
    if (*value != NCPU_REG_MAX) {
        (*value)++;
    }
}



PORT_C_INLINE_ALWAYS
void ncore_sat_decrement(
    ncpu_reg *                  value)
{
    if (*value != 0u) {
        (*value)--;
    }
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
