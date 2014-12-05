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
 * @brief       Event Object Configuration
 * @addtogroup  eds_event
 *********************************************************************//** @{ */
/**@defgroup    eds_event_cfg Event Object Configuration
 * @brief       Event Object Configuration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_EVENT_CONFIG_H_
#define ES_EVENT_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Podesavanje Event objekta
 * @brief       Specifikacija @ref esEvent strukture dogadjaja
 * @details     Opcijom @ref CONFIG_EVENT_STRUCT_ATTRIBUTE se definise kakav layout
 *              struktura treba da ima u memoriji. Ovde se najpre misli na
 *              poravnjanje (@c alignment) elemenata u strukturi. Ostale opcije
 *              specificiraju koje elemente struktura poseduje.
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
# define CONFIG_EVENT_SIZE              1
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
# define CONFIG_EVENT_PRODUCER          1
#endif

/**
 * @brief       Koristi se vremenski marker dogadjaja.
 * @details     Moguce vrednosti:
 *              - 0 - vrem. marker se ne koristi
 *              - 1 - vrem. marker se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref CFG_EVT_TIMESTAMP_TYPE.
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              vremenskom markeru)
 */
#if !defined(OPT_EVT_USE_TIMESTAMP)
# define CONFIG_EVENT_TIMESTAMP         1
#endif

#if !defined(CONFIG_EVENT_STORAGE_NPOOLS)
# define CONFIG_EVENT_STORAGE_NPOOLS    2
#endif

#if !defined(CONFIG_GET_CURRENT_EPA)
# define CONFIG_GET_CURRENT_EPA(epa)                                            \
    do {                                                                        \
        esError esEdsGetCurrent(struct esEpa **);                               \
        (void)esEdsGetCurrent(epa);                                             \
    } while (0)
#endif

/**@} *//*--------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//** @} *//*********************************************
 * END of event_config.h
 *//** @} *//******************************************************************/
#endif /* ES_EVENT_CONFIG_H_ */
