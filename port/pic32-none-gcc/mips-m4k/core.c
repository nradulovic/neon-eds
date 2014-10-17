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
 * @brief       Implementation of PIC32 CPU port.
 * @addtogroup  pic32-none-gcc-cpu_impl
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-cpu_impl CPU module Implementation
 * @brief       CPU module Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/ncore.h"
#include "family/profile.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void module_init_cpu(void);
static void module_term_cpu(void);
static void module_init_isr(void);
static void module_term_isr(void);
static void module_init_core_timer(void);
static void module_term_core_timer(void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static void module_init_cpu(void)
{

}



static void module_term_cpu(void)
{
    ncpu_stop();
}



static void module_init_isr(void)
{
    nisr_disable();
}



static void module_term_isr(void)
{
    nisr_disable();
}



static void module_init_core_timer(void)
{
    ncore_timer_disable();
    ncore_timer_isr_disable();
}

static void module_term_core_timer(void)
{

}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void __attribute__ ((nomips16)) nisr_enable(void)
{
    __asm __volatile__(
        "   ei                                              \n");
}



void __attribute__ ((nomips16)) nisr_disable(void)
{
    __asm __volatile__(
        "   di                                              \n");
}



void ncore_init(void)
{
    module_init_cpu();
    module_init_isr();
    module_init_core_timer();
}

void ncore_term(void)
{
    module_term_core_timer();
    module_term_isr();
    module_term_cpu();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of cpu.c
 ******************************************************************************/
