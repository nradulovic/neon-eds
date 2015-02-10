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
 * @brief       Interface of ARM Cortex CPU module port.
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-v7-m-cpu ARM Cortex M3/M4 CPU module
 * @brief       CPU module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_CPU_H_
#define ES_CPU_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <plat/compiler.h>
#include <stdint.h>

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

/**@} *//*----------------------------------------------------------------*//**
 * @name        Bit operations
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Find Last Set bit in a word
 */
#define ES_CPU_FLS(val)                 portCpuFls_(val)

/**@brief       Compute power of 2
 */
#define ES_CPU_PWR2(pwr)                (0x01u << (pwr))

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

#define ES_CPU_INIT_EARLY()             (void)0

#define ES_CPU_INIT()                   (void)0

#define ES_CPU_INIT_LATE()              (void)0

#define ES_CPU_TERM()                   (void)0

/**@} *//*-----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       General purpose registers are 32bit wide.
 */
typedef unsigned long ncpu_reg;

typedef unsigned int ncpu_size;

typedef signed   int ncpu_ssize;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Stop the further CPU execution
 */
PORT_C_INLINE
void ncpu_stop(void)
{
    for (;;) {
        /*
         * TODO: Put the CPU to sleep
         */
    }
}



/**@brief       Computes integer logarithm base 2
 */
PORT_C_INLINE
uint_fast8_t ncpu_log2(
    ncpu_reg                    value)
{
    extern const uint_fast8_t   g_log2_lookup[256];

    if (value > 255) {
        return (g_log2_lookup[value >> 8]);
    } else {
        return (g_log2_lookup[value]);
    }
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

/**@brief       Initialize port
 */
void ncpu_module_init(void);



/**@brief       Terminate port
 */
void ncpu_module_term(void);



extern void nkernel_isr(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of cpu.h
 ******************************************************************************/
#endif /* ES_CPU_H_ */
