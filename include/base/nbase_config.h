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
 * @author  	Nenad Radulovic
 * @brief   	Base configuration settings
 * @addtogroup  base
 *********************************************************************//** @{ */
/**@defgroup    base_cfg Configuration
 * @brief       Base configuration settings
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NBASE_CONFIG_H
#define NBASE_CONFIG_H

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/**@brief       Enable/disable Debug module
 * @details     Possible values:
 *              - 0 - All debug options are disabled
 *              - 1 - Debug options can be enabled individually
 */
#if !defined(CONFIG_DEBUG)
# define CONFIG_DEBUG                       1
#endif

/**@brief       Enable/disable API arguments validation
 * @details     Possible values:
 *              - 0 - API validation is disabled
 *              - 1 - API validation is enabled
 *
 * @note        This option is enabled only if @ref CONFIG_DEBUG is enabled, too.
 */
#if !defined(CONFIG_DEBUG_API)
# define CONFIG_DEBUG_API                   1
#endif

/**@brief       Enable/disable internal checks
 * @details     Possible values:
 *              - 0 - API validation is disabled
 *              - 1 - API validation is enabled
 *
 * @note        This option is enabled only if @ref CONFIG_DEBUG is enabled, too.
 */
#if !defined(CONFIG_DEBUG_INTERNAL)
# define CONFIG_DEBUG_INTERNAL              1
#endif

/*------------------------------------------------------------------------*//**
 * @name        Kernel configuration options and settings
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Scheduler priority levels
 * @details     Possible values:
 *              - Min: 3 (three priority levels)
 *              - Max: 256
 */
#if !defined(CONFIG_BIAS_LEVELS)
# define CONFIG_BIAS_LEVELS                 32u
#endif

#if !defined(CONFIG_BIAS_BUCKETS)
# define CONFIG_BIAS_BUCKETS                32u
#endif

/**@brief       The frequency of system timer tick event
 * @note        This setting is valid only if configuration option
 *              @ref CONFIG_SYSTIMER_CLOCK_FREQ is properly set in port
 *              configuration file cpu_cfg.h
 */
#if !defined(CFG_SYSTMR_EVENT_FREQUENCY)
# define CFG_SYSTMR_EVENT_FREQUENCY         100ul
#endif

/** @} *//*---------------------------------------------------------------*//**
 * @name        Kernel pre hooks
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       System timer event hook function
 */
#if !defined(CFG_HOOK_SYSTMR_EVENT)
# define CFG_HOOK_SYSTMR_EVENT          0
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if ((CONFIG_DEBUG != 1) && (CONFIG_DEBUG != 0))
# error "Neon RT Kernel: Configuration option CONFIG_DEBUG is out of range."
#endif

#if ((CONFIG_DEBUG_API != 1) && (CONFIG_DEBUG_API != 0))
# error "Neon RT Kernel: Configuration option CONFIG_DEBUG_API is out of range."
#endif

#if ((CONFIG_DEBUG_INTERNAL != 1) && (CONFIG_DEBUG_INTERNAL != 0))
# error "Neon RT Kernel: Configuration option CONFIG_DEBUG_INTERNAL is out of range."
#endif

#if (CONFIG_DEBUG == 0) || defined(NDEBUG)
# undef  CONFIG_DEBUG
# define CONFIG_DEBUG                       0
# undef  CONFIG_DEBUG_API
# define CONFIG_DEBUG_API                   0
# undef  CONFIG_DEBUG_INTERNAL
# define CONFIG_DEBUG_INTERNAL              0
#endif

#if ((3u > CONFIG_BIAS_LEVELS) || (CONFIG_BIAS_LEVELS > 256))
# error "Neon RT Kernel: Configuration option CONFIG_BIAS_LEVELS is out of range."
#endif

#if (CONFIG_BIAS_BUCKETS > CONFIG_BIAS_LEVELS)
# error "Neon RT Kernel: Configuration option CONFIG_BIAS_BUCKETS is out of range. It must be smaller or equal to CONFIG_BIAS_LEVELS."
#endif

#if ((CONFIG_BIAS_BUCKETS !=  1) && (CONFIG_BIAS_BUCKETS !=   2) && (CONFIG_BIAS_BUCKETS !=   4) &&         \
     (CONFIG_BIAS_BUCKETS !=  8) && (CONFIG_BIAS_BUCKETS !=  16) && (CONFIG_BIAS_BUCKETS !=  32) &&         \
     (CONFIG_BIAS_BUCKETS != 64) && (CONFIG_BIAS_BUCKETS != 128) && (CONFIG_BIAS_BUCKETS != 256))
# error "Neon RT Kernel: Configuration option CONFIG_BIAS_BUCKETS is not valid. It must be a 2^n number."
#endif

#if ((1u != CFG_HOOK_SYSTMR_EVENT) && (0u != CFG_HOOK_SYSTMR_EVENT))
# error "Neon RT Kernel: Configuration option CFG_HOOK_SYSTMR_EVENT is out of range."
#endif

/** @endcond *//** @} *//******************************************************
 * END of nbase_config.h
 ******************************************************************************/
#endif /* NBASE_CONFIG_H */
