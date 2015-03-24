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

/*======================================================  LOCAL DATA TYPES  ==*/

struct syst
{
    volatile unsigned int       csr;
    volatile unsigned int       rvr;
    volatile unsigned int       cvr;
    volatile unsigned int       calib;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/**@brief       Initialise and start the system timer
 */
void nsystimer_init(
    nsystimer_tick              val)
{
    SYST->csr &= ~SYST_CSR_ENABLE;                          /* Disable timer  */
    SYST->rvr  = val;
    SYST->cvr  = 0u;
    SYST->csr |=  SYST_CSR_ENABLE;
}



/**@brief       Stop and terminate the system timer
 */
void nsystimer_term(void)
{
    SYST->csr &= ~SYST_CSR_ENABLE;
}



/**@brief       Get free counter value
 */
nsystimer_tick nsystimer_get_current(void)
{
    return (SYST->cvr);
}



/**@brief       Get reload counter value
 */
nsystimer_tick nsystimer_get_reload(void)
{
    return (SYST->rvr);
}



/**@brief       Load the system timer Reload value register
 */
void nsystimer_load(
    nsystimer_tick              val)
{
    SYST->csr &= ~SYST_CSR_ENABLE;                          /* Disable timer  */
    SYST->rvr  = val;
    SYST->cvr  = 0u;
    SYST->csr |=  SYST_CSR_ENABLE;
}



/**@brief       Enable the system timer
 */
void nsystimer_enable(void)
{
    SYST->csr |= SYST_CSR_ENABLE;
}



/**@brief       Disable the system timer
 */
void nsystimer_disable(void)
{
    SYST->csr &= ~SYST_CSR_ENABLE;
}



/**@brief       Disable the system timer interrupt
 */
void nsystimer_isr_enable(void)
{
    SYST->csr |= SYST_CSR_TICKINT;
}



/**@brief       Enable the system timer interrupt
 */
void nsystimer_isr_disable(void)
{
    SYST->csr &= ~SYST_CSR_TICKINT;
    SCB_ICSR |= SCB_ICSR_PENDSTCLR;
}

void nsystimer_module_init(void)
{
    SYST->csr &= ~SYST_CSR_ENABLE;
    SYST->csr &= ~SYST_CSR_TICKINT;
    SYST->csr |=  SYST_CSR_CLKSOURCE;
    SCB_ICSR  |=  SCB_ICSR_PENDSTCLR;
    SCB_SHP_SYSTICK = (uint8_t)NSYS_LOCK_LEVEL_TO_CODE(CONFIG_SYS_LOCK_MAX_LEVEL);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of nport.c
 ******************************************************************************/
