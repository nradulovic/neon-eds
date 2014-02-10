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
 * @brief   	State Machine Processor Configuration
 * @addtogroup  eds_smp
 *********************************************************************//** @{ */
/**@defgroup    eds_smp_cfg State Machine Processor Configuration
 * @brief       State Machine Processor Configuration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef SMP_CONFIG_H_
#define SMP_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

#if !defined(CONFIG_SMP_HSM)
# define CONFIG_SMP_HSM                 1
#endif

#if !defined(CONFIG_SMP_EVENT_ID_BASE)
# define CONFIG_SMP_EVENT_ID_BASE       0u
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of smp_config.h
 ******************************************************************************/
#endif /* SMP_CONFIG_H_ */
