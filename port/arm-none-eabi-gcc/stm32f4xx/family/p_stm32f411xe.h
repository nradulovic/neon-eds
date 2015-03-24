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
 * @brief       Port for stm32f411xe series
 *********************************************************************//** @{ */

#ifndef PORT_FAMILY_P_STM32F411XE_H_
#define PORT_FAMILY_P_STM32F411XE_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

#define NPROFILE_MAX_CPU_CLOCK          (100ul * 1000000ul)
#define NPROFILE_MAX_SYSTIMER_VAL       0xfffffful
#define NPROFILE_RAM_SIZE               (128ul * 1024ul)

#define NPROFILE_AVAILABLE_GPIO                                                 \
    (NP_EN_MAJOR(1) | NP_EN_MAJOR(3))

#define NPROFILE_AVAILABLE_UART                                                 \
    (NP_EN_MAJOR(1) | NP_EN_MAJOR(2) | NP_EN_MAJOR(6))

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
 * END of p_stm32f411xe.h
 ******************************************************************************/
#endif /* PORT_FAMILY_P_STM32F411XE_H_ */
