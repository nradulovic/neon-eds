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
 * @author      Nenad Radulovic
 * @brief       Profile for Microchip PIC32MX150F128
 * @addtogroup  pic32-none-gcc
 * @brief       Profile for Microchip PIC32MX150F128
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-pic32mx150f128 Microchip PIC32MX150F128
 * @brief       Profile for Microchip PIC32MX150F128
 * @{ *//*--------------------------------------------------------------------*/

#ifndef DEVICE_PROFILE_H_
#define DEVICE_PROFILE_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

/**@brief       Specifies maximum CPU clock speed in Hz.
 */
#define NDEVICE_MAX_CPU_CLOCK           70000000ul

/**@brief       Maximum RAM for this device
 */
#define NDEVICE_RAM_SIZE                53248u

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
#endif /* DEVICE_PROFILE_H_ */
