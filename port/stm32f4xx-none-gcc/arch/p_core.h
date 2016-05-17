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
 * @brief       STM32F4xx port core
 *********************************************************************//** @{ */

#ifndef NEON_ARCH_P_CORE_H_
#define NEON_ARCH_P_CORE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "port/compiler.h"
#include "base/config.h"

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

/**@brief       Specify the number of bits used in ISR priority mask. For now
 *              all STM32F4xx series MCU's implement 4 bits
 */
#define NCORE_LOCK_LEVEL_BITS               4u

/**@brief       Convert Neon priority level to port code
 */
#define NCORE_LOCK_TO_CODE(lock)                                                \
    ((255 - (lock)) >> NCORE_LOCK_LEVEL_BITS)

/**@brief       Convert port code to Neon priority level
 */
#define NCORE_CODE_TO_LOCK(code)                                                \
    (255 - ((code) << NCORE_LOCK_LEVEL_BITS))
 
#define ncore_os_ready(thread)              (void)thread

#define ncore_os_block(thread)              (void)thread

#define ncore_os_should_exit()

#define ncore_os_exit()

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief General purpose registers are 32bit wide.
 */
typedef unsigned int ncore_reg;

typedef unsigned int ncpu_size;

typedef   signed int ncpu_ssize;

struct PORT_C_ALIGN(NCPU_DATA_ALIGNMENT) ncore_ref
{
	uint32_t					value;
};

/**@brief       Interrupt context structure
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
struct ncore_lock
{
    unsigned int                level;
};

struct PORT_C_ALIGN(NCPU_DATA_ALIGNMENT) ncore_atomic
{
	int32_t						value;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE_ALWAYS
uint_fast8_t ncore_log2(
    ncore_reg                    value)
{
    uint_fast8_t                clz;

    __asm__ __volatile__ (
        "@  ncore_log2                                      \n"
        "   clz    %0, %1                                   \n"
        : "=r"(clz)
        : "r"(value));

    return (31u - clz);
}



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
uint32_t ncore_ref_read(
	struct ncore_ref *          ref)
{
    return (ref->value);
}



PORT_C_INLINE_ALWAYS
void ncore_ref_increment(
	struct ncore_ref *      	ref)
{
    if (ref->value != UINT32_MAX) {
        ref->value++;
    }
}



PORT_C_INLINE_ALWAYS
void ncore_ref_decrement(
	struct ncore_ref *          ref)
{
    if (ref->value != 0u) {
        ref->value--;
    }
}



/*
 * TODO: Write this thread safe and in assembler.
 */
PORT_C_INLINE_ALWAYS
void ncore_atomic_write(struct ncore_atomic * v, int32_t i)
{
	v->value = i;
}



/*
 * TODO: Write this thread safe and in assembler.
 */
PORT_C_INLINE_ALWAYS
int32_t ncore_atomic_read(
	struct ncore_atomic *       ref)
{
    return (ref->value);
}



/*
 * TODO: Write this thread safe and in assembler.
 */
PORT_C_INLINE_ALWAYS
void ncore_atomic_inc(
	struct ncore_atomic *      	ref)
{
    if (ref->value != INT32_MAX) {
    	ref->value++;
    }
}



/*
 * TODO: Write this thread safe and in assembler.
 */
PORT_C_INLINE_ALWAYS
void ncore_atomic_dec(
	struct ncore_atomic *        ref)
{
    if (ref->value != 0u) {
    	ref->value--;
    }
}



PORT_C_INLINE
void ncore_lock_enter(
    struct ncore_lock *          lock)
{
#if (CONFIG_CORE_LOCK_MAX_LEVEL != 255)
    unsigned int                new_mask;

    new_mask = NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL) << NCORE_LOCK_LEVEL_BITS;

    __asm __volatile__ (
        "@  ncore_lock_enter                                \n"
        "   mrs     %0, basepri                             \n"
        "   msr     basepri, %1                             \n"
        : "=&r"(lock->level)
        : "r"(new_mask));
#else
    unsigned int                new_mask;

    new_mask = 1;

    __asm __volatile__ (
        "@  ncore_lock_enter                                \n"
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
        "@  ncore_lock_exit                                 \n"
        "   msr    basepri, %0                              \n"
        :
        : "r"(lock->level));
#else
    __asm __volatile__ (
        "@  ncore_lock_exit                                 \n"
        "   msr    primask, %0                              \n"
        :
        : "r"(lock->level));
#endif
}



void ncore_deferred_init(void);



PORT_C_INLINE
void ncore_deferred_do(void)
{
	/* Trigger PendSV
	 */
	*((uint32_t volatile *)0xE000ED04) = 0x10000000;
}



extern void ncore_deferred_work(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_core.h
 ******************************************************************************/
#endif /* NEON_ARCH_P_CORE_H_ */
