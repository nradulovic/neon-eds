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
 * @author  	Nenad Radulovic
 * @brief   	Family profile for STM32F10x
 * @addtogroup  port_family_stm32f10x
 * @brief       Family profile for STM32F10x
 *********************************************************************//** @{ */

#ifndef ES_PROFILE_H_
#define ES_PROFILE_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        ST-Microelectronics STM32F10x
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Port constant: interrupt priority bits implemented in MCU
 * @note        It is also recommended to ensure that all priority bits are
 *              assigned as being preemption priority bits, and none as sub
 *              priority bits
 */
#define CPU_DEF_ISR_PRIO_BITS           4u

/**@brief       System timer maximum value
 * @details     STM32F10x family has 24-bit wide system tick register
 */
#define PORT_DEF_SYSTMR_MAX_VAL         0xfffffful

/**@} *//*--------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of profile.h
 ******************************************************************************/
#endif /* ES_PROFILE_H_ */
