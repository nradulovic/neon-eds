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
 * @brief       Implementation of PIC32 interrupt port.
 * @addtogroup  pic32-none-gcc-intr_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "port/isr.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

volatile unsigned int * const g_iec_reg[] =
{
#if defined(IEC0)
    &IEC0,
#endif
#if defined(IEC1)
    &IEC1,
#endif
#if defined(IEC2)
    &IEC2,
#endif
#if defined(IEC3)
    &IEC3,
#endif
#if defined(IEC4)
    &IEC4,
#endif
#if defined(IEC5)
    &IEC5,
#endif
#if defined(IEC6)
    &IEC6,
#endif
#if defined(IEC7)
    &IEC7,
#endif
#if defined(IEC8)
    &IEC8,
#endif
};

volatile unsigned int * const g_ifs_reg[] =
{
#if defined(IFS0)
    &IFS0,
#endif
#if defined(IFS1)
    &IFS1,
#endif
#if defined(IFS2)
    &IFS2,
#endif
#if defined(IFS3)
    &IFS3,
#endif
#if defined(IFS4)
    &IFS4,
#endif
#if defined(IFS5)
    &IFS5,
#endif
#if defined(IFS6)
    &IFS6,
#endif
#if defined(IFS7)
    &IFS7,
#endif
#if defined(IFS8)
    &IFS8,
#endif
};

volatile unsigned int * const g_ipc_reg[] =
{
#if defined(IPC0)
    &IPC0,
#endif
#if defined(IPC1)
    &IPC1,
#endif
#if defined(IPC2)
    &IPC2,
#endif
#if defined(IPC3)
    &IPC3,
#endif
#if defined(IPC4)
    &IPC4,
#endif
#if defined(IPC5)
    &IPC5,
#endif
#if defined(IPC6)
    &IPC6,
#endif
#if defined(IPC7)
    &IPC7,
#endif
#if defined(IPC8)
    &IPC8,
#endif
#if defined(IPC9)
    &IPC9,
#endif
#if defined(IPC10)
    &IPC10,
#endif
#if defined(IPC11)
    &IPC11,
#endif
#if defined(IPC12)
    &IPC12,
#endif
#if defined(IPC13)
    &IPC13,
#endif
#if defined(IPC14)
    &IPC14,
#endif
#if defined(IPC15)
    &IPC15,
#endif
#if defined(IPC16)
    &IPC16,
#endif
#if defined(IPC17)
    &IPC17,
#endif
#if defined(IPC18)
    &IPC18,
#endif
#if defined(IPC19)
    &IPC19,
#endif
#if defined(IPC20)
    &IPC20,
#endif
#if defined(IPC21)
    &IPC21,
#endif
#if defined(IPC22)
    &IPC22,
#endif
#if defined(IPC23)
    &IPC23,
#endif
#if defined(IPC24)
    &IPC24,
#endif
#if defined(IPC25)
    &IPC25,
#endif
#if defined(IPC26)
    &IPC26,
#endif
#if defined(IPC27)
    &IPC27,
#endif
#if defined(IPC28)
    &IPC28,
#endif
#if defined(IPC29)
    &IPC29,
#endif
#if defined(IPC30)
    &IPC30,
#endif
#if defined(IPC31)
    &IPC31,
#endif
#if defined(IPC32)
    &IPC32,
#endif
#if defined(IPC33)
    &IPC33,
#endif
#if defined(IPC34)
    &IPC34,
#endif
#if defined(IPC35)
    &IPC35,
#endif
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void nisr_module_init(void) 
{
    nisr_global_disable();
    INTCON1bits.NSTDIS = 0;                   /* Interrupt nesting is enabled */
}



void nisr_module_term(void) 
{
    nisr_global_disable();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of isr.c
 ******************************************************************************/
