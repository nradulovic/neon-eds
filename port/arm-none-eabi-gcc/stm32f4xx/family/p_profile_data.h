/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Port profile data
 *********************************************************************//** @{ */

#ifndef NEON_FAMILY_P_PROFILE_DATA_H_
#define NEON_FAMILY_P_PROFILE_DATA_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

#if defined(STM32F411xE)
# include "family/p_stm32f411xe.h"
#endif

/**@brief       Specify the number of bits used in ISR priority mask. For now
 *              all STM32F4 series MCU's use 4 bits
 */
#if !defined(PORT_ISR_PRIO_BITS)
#define PORT_ISR_PRIO_BITS              4u
#endif

/**@brief       This is defined to ONE if the port supports Cortex-M bit-banding
 */
#if !defined(PORT_BIT_BAND)
#define PORT_BIT_BAND                   1u
#endif

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of p_profile_data.h
 ******************************************************************************/
#endif /* NEON_FAMILY_P_PROFILE_DATA_H_ */
