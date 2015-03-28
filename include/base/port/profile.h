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
 * @brief       MCU profile data
 * @defgroup    profile_intf MCU profile data
 * @brief       MCU profile data
 *********************************************************************//** @{ */

#ifndef NEON_PORT_PROFILE_H_
#define NEON_PORT_PROFILE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "family/p_profile_data.h"
#include "base/port/peripheral.h"
#include "base/shared/config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       This macro checks if a GPIO is enabled.
 * @details     In order to check if particular GPIO port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if GPIO port A is enabled then
 *              you would write 'if (NPROFILE_EN_GPIO & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_GPIO)' which will
 *              evaluate to TRUE if any GPIO port is enabled.
 */
#define NPROFILE_EN_GPIO                (CONFIG_GPIO & NPROFILE_AVAILABLE_GPIO)

/**@brief       This macro checks if an UART is enabled.
 * @details     In order to check if particular UART port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if UART number 1 is enabled then
 *              you would write 'if (NPROFILE_EN_UART & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_UART)' which will
 *              evaluate to TRUE if any UART peripheral is enabled.
 */
#define NPROFILE_EN_UART                (CONFIG_UART & NPROFILE_AVAILABLE_UART)

/**@brief       This macro checks if an I2C peripheral is enabled.
 * @details     In order to check if particular I2C port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if I2C number 1 is enabled then
 *              you would write 'if (NPROFILE_EN_I2C & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_I2C)' which will
 *              evaluate to TRUE if any I2C peripheral is enabled.
 */
#define NPROFILE_EN_I2C                 (CONFIG_I2C &  NPROFILE_AVAILABLE_I2C)

/**@brief       This macro checks if an SPI peripheral is enabled.
 * @details     In order to check if particular SPI port is enabled then do a
 *              bitwise AND operation between this macro and the peripheral
 *              macro @ref NP_DEV_MAJOR().
 *
 *              For example, in order to check if SPI number 1 is enabled then
 *              you would write 'if (NPROFILE_EN_SPI & NP_DEV_MAJOR(1))'.
 *
 *              Likewise, you can write 'if (NPROFILE_EN_SPI)' which will
 *              evaluate to TRUE if any SPI peripheral is enabled.
 */
#define NPROFILE_EN_SPI                 (CONFIG_SPI &  NPROFILE_AVAILABLE_SPI)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

/*
 * Individual peripheral data
 */
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(1))
extern const struct np_dev      g_gpioa;
#endif

#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(2))
extern const struct np_dev      g_gpiob;
#endif

#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(3))
extern const struct np_dev      g_gpioc;
#endif

#if (NPROFILE_EN_UART & NP_EN_MAJOR(1))
extern const struct np_dev      g_uart1;
#endif

#if (NPROFILE_EN_UART & NP_EN_MAJOR(2))
extern const struct np_dev      g_uart2;
#endif

#if (NPROFILE_EN_UART & NP_EN_MAJOR(6))
extern const struct np_dev      g_uart6;
#endif

/*
 * Group peripheral data
 */
#if (NPROFILE_EN_GPIO)
extern const struct np_dev *    g_gpios[];
#endif

#if (NPROFILE_EN_UART)
extern const struct np_dev *    g_uarts[];
#endif

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of profile.h
 ******************************************************************************/
#endif /* NEON_PORT_PROFILE_H_ */
