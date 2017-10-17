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
#include <stdbool.h>

#include "port/compiler.h"
#include "base/config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Specifies bit-width of general purpose registers
 */
#define NCPU_DATA_WIDTH                     32u

/**@brief       Specifies data alignment for optimal performance
 */
#define NCPU_DATA_ALIGNMENT                 4u

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

#define ncore_os_should_exit()              false

#define ncore_os_exit()

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Interrupt context structure
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
struct ncore_lock
{
    unsigned int                level;
};

/**@brief       Atomic structure
 * @api
 */
struct PORT_C_ALIGN(NCPU_DATA_ALIGNMENT) ncore_atomic
{
    int32_t                     value;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE_ALWAYS
uint_fast8_t ncore_log2(uint32_t value)
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
uint32_t ncore_exp2(uint_fast8_t value)
{
    return (0x1u << value);
}



PORT_C_INLINE
void ncore_lock_enter(struct ncore_lock * lock)
{
    extern bool                 g_core_is_locked;
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
    g_core_is_locked = true;
}



PORT_C_INLINE
void ncore_lock_exit(struct ncore_lock * lock)
{
    extern bool                 g_core_is_locked;

    g_core_is_locked = false;
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



PORT_C_INLINE
bool ncore_is_lock_valid(void)
{
    extern bool                 g_core_is_locked;
    bool                        is_isr;

    is_isr = !!(*((uint32_t volatile *)0xe000ed04u) & 0x1ffu);

    return (!g_core_is_locked != !is_isr);
}



PORT_C_INLINE_ALWAYS
void ncore_atomic_inc(struct ncore_atomic * ref)
{
	struct ncore_lock			lock;

	ncore_lock_enter(&lock);
	ref->value++;
    ncore_lock_exit(&lock);
}



PORT_C_INLINE_ALWAYS
void ncore_atomic_dec(struct ncore_atomic * ref)
{
	struct ncore_lock			lock;

	ncore_lock_enter(&lock);
	ref->value--;
    ncore_lock_exit(&lock);
}



void ncore_deferred_init(void);



PORT_C_INLINE
void ncore_deferred_do(void)
{
    /* Interrupt Control and State Register & PENDSVSET */
    *((uint32_t volatile *)0xe000ed04u) = 0x10000000u;
}



extern void ncore_deferred_work(void);



PORT_C_INLINE
uint8_t ncore_exu4(uint32_t data)
{
    uint32_t        retval;

    __asm __volatile__(
        "@ ncore_exu4                                       \n"
        "    ubfx   %0, %1, #24, #8                         \n"
        : "=r"(retval)
        : "r"(data));

    return ((uint8_t)retval);
}



PORT_C_INLINE
uint8_t ncore_exu3(uint32_t data)
{
    uint32_t        retval;

    __asm __volatile__(
        "@ ncore_exu3                                       \n"
        "    ubfx   %0, %1, #16, #8                         \n"
        : "=r"(retval)
        : "r"(data));

    return ((uint8_t)retval);
}



PORT_C_INLINE uint8_t
ncore_exu2(uint32_t data)
{
    uint32_t                    retval;

    __asm __volatile__(
        "@ ncore_exu2                                       \n"
        "    ubfx   %0, %1, #8, #8                          \n"
        : "=r"(retval)
        : "r"(data));

    return ((uint8_t)retval);
}



PORT_C_INLINE uint8_t
ncore_exu1(uint32_t data)
{
    return ((uint8_t)data);
}



PORT_C_INLINE int32_t
ncore_ext_i24(int32_t data)
{
    int32_t                     retval;

    __asm __volatile__(
        "@ ncore_ext_i24                                    \n"
        "   sbfx    %0, %1, #0, #24                         \n"
        : "=r"(retval)
        : "r"(data));

    return (retval);
}

void ncore_dummy_rd(volatile uint32_t value);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_core.h
 ******************************************************************************/
#endif /* NEON_ARCH_P_CORE_H_ */
