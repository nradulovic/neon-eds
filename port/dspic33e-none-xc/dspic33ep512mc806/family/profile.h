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
 * @brief   	Profile for Microchip PIC32MX150F128
 * @addtogroup  pic32-none-gcc
 * @brief       Profile for Microchip PIC32MX150F128
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-pic32mx150f128 Microchip PIC32MX150F128
 * @brief       Profile for Microchip PIC32MX150F128
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NPROFILE_H_
#define NPROFILE_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

/**@brief       Specifies maximum CPU clock speed in Hz.
 */
#define NPROFILE_MAX_CPU_CLOCK          80000000ul

/**@brief       System timer maximum value
 * @details     STM32F10x family has 24-bit wide system tick register
 */
#define NPROFILE_MAX_SYSTIMER_VAL       0xfffffffful

/**@brief       Maximum RAM get_size for this family
 * @details     This define is used to choose optimal algorithm for this family
 *              of micro-controllers.
 */
#define NPROFILE_RAM_SIZE               65536u

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
/** @endcond *//** @} *//** @} *//*********************************************
 * END of profile.h
 ******************************************************************************/
#endif /* ES_PROFILE_H_ */
