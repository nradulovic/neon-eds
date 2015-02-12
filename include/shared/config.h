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
 * @author  	Nenad Radulovic
 * @brief   	Common package configuration header
 * @addtogroup  common Common
 *********************************************************************//** @{ */
/**@defgroup    common_cfg Package configuration
 * @brief       Common package configuration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_SHARED_CONFIG_H_
#define NEON_BASE_SHARED_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name       	base::shared: Debugging options
 * @{ *//*--------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------*//**
 * @name       	base::port: Port options
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Priority of critical sections in kernel
 * @details     Specify the priority in range: <code>0</code>\f$\leq\f$
 *              <code>CONFIG_INTR_MAX_ISR_PRIO</code> \f$\leq\f$ <code>15</code>.
 *              The lower the number the higher the priority.
 * @note        When priority is set to @b 0 then critical code section will not
 *              use priority levels but instead it will just disable interrupts
 *              on entry and enable interrupts on exit.
 */
#if !defined(CONFIG_INTR_MAX_ISR_PRIO)
# define CONFIG_INTR_MAX_ISR_PRIO       0u
#endif

/**@brief       The frequency of clock which is used for the system timer
 * @details     System timer SysTick uses core clock (sometimes referred to as
 *              HCLK) for counting. Specify here the core clock so the OS can
 *              properly manage system tick event generation.
 */
#if !defined(CONFIG_SYSTIMER_CLOCK_FREQ)
# define CONFIG_SYSTIMER_CLOCK_FREQ     1000000ul
#endif

/**@brief       The frequency of system timer tick event
 * @note        This setting is valid only if configuration option
 *              @ref CONFIG_SYSTIMER_CLOCK_FREQ is properly set.
 */
#if !defined(CONFIG_SYSTIMER_EVENT_FREQ)
# define CONFIG_SYSTIMER_EVENT_FREQ     100ul
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name       	eds: Kernel configuration
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Scheduler priority levels
 * @details     Possible values:
 *              - Min: 3 (three priority levels)
 *              - Max: 256
 */
#if !defined(CONFIG_PRIORITY_LEVELS)
# define CONFIG_PRIORITY_LEVELS             32u
#endif

#if !defined(CONFIG_PRIORITY_BUCKETS)
# define CONFIG_PRIORITY_BUCKETS            32u
#endif

/**@brief       Enable/disable registry
 * @details     Possible values are:
 *              - 0u - registry is disabled
 *              - 1u - registry is enabled
 */
#if !defined(CONFIG_REGISTRY)
# define CONFIG_REGISTRY                    1u
#endif

#if !defined(CONFIG_REGISTRY_NAME_SIZE)
# define CONFIG_REGISTRY_NAME_SIZE			16u
#endif


/**@} *//*----------------------------------------------------------------*//**
 * @name       	eds: Event object configuration
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Attributes of event header structure
 * @details     The user can easily specify here additional attributes for event
 *              header structure to control the layout of the structure in
 *              memory. The layout of structure can be important when event are
 *              sent across networked devices.
 * @note        Default settings: empty (no additional structure attribute)
 */
#if !defined(CONFIG_EVENT_STRUCT_ATTRIBUTE)
# define CONFIG_EVENT_STRUCT_ATTRIBUTE
#endif

/**
 * @brief       Koristi se atribut o velicini dogadjaja
 * @details     Moguce vrednosti:
 *              - 0 - ne koristi se atribut o velicini
 *              - 1 - koristi se atribut o velicini
 *
 *              Podesavanje tipa se vrsi pomocu @ref CFG_EVT_SIZE_TYPE.
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              velicini)
 */
#if !defined(CONFIG_EVENT_SIZE)
# define CONFIG_EVENT_SIZE              0
#endif

/**
 * @brief       Koristi se pokazivac na proizvodjaca dogadjaja.
 * @details     Moguce vrednosti:
 *              - 0 - pokazivac na objekat se ne koristi
 *              - 1 - pokazivac na objekat se koristi
 *
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              generatoru)
 */
#if !defined(OPT_EVT_USE_GENERATOR)
# define CONFIG_EVENT_PRODUCER          0
#endif

#if !defined(CONFIG_EVENT_STORAGE_NPOOLS)
# define CONFIG_EVENT_STORAGE_NPOOLS    2
#endif

#if !defined(CONFIG_GET_CURRENT_EPA)
# define CONFIG_GET_CURRENT_EPA(epa)                                            \
    do {                                                                        \
        extern struct nepa * neds_get_current(void);                            \
        *epa = neds_get_current();                                              \
    } while (0)
#endif


/**@} *//*----------------------------------------------------------------*//**
 * @name       	eds: state machine processor
 * @{ *//*--------------------------------------------------------------------*/

#if !defined(CONFIG_SMP_HSM)
# define CONFIG_SMP_HSM                 1
#endif

#if !defined(CONFIG_SMP_EVENT_ID_BASE)
# define CONFIG_SMP_EVENT_ID_BASE       0u
#endif

/**@} *//*--------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if ((CONFIG_DEBUG != 1) && (CONFIG_DEBUG != 0))
# error "NEON::base::shared: Configuration option CONFIG_DEBUG is out of range."
#endif

#if ((CONFIG_API_VALIDATION != 1) && (CONFIG_API_VALIDATION != 0))
# error "NEON::base::shared: Configuration option CONFIG_DEBUG_API_VALIDATION is out of range."
#endif

#if ((CONFIG_ASSERT_INTERNAL != 1) && (CONFIG_ASSERT_INTERNAL != 0))
# error "NEON::base::shared: Configuration option CONFIG_DEBUG_INTERNAL_CHECK is out of range."
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
 * END of config.h
 ******************************************************************************/
#endif /* NEON_BASE_SHARED_CONFIG_H_ */
