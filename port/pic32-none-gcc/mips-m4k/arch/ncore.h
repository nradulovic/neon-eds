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
/**@defgroup    arm-none-eabi-gcc-v7-m-core ARM Cortex M3/M4 Core module
 * @brief       Port core module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NPORT_CORE_H
#define NPORT_CORE_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>
#include <peripheral/int.h>

#include "plat/compiler.h"
#include "family/profile.h"
#include "arch/ncore_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        CPU management macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Specifies bit-width of general purpose registers
 */
#define NCPU_DATA_WIDTH                     32u

/**@brief       Specifies data alignment for optimal performance
 */
#define NCPU_DATA_ALIGNMENT                 4u

#define NCPU_REG_MAX                        UINT32_MAX

#define NCPU_SIZE_MAX                       UINT32_MAX

#define NCPU_SSIZE_MAX                      INT32_MAX

#define NCORE_TIMER_MAX                     UINT32_MAX

/**@} *//*----------------------------------------------------------------*//**
 * @name        Interrupt service management macros
 * @{ *//*--------------------------------------------------------------------*/

#define NISR_PRIO_TO_CODE(prio)                                                 \
    (((prio) << _CP0_STATUS_IPL_POSITION) & _CP0_STATUS_IPL_MASK)

#define NISR_CODE_TO_PRIO(code)                                                 \
    (((code) & _CP0_STATUS_IPL_MASK) >> _CP0_STATUS_IPL_POSITION)

#define nisr_exit()                         (void)0

/**@} *//*----------------------------------------------------------------*//**
 * @name        Core timer macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Core timer one tick value
 */
