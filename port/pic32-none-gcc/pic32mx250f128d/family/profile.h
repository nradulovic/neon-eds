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
 * @brief   	Profile for Microchip PIC32MX250F128
 * @addtogroup  pic32-none-gcc
 * @brief       Profile for Microchip PIC32MX250F128
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-pic32mx250f128 Microchip PIC32MX250F128
 * @brief       Profile for Microchip PIC32MX150F128
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_PROFILE_H_
#define ES_PROFILE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "p32xxxx.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Specifies maximum CPU clock speed in Hz.
 */
#define ES_PROFILE_MAX_CPU_CLOCK        50000000ul

/**@brief       System timer maximum value
 * @details     PIC32 family has 32-bit wide system tick register
 */
#define ES_PROFILE_MAX_SYSTIMER_VAL     0xfffffffful

/**@brief       Maximum RAM size for this family
 * @details     This define is used to choose optimal algorithm for this family
 *              of micro-controllers.
 */
#define ES_PROFILE_MAX_RAM_SIZE         32768u

#if !defined(ES_OPTIMIZE_FOR_SPEED)
#define ES_RAM_SIZE_MAX                 65535
#define ES_RAM_SSIZE_MAX                32767
#define ES_RAM_SSIZE_MIN                -32768
#else
#define ES_RAM_SIZE_MAX                 4294967295ul
#define ES_RAM_SSIZE_MAX                2147483647l
#define ES_RAM_SSIZE_MIN                -2147483648l
#endif /* !ES_OPTIMIZE_FOR_SPEED */

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

#if !defined(ES_OPTIMIZE_FOR_SPEED)
/**@brief       Unsigned RAM memory size type
 */
typedef unsigned short int esRamSize;

/**@brief       Signed RAM memory size type
 */
typedef signed   short int esRamSSize;
#else
typedef unsigned int       esRamSize;
typedef signed   int       esRamSSize;
#endif /* !ES_OPTIMIZE_FOR_SPEED */

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
