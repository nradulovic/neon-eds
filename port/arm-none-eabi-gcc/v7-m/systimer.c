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
 * @author      Nenad Radulovic
 * @brief       Implementation of ARM Cortex-M3 interrupt port.
 * @addtogroup  arm-none-eabi-gcc-v7-m_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "family/profile.h"
#include "arch/systimer.h"
#include "arch/isr.h"

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
	PORT_SYST_CSR &= ~PORT_SYST_CSR_ENABLE;
	PORT_SYST_CSR &= ~PORT_SYST_CSR_TICKINT;
	PORT_SYST_CSR |=  PORT_SYST_CSR_CLKSOURCE;
	PORT_SCB_ICSR |=  PORT_SCB_ICSR_PENDSTCLR;
	PORT_SCB_SHP_SYSTICK = (uint8_t)NISR_PRIO_TO_CODE(CONFIG_ISR_MAX_PRIO);
}

#if defined(PORT_SYSTIMER_HANDLER)
void PORT_SYSTIMER_HANDLER(void)
{
	nsystimer_isr();
    /*
     * TODO: Clear interrupt flag
     */
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of nport.c
 ******************************************************************************/
