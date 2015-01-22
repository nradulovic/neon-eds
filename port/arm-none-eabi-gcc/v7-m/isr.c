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

#include "arch/isr.h"
#include "arch/cpu.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static void isr_set_priority_grouping(
    uint32_t                    grouping);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

bool g_isr_is_active = false;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


/**@brief       Set Priority Grouping
 * @param       grouping
 *              Priority grouping field.
 * @details     The function sets the priority grouping field using the required
 *              unlock sequence. The parameter grouping is assigned to the field
 *              SCB->AIRCR [10:8] PRIGROUP field. Only values from 0..7 are used.
 *              In case of a conflict between priority grouping and available
 *              priority bits (PORT_ISR_PRIO_BITS), the smallest possible
 *              priority group is set.
 */
static void isr_set_priority_grouping(
    uint32_t                    grouping)
{
    ncpu_reg                    reg;

    grouping &= 0x07u;
    reg  = PORT_SCB->AIRCR;
    reg &= ~(PORT_SCB_AIRCR_VECTKEY_Msk | PORT_SCB_AIRCR_PRIGROUP_Msk);
    reg |=  (PORT_SCB_AIRCR_VECTKEY_VALUE << PORT_SCB_AIRCR_VECTKEY_Pos);
    reg |=  (grouping << PORT_SCB_AIRCR_PRIGROUP_Pos);
    PORT_SCB->AIRCR = reg;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void nisr_module_init(void)
{
	nisr_global_disable();
	isr_set_priority_grouping(PORT_CONFIG_ISR_SUBPRIORITY);
												/* Setup priority subgroup.   */
}



void nisr_module_term(void)
{
	nisr_global_disable();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of isr.c
 ******************************************************************************/
