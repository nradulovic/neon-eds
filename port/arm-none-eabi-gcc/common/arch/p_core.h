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

#ifndef NEON_ARCH_P_CORE_H_
#define NEON_ARCH_P_CORE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "base/port/compiler.h"
#include "base/port/profile.h"
#include "base/shared/config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Specifies bit-width of general purpose registers
 */
#define NCPU_DATA_WIDTH                     32u

/**@brief       Specifies data alignment for optimal performance
 */
#define NCPU_DATA_ALIGNMENT                 4u

#define NCPU_REG_MAX                        UINT32_MAX

#define NCPU_SIZE_MAX                       UINT32_MAX

#define NCPU_SSIZE_MAX                      INT32_MAX

#define NCORE_LOCK_TO_CODE(level)           (255 - (level))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief General purpose registers are 32bit wide.
 */
typedef unsigned int ncpu_reg;

typedef unsigned int ncpu_size;

typedef   signed int ncpu_ssize;

/**@brief       Interrupt context structure
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
struct ncore_lock
{
    unsigned int                level;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Stop the further CPU execution
 */
PORT_C_INLINE
void ncpu_stop(void)
{
    for (;;) {
    __asm__ __volatile__ (
        "@  ncpu_stop                                       \n"
        "   wfe                                             \n");
    }
}



/**@brief       Computes integer logarithm base 2
 */
PORT_C_INLINE_ALWAYS
uint_fast8_t ncpu_log2(
    ncpu_reg                    value)
{
    uint_fast8_t                clz;

    __asm__ __volatile__ (
        "@  ncpu_log2                                       \n"
        "   clz    %0, %1                                   \n"
        : "=r"(clz)
        : "r"(value));

    return (31u - clz);
}



/**@brief       Computes integer exponent base 2
 */
PORT_C_INLINE_ALWAYS
ncpu_reg ncpu_exp2(
    uint_fast8_t                value)
{
    return (0x1u << value);
}



PORT_C_INLINE_ALWAYS
void ncpu_sat_increment(
    ncpu_reg *                  value)
{
    if (*value != NCPU_REG_MAX) {
        (*value)++;
    }
}



PORT_C_INLINE_ALWAYS
void ncpu_sat_decrement(
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
#if (CONFIG_CORE_LOCK_MAX_LEVEL != 255)
    unsigned int                new_mask;

    new_mask = (((CONFIG_CORE_LOCK_MAX_LEVEL) << (8u - NCORE_LOCK_LEVEL_BITS)) & 0xfful);

    __asm __volatile__ (
        "@  nsys_lock_enter                                 \n"
        "   mrs     %0, basepri                             \n"
        "   msr     basepri, %1                             \n"
        : "=&r"(lock->level)
        : "r"(new_mask));
#else
    unsigned int                new_mask;

    new_mask = 1;

    __asm __volatile__ (
        "@  nsys_lock_enter                                 \n"
        "   mrs     %0, primask                             \n"
        "   msr    primask, %1                              \n"
        : "=&r"(lock->level)
        : "r"(new_mask));
#endif
}



PORT_C_INLINE
void ncore_lock_exit(
    struct ncore_lock *          lock)
{
#if (CONFIG_CORE_LOCK_MAX_LEVEL != 255)
    __asm __volatile__ (
        "@  nsys_lock_exit                                  \n"
        "   msr    basepri, %0                              \n"
        :
        : "r"(lock->level));
#else
    __asm __volatile__ (
        "@  nsys_lock_exit                                  \n"
        "   msr    primask, %0                              \n"
        :
        : "r"(lock->level));
#endif
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
