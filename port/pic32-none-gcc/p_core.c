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
 * @brief       STM32F4xx port core source
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include <sys/attribs.h>

#include "port/core.h"

/*=========================================================  LOCAL MACRO's  ==*/

#if (CONFIG_CORE_LOCK_MAX_LEVEL < 32)
#define CORE_TIMER_IPL                  IPL0SOFT
#elif (CONFIG_CORE_LOCK_MAX_LEVEL < 64)
#define CORE_TIMER_IPL                  IPL1SOFT
#elif (CONFIG_CORE_LOCK_MAX_LEVEL < 96)
#define CORE_TIMER_IPL                  IPL2SOFT
#elif (CONFIG_CORE_LOCK_MAX_LEVEL < 128)
#define CORE_TIMER_IPL                  IPL3SOFT
#elif (CONFIG_CORE_LOCK_MAX_LEVEL < 160)
#define CORE_TIMER_IPL                  IPL4SOFT
#elif (CONFIG_CORE_LOCK_MAX_LEVEL < 192)
#define CORE_TIMER_IPL                  IPL5SOFT
#elif (CONFIG_CORE_LOCK_MAX_LEVEL < 224)
#define CORE_TIMER_IPL                  IPL6SOFT
#else
#define CORE_TIMER_IPL                  IPL7SOFT
#endif

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE
void cpu_init(void);



static void lock_init(void);



static void lock_term(void);



static void timer_init(void);



static void timer_term(void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


PORT_C_INLINE
void cpu_init(void)
{

}



PORT_C_INLINE
void cpu_sleep(void)
{
    __asm__ __volatile__(
        "@  cpu_sleep                                       \n"
        "   wfe                                             \n");
}



PORT_C_INLINE
void cpu_term(void)
{
    for (;;) {
    /*
     * NOTE: Turn off WaitForEvent when in debug mode. Some debuggers have 
     * trouble working when WFE is enabled.
     */
#if !defined(NDEBUG) && (CONFIG_DEBUG == 0)
        cpu_sleep();
#endif
    }
}



static void lock_init(void)
{
    ncore_reg                   cause;
    ncore_reg                   status;

    __asm __volatile__(
        "   di                                          \n"
        :
        : "memory"
        :);  
    
    /* Use vectored interrupt table */
    cause   = _CP0_GET_CAUSE();
    cause  |= _CP0_CAUSE_IV_MASK;
    _CP0_SET_CAUSE(cause);
    status  = _CP0_GET_STATUS();
    status &= ~_CP0_STATUS_BEV_MASK;
    _CP0_SET_STATUS(status);
    INTCONSET = _INTCON_MVEC_MASK;
    
    __asm __volatile__(
        "   ei                                          \n"
        :
        : "memory"
        :); 
}



static void lock_term(void)
{
    __asm __volatile__(
        "   di                                          \n"
        :
        : "memory"
        :); 
}



static void timer_init(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    ncore_reg                   cause;

    /* Disable timer */
    cause  = _CP0_GET_CAUSE();
    cause |= _CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
    
    /* Disable ISR */
    IEC0CLR = _IEC0_CTIE_MASK;

    /* Set ISR priority */
    IPC0CLR = 0x7u << IPC0_CTIP_SHIFT;
    IPC0SET = (NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL) << IPC0_CTIP_SHIFT) & IPC0_MASK;
#endif
}



static void timer_term(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    ncore_reg                   cause;
    
    /* Disable ISR */
    IEC0CLR = _IEC0_CTIE_MASK;
    
    /* Disable timer */
    cause  = _CP0_GET_CAUSE();
    cause |= _CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
#endif
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void ncore_init(void)
{
    cpu_init();
    lock_init();
    timer_init();
}



void ncore_term(void)
{
    timer_term();
    lock_term();
    cpu_term();
}



void ncore_idle(void)
{
    /* NOTE:
     * When in debug mode do not use sleep instruction.
     */
#if !defined(NDEBUG) && (CONFIG_DEBUG == 0)
    cpu_sleep();
#endif
}



void ncore_timer_enable(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    ncore_reg                   cause;
    
    /* Set and enable timer */
    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE((CONFIG_SYSTIMER_CLOCK_FREQ / CONFIG_SYSTIMER_EVENT_FREQ));
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
    
    /* Enable ISR */
    IFS0CLR = _IFS0_CTIF_MASK;
    IEC0SET = _IEC0_CTIE_MASK;
#endif
}



void ncore_timer_disable(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    ncore_reg                   cause;
    
    /* Disable ISR */
    IEC0CLR = _IEC0_CTIE_MASK;
    
    /* Disable timer */
    cause  = _CP0_GET_CAUSE();
    cause |= _CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
#endif
}




#if (CONFIG_CORE_TIMER_SOURCE == 0)
void __ISR(_CORE_TIMER_VECTOR, CORE_TIMER_IPL) core_timer_handler(void)
{
    ncore_reg                   compare;
    ncore_reg                   cause;

    /* Disable timer */
    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    
    /* Reload value */
    compare = _CP0_GET_COMPARE();
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(compare);
   
    /* Enable timer */
    _CP0_SET_CAUSE(cause);
    
    ncore_timer_isr();
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if ((CONFIG_CORE_TIMER_SOURCE != 0))
# error "NEON::base::port::pic32-none-xc: The selected timer in CONFIG_CORE_TIMER_SOURCE is not supported!"
#endif

/** @endcond *//** @} *//******************************************************
 * END of p_sys_lock.c
 ******************************************************************************/