#define NCORE_TIMER_ONE_TICK                                                    \
    (CONFIG_CORE_TIMER_CLOCK_FREQ / CONFIG_CORE_TIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the core timer
 */
#define NCORE_TIMER_MAX_TICKS                                                   \
    (NPROFILE_MAX_CORE_TIMER_VAL / NCORE_TIMER_ONE_TICK)

/**@} *//*----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief General purpose registers are 32bit wide.
 */
typedef unsigned int ncpu_reg;

typedef unsigned int ncpu_size;

typedef signed   int ncpu_ssize;

/**@brief       Interrupt context type
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
typedef unsigned int nisr_ctx;

/**@brief       Core timer hardware register type.
 */
typedef unsigned int ncore_timer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/

extern bool g_isr_is_active;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        CPU arithmetic/logic operations
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Stop the further CPU execution
 */
PORT_C_INLINE
void ncpu_stop(void)
{
    while (true) {
        /*
         * TODO: Put the CPU to sleep
         */
    }
}



/**@brief       Computes integer logarithm base 2
 */
PORT_C_INLINE_ALWAYS
uint_fast8_t ncpu_log2(
    ncpu_reg                    value)
{
    uint_fast8_t                clz;

    clz = __builtin_clz(value);

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

/**@} *//*----------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void __attribute__ ((nomips16)) nisr_enable(void)
{
    __asm __volatile__(
        "   ei                                              \n");
}



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void __attribute__ ((nomips16)) nisr_disable(void)
{
    __asm __volatile__(
        "   di                                              \n");
}



/**@brief       Set the new interrupt priority state
 * @param       new_mask
 *              New interrupt priority mask or new state of interrupts
 * @note        Depending on @ref CONFIG_ISR_MAX_PRIO setting this function will
 *              either set the new priority of allowed interrupts or just
 *              disable/enable all interrupts.
 */
PORT_C_INLINE
void __attribute__ ((nomips16)) nisr_set_mask(
    nisr_ctx                    new_mask)
{
#if (CONFIG_ISR_MAX_PRIO == 0)
    
    if (new_mask == 0u) {
        nisr_disable();
    } else {
        nisr_enable();
    }
#else
    ncpu_reg                    isr_status;
    
    isr_status  =  _CP0_GET_STATUS();
    isr_status &= ~_CP0_STATUS_IPL_MASK;
    isr_status |= new_mask & _CP0_STATUS_IPL_MASK;
    _CP0_SET_STATUS(isr_status);
#endif
}



/**@brief       Get old and set new interrupt priority mask
 * @return      Current interrupt priority mask
 */
PORT_C_INLINE
nisr_ctx __attribute__ ((nomips16)) nisr_replace_mask(
    nisr_ctx                    new_mask)
{
#if (CONFIG_ISR_MAX_PRIO != 0)
    nisr_ctx                    old_mask;
    
    if (new_mask == 0u) {
        __asm __volatile__(
            "   di      %0                                  \n"
            : "=r"(old_mask));
    } else {
        __asm __volatile__(
            "   ei      %0                                  \n"
            : "=r"(old_mask));
    }
    
    return (old_mask);
#else
    nisr_ctx                    old_mask;
    ncpu_reg                    isr_status;
    
    isr_status  = _CP0_GET_STATUS();
    old_mask    = isr_status;
    isr_status &= ~_CP0_STATUS_IPL_MASK;
    _CP0_SET_STATUS(isr_status | new_mask);
    
    return (old_mask);
#endif
}



PORT_C_INLINE
void nisr_enter(void)
{
    g_isr_is_active = true;
}



PORT_C_INLINE
void nisr_pend_kernel(void)
{
    /* TODO: Set the pending kernel ISR here
     */
}



PORT_C_INLINE
bool nisr_is_active(void)
{
    return (g_isr_is_active);
}

/**@} *//*----------------------------------------------------------------*//**
 * @name        Core timer management
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Initialize and start the system timer
 */
PORT_C_INLINE
void ncore_timer_init(
    ncore_timer_tick            tick)
{
    ncpu_reg                    cause;
    
    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(tick);
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
}



/**@brief       Stop and terminate the system timer
 */
PORT_C_INLINE
void ncore_timer_term(void)
{
    ncpu_reg                    cause;
    
    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
}



/**@brief       Get free counter value
 */
PORT_C_INLINE
ncore_timer_tick ncore_timer_get_current(void)
{
    return (_CP0_GET_COMPARE() - _CP0_GET_COUNT());
}



/**@brief       Get reload counter value
 */
PORT_C_INLINE
ncore_timer_tick ncore_timer_get_reload(void)
{
    return (_CP0_GET_COMPARE());
}



/**@brief       Load the system timer Reload value register
 */
PORT_C_INLINE
void ncore_timer_load(
    ncore_timer_tick            val)
{
    ncpu_reg                    cause;

    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(tick);
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
}



/**@brief       Enable the system timer
 */
PORT_C_INLINE
void ncore_timer_enable(void)
{
    ncpu_reg                    cause;

    cause  = _CP0_GET_CAUSE();
    cause &= ~_CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
}



/**@brief       Disable the system timer
 */
PORT_C_INLINE
void ncore_timer_disable(void)
{
    ncpu_reg                    cause;

    cause  = _CP0_GET_CAUSE();
    cause |= _CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
}



/**@brief       Disable the system timer interrupt
 */
PORT_C_INLINE
void ncore_timer_isr_enable(void)
{
    IFS0CLR = _IFS0_CTIF_MASK;
    IEC0SET = _IEC0_CTIE_MASK;
}




/**@brief       Enable the system timer interrupt
 */
PORT_C_INLINE
void ncore_timer_isr_disable(void)
{
    IEC0CLR = _IEC0_CTIE_MASK;
}



/**@brief       Register a handler function to core timer
 * @param       handler
 *              Handler callback function
 * @param       slot
 *              Occupy a predefined slot
 */
void ntimer_set_handler(
    void                     (* handler)(void),
    uint_fast8_t                slot);


/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port functions
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Initialize port
 */
void ncore_init(void);



/**@brief       Terminate port
 */
void ncore_term(void);



extern void ncore_timer_isr(void);



extern void ncore_kernel_isr(void);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of nport_core.h
 ******************************************************************************/
#endif /* NPORT_CORE_H */
