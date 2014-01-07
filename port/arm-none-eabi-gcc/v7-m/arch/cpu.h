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
 * @brief       Interface of ARM Cortex-M3 CPU port.
 * @addtogroup  arm-none-eabi-gcc-v7-m
 *********************************************************************//** @{ */

#if !defined(CPU_H__)
#define CPU_H__

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include "arch/compiler.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_DEF_DATA_WIDTH             32u

#define PORT_DEF_DATA_ALIGNMENT         4u

/**@} *//*----------------------------------------------------------------*//**
 * @name        Bit operations
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_CPU_FLS(val)               portCpuFLS_(val)

#define PORT_CPU_PWR2(pwr)              (0x01u << (pwr))

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_CPU_INIT_EARLY()           (void)0                                 /**< @brief This port does not need this function call      */

#define PORT_CPU_INIT()                 portCpuInit()

#define PORT_CPU_INIT_LATE()            (void)0                                 /**< @brief This port does not need this function call      */

#define PORT_CPU_TERM()                 portCpuTerm()

/**@} *//*----------------------------------------------------------------*//**
 * @name        CPU register descriptions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       PSR Thumb state: Position.
 */
#define CPU_PSR_THUMB_STATE_Pos         (24u)

/**@brief       PSR Thumb state: Mask.
 */
#define CPU_PSR_THUMB_STATE_Msk         (0x01ul << CPU_PSR_THUMB_STATE_Pos)

/** @} *//*---------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Bit operations
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Find last set bit in a word
 * @param       value
 *              32 bit value which will be evaluated
 * @return      Last set bit in a word
 * @details     This implementation uses @c clz instruction and then it computes
 *              the result using the following expression:
 *              <code>fls(x) = w − clz(x)</code>.
 * @inline
 */
static PORT_C_INLINE_ALWAYS uint_fast8_t portCpuFLS_(
    esAtomic       value) {

    uint_fast8_t    clz;

    __asm__ __volatile__ (
        "   clz    %0, %1                                   \n"
        : "=r"(clz)
        : "r"(value));

    return (31u - clz);
}

/** @} *//*---------------------------------------------------------------*//**
 * @name        Generic port functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize port
 */
void portCpuInit(
    void);

/**@brief       Terminate port
 */
void portCpuTerm(
    void);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of cpu.h
 ******************************************************************************/
#endif /* CPU_H__ */
