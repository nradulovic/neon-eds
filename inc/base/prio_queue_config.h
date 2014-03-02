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
 * @brief       Priority queue configuration
 * @defgroup    base_prio_queue Priority queue configuration
 * @brief       Priority queue configuration
 *********************************************************************//** @{ */
/**@defgroup    base_prio_queue_cfg Configuration
 * @brief       Priority queue configuration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_PRIO_QUEUE_CONFIG_H_
#define ES_PRIO_QUEUE_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

#if !defined(CONFIG_PQ_PRIORITY_LEVELS)
#define CONFIG_PQ_PRIORITY_LEVELS             32u
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of prio_queue_config.h
 ******************************************************************************/
#endif /* ES_PRIO_QUEUE_CONFIG_H_ */
