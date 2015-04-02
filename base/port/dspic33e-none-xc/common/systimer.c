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
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    IEC0bits.T2IE = 0;
    IEC0bits.T3IE = 0;
    IFS0bits.T2IF = 0;
    IFS0bits.T3IF = 0;
    IPC1bits.T2IP = CONFIG_ISR_MAX_PRIO;
    T2CON         = 0;
    T3CON         = 0;
    T2CONbits.T32 = 1;
    TMR2          = 0;
    PR2           = 0;
#endif
}



void nsystimer_module_term(void) 
{
    nsystimer_disable();
    nsystimer_isr_disable();
}



#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    nsystimer_isr();                                 /* Call the user handler */
    IFS0bits.T2IF;
}
#endif
    

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of cpu.c
 ******************************************************************************/

