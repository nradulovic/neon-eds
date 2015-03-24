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

#include "port/sys_lock.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define PORT_CONFIG_ISR_SUBPRIORITY     0

/*--  On AIRCR register writes, write 0x5FA, otherwise the write is ignored  -*/

#define SCB_AIRCR                       (*(volatile unsigned int *)0xE000ED0C)
#define SCB_AIRCR_VECTKEY_Pos           16
#define SCB_AIRCR_VECTKEY               (0xfffful << 16)
#define SCB_AIRCR_VECTKEY_VALUE         (0x5faul << 16)
#define SCB_AIRCR_PRIGROUP_Pos          8
#define SCB_AIRCR_PRIGROUP              (0x7ul << 8)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void isr_global_enable(void);



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void isr_global_disable(void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

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

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void nsys_lock_module_init(void)
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



void nsys_lock_module_term(void)
{
    isr_global_disable();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_sys_lock.c
 ******************************************************************************/
