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
 * @brief   	Debug support configuration header
 * @addtogroup  base_dbg
 *********************************************************************//** @{ */
/**@defgroup    base_dbg_cfg Configuration
 * @brief       Debug support Configuration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_DEBUG_CONFIG_H_
#define ES_DEBUG_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/**@brief       Enable/disable Debug module
 * @details     Possible values:
 *              - 0 - All debug options are disabled
 *              - 1 - Debug options can be enabled individually
 */
#if !defined(CONFIG_DEBUG)
# define CONFIG_DEBUG                  1
#endif

/**@brief       Enable/disable API arguments validation
 * @details     Possible values:
 *              - 0 - API validation is disabled
 *              - 1 - API validation is enabled
 *
 * @note        This option is enabled only if @ref CONFIG_DEBUG is enabled, too.
 */
#if !defined(CONFIG_API_VALIDATION)
# define CONFIG_API_VALIDATION          1
#endif

/**@brief       Enable/disable internal checks
 * @details     Possible values:
 *              - 0 - API validation is disabled
 *              - 1 - API validation is enabled
 *
 * @note        This option is enabled only if @ref CONFIG_DEBUG is enabled, too.
 */
#if !defined(CONFIG_ASSERT_INTERNAL)
# define CONFIG_ASSERT_INTERNAL         1
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if ((CONFIG_DEBUG != 1) && (CONFIG_DEBUG != 0))
# error "eSolid RT Kernel: Configuration option CONFIG_DEBUG is out of range."
#endif

#if ((CONFIG_API_VALIDATION != 1) && (CONFIG_API_VALIDATION != 0))
# error "eSolid RT Kernel: Configuration option CONFIG_DEBUG_API_VALIDATION is out of range."
#endif

#if ((CONFIG_ASSERT_INTERNAL != 1) && (CONFIG_ASSERT_INTERNAL != 0))
# error "eSolid RT Kernel: Configuration option CONFIG_DEBUG_INTERNAL_CHECK is out of range."
#endif

#if (CONFIG_DEBUG == 0) || defined(NDEBUG)
# undef  CONFIG_DEBUG
# define CONFIG_DEBUG                   0
# undef  CONFIG_API_VALIDATION
# define CONFIG_API_VALIDATION          0
# undef  CONFIG_ASSERT_INTERNAL
# define CONFIG_ASSERT_INTERNAL         0
#endif

/** @endcond *//** @} *//** @} *//*********************************************
 * END of debug_config.h
 ******************************************************************************/
#endif /* ES_DEBUG_CONFIG_H_ */
