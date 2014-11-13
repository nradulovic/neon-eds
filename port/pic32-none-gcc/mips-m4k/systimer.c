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
 * @author      Nenad Radulovic
 * @brief       Implementation of PIC32 System Timer port.
 * @addtogroup  pic32-none-gcc-systimer
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-systimer_impl System Timer module Implementation
 * @brief       System Timer module Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>
#include <sys/attribs.h>

#include "arch/systimer.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void nsystimer_module_init(void) 
{
    nsystimer_disable();
    nsystimer_isr_disable();
    IPC0CLR = 0x7u << IPC0_CTIP_SHIFT;
    IPC0SET = (CONFIG_ISR_MAX_PRIO << IPC0_CTIP_SHIFT) & IPC0_MASK;
}



void nsystimer_module_term(void) 
{
    nsystimer_disable();
    nsystimer_isr_disable();
}



void __ISR(_CORE_TIMER_VECTOR, IPL7AUTO) systimer_handler(void)
{
    ncpu_reg                    compare;

    nsystimer_disable();                                     /* Restart timer */
    compare = _CP0_GET_COMPARE();
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(compare);
    nsystimer_enable();
#if 0
    nsystimer_isr();                                 /* Call the user handler */
#endif
    IFS0CLR = IFS0_CT_BIT;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of cpu.c
 ******************************************************************************/

