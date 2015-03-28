/*
 * This file is part of neon-test.
 *
 * Copyright (C) 2010 - 2015 nenad
 *
 * neon-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * neon-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with neon-test.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    
 * e-mail  :    
 *//***********************************************************************//**
 * @file
 * @author      nenad
 * @brief       Brief description
 *********************************************************************//** @{ */
/**@defgroup    def_impl Implementation
 * @brief       Default Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/port/peripheral.h"
#include "base/port/profile.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void np_clock_enable(const struct np_dev_clock * clock)
{
    *(volatile uint32_t *)clock->reg |= clock->mask;
}

void np_clock_disable(const struct np_dev_clock * clock)
{
    *(volatile uint32_t *)clock->reg &= ~clock->mask;
}

void np_isr_enable(const struct np_dev_isr * isr)
{
    NVIC_EnableIRQ(isr->irqn);
}

void np_isr_disable(const struct np_dev_isr * isr)
{
    NVIC_DisableIRQ(isr->irqn);
}

void np_isr_clear_flag(const struct np_dev_isr * isr)
{
    NVIC_ClearPendingIRQ(isr->irqn);
}

void np_isr_set_flag(const struct np_dev_isr * isr)
{
    NVIC_SetPendingIRQ(isr->irqn);
}

void np_isr_set_prio(const struct np_dev_isr * isr, uint32_t prio)
{
    NVIC_SetPriority(isr->irqn, prio);
}

uint32_t np_isr_get_prio(const struct np_dev_isr * isr)
{
    return (NVIC_GetPriority(isr->irqn));
}

void np_mux_enable(const struct np_dev_mux * mux, uint32_t pin_id)
{
}

void np_mux_disable(const struct np_dev_mux * mux, uint32_t pin_id)
{
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of peripheral_device.c
 ******************************************************************************/
