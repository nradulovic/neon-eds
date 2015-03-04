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

#include "port/sys_lock.h"
#include "port/systimer.h"
#include "family/profile.h"
#include "cortex_m3.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/**@brief       Initialize and start the system timer
 */
void nsystimer_init(
    nsystimer_tick              val)
{
    PORT_SYST_CSR &= ~PORT_SYST_CSR_ENABLE;     /* Disable timer */
    PORT_SYST_RVR  = val;
    PORT_SYST_CVR  = 0u;
    PORT_SYST_CSR |= PORT_SYST_CSR_ENABLE;
}



/**@brief       Stop and terminate the system timer
 */
void nsystimer_term(void)
{
    PORT_SYST_CSR &= ~PORT_SYST_CSR_ENABLE;
}



/**@brief       Get free counter value
 */
nsystimer_tick nsystimer_get_current(void)
{
    return (PORT_SYST_CVR);
}



/**@brief       Get reload counter value
 */
nsystimer_tick nsystimer_get_reload(void)
{
    return (PORT_SYST_RVR);
}



/**@brief       Load the system timer Reload value register
 */
void nsystimer_load(
    nsystimer_tick              val)
{
    PORT_SYST_CSR &= ~PORT_SYST_CSR_ENABLE;
    PORT_SYST_RVR  = val;
    PORT_SYST_CVR  = 0u;
    PORT_SYST_CSR |= PORT_SYST_CSR_ENABLE;
}



/**@brief       Enable the system timer
 */
void nsystimer_enable(void)
{
    PORT_SYST_CSR |= PORT_SYST_CSR_ENABLE;
}



/**@brief       Disable the system timer
 */
void nsystimer_disable(void)
{
    PORT_SYST_CSR &= ~PORT_SYST_CSR_ENABLE;
}



/**@brief       Disable the system timer interrupt
 */
void nsystimer_isr_enable(void)
{
    PORT_SYST_CSR |= PORT_SYST_CSR_TICKINT;
}



/**@brief       Enable the system timer interrupt
 */
void nsystimer_isr_disable(void)
{
    PORT_SYST_CSR &= ~PORT_SYST_CSR_TICKINT;
    PORT_SCB_ICSR |= PORT_SCB_ICSR_PENDSTCLR;
}

void nsystimer_module_init(void)
{
    PORT_SYST_CSR &= ~PORT_SYST_CSR_ENABLE;
    PORT_SYST_CSR &= ~PORT_SYST_CSR_TICKINT;
    PORT_SYST_CSR |=  PORT_SYST_CSR_CLKSOURCE;
    PORT_SCB_ICSR |=  PORT_SCB_ICSR_PENDSTCLR;
    PORT_SCB_SHP_SYSTICK = (uint8_t)NSYS_LOCK_LEVEL_TO_CODE(CONFIG_SYS_LOCK_MAX_LEVEL);
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
