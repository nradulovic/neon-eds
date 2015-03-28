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
 * @brief       System lock
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "base/port/core.h"

/*=========================================================  LOCAL MACRO's  ==*/

/*--  SysTick Control / Status Register Definitions  -------------------------*/
#define SYST                            ((volatile struct syst *)0xE000E010)
#define SYST_CSR_CLKSOURCE              (1ul << 2)
#define SYST_CSR_TICKINT                (1ul << 1)
#define SYST_CSR_ENABLE                 (1ul << 0)

#define SCB_ICSR                        (*(volatile unsigned int *)0xE000ED04)
#define SCB_ICSR_PENDSTSET              (1ul << 26)
#define SCB_ICSR_PENDSTCLR              (1ul << 25)
#define SCB_SHP_SYSTICK                 (*(volatile unsigned char *)0xE000ED23)

#define PORT_CONFIG_ISR_SUBPRIORITY     0

/*--  On AIRCR register writes, write 0x5FA, otherwise the write is ignored  -*/

#define SCB_AIRCR                       (*(volatile unsigned int *)0xE000ED0C)
#define SCB_AIRCR_VECTKEY_Pos           16
#define SCB_AIRCR_VECTKEY               (0xfffful << 16)
#define SCB_AIRCR_VECTKEY_VALUE         (0x5faul << 16)
#define SCB_AIRCR_PRIGROUP_Pos          8
#define SCB_AIRCR_PRIGROUP              (0x7ul << 8)

/*======================================================  LOCAL DATA TYPES  ==*/

struct syst
{
    volatile unsigned int       csr;
    volatile unsigned int       rvr;
    volatile unsigned int       cvr;
    volatile unsigned int       calib;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE
void cpu_init(void);



/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void isr_global_enable(void);



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void isr_global_disable(void);



static void sys_lock_init(void);



static void sys_lock_term(void);



static void systimer_init(void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


PORT_C_INLINE
void cpu_init(void)
{
    __asm__ __volatile__(
        "@  ncpu_init                                       \n"
        "   clrex                                           \n");
                                              /* Clear the exclusive monitor. */
}



PORT_C_INLINE
void cpu_term(void)
{
    for (;;) {
        __asm__ __volatile__(
            "   wfe                                         \n");
    }
}



PORT_C_INLINE
void isr_global_enable(void)
{
    __asm __volatile__ (
        "@  nisr_enable                                     \n"
        "   cpsie   i                                       \n");
}



PORT_C_INLINE
void isr_global_disable(void)
{
    __asm __volatile__ (
        "@  nisr_disable                                    \n"
        "   cpsid   i                                       \n");
}



static void sys_lock_init(void)
{
    unsigned int                reg;

    isr_global_disable();
    reg  = SCB_AIRCR;
    reg &= ~(SCB_AIRCR_VECTKEY | SCB_AIRCR_PRIGROUP);
    reg |=   SCB_AIRCR_VECTKEY_VALUE;
    reg |=  (PORT_CONFIG_ISR_SUBPRIORITY << SCB_AIRCR_PRIGROUP_Pos);
    SCB_AIRCR = reg;
    isr_global_enable();
}



static void sys_lock_term(void)
{
    isr_global_disable();
}



static void systimer_init(void)
{
    SYST->csr &= ~SYST_CSR_ENABLE;
    SYST->csr &= ~SYST_CSR_TICKINT;
    SYST->csr |=  SYST_CSR_CLKSOURCE;
    SCB_ICSR  |=  SCB_ICSR_PENDSTCLR;
    SCB_SHP_SYSTICK = (uint8_t)NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL);
}



/**@brief       Stop and terminate the system timer
 */
static void systimer_term(void)
{
    SYST->csr &= ~SYST_CSR_ENABLE;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/**@brief       Initialise and start the system timer
 */
void ncore_timer_init(
    ncore_time_tick              val)
{
    SYST->csr &= ~SYST_CSR_ENABLE;                          /* Disable timer  */
    SYST->rvr  = val;
    SYST->cvr  = 0u;
    SYST->csr |=  SYST_CSR_ENABLE;
}



/**@brief       Get free counter value
 */
ncore_time_tick ncore_timer_get_current(void)
{
    return (SYST->cvr);
}



/**@brief       Get reload counter value
 */
ncore_time_tick ncore_timer_get_reload(void)
{
    return (SYST->rvr);
}



/**@brief       Load the system timer Reload value register
 */
void ncore_timer_load(
    ncore_time_tick              val)
{
    SYST->csr &= ~SYST_CSR_ENABLE;                          /* Disable timer  */
    SYST->rvr  = val;
    SYST->cvr  = 0u;
    SYST->csr |=  SYST_CSR_ENABLE;
}



/**@brief       Enable the system timer
 */
void ncore_timer_enable(void)
{
    SYST->csr |= SYST_CSR_ENABLE;
}



/**@brief       Disable the system timer
 */
void ncore_timer_disable(void)
{
    SYST->csr &= ~SYST_CSR_ENABLE;
}



/**@brief       Disable the system timer interrupt
 */
void ncore_timer_isr_enable(void)
{
    SYST->csr |= SYST_CSR_TICKINT;
}



/**@brief       Enable the system timer interrupt
 */
void ncore_timer_isr_disable(void)
{
    SYST->csr &= ~SYST_CSR_TICKINT;
    SCB_ICSR |= SCB_ICSR_PENDSTCLR;
}




void ncore_init(void)
{
    cpu_init();
    sys_lock_init();
    systimer_init();
}



void ncore_term(void)
{
    systimer_term();
    sys_lock_term();
    cpu_term();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_sys_lock.c
 ******************************************************************************/
