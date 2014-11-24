/*
 * This file is part of Neon RT Kernel.
 *
 * Copyright (C) 2010 - 2014 Nenad Radulovic
 *
 * Neon RT Kernel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon RT Kernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon RT Kernel.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Port core module header
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-v7-m-core ARM Cortex M3/M4 CPU module
 * @brief       Port core module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ARCH_CPU_H
#define ARCH_CPU_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stdint.h>

#include "common/compiler.h"

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

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief General purpose registers are 32bit wide.
 */
typedef unsigned int ncpu_reg;

typedef unsigned int ncpu_size;

typedef   signed int ncpu_ssize;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Stop the further CPU execution
 */
PORT_C_INLINE
void ncpu_stop(void)
{
    while (true) {
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



/**@brief       Initialise CPU port
 */
void ncpu_module_init(void);



/**@brief       Terminate CPU port
 */
void ncpu_module_term(void);



extern void nkernel_isr(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of cpu.h
 ******************************************************************************/
#endif /* ARCH_CPU_H */
