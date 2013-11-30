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
 * @brief   	Configuration of Interrupt port - Template.
 * @addtogroup  template_intr_cfg
 * @brief		Interrupt port specific configuration options
 * @details     Each configuration option or setting has its own default value
 *              when not defined by the application. When application needs to
 *              change a setting it just needs to define a configuration macro
 *              with another value and the default configuration macro will be
 *              overridden.
 *********************************************************************//** @{ */

#if !defined(INTR_CFG_H__)
#define INTR_CFG_H__

/*=========================================================  INCLUDE FILES  ==*/

#include "family/profile.h"

/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port General configuration
 * @brief       Configuration options and settings which are available for every
 *              port.
 * @note        1) All port General configuration macros are prefixed with
 *              @c CFG_ string.
 * @note        2) All port specific options and constants are prefixed with
 *              @c CPU_ string.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Priority of critical sections in eSolid
 * @details     This option varies with the MCU used. In the simplest case when
 *              the MCU does not support interrupt priorities than only one
 *              priority level is available. In that case critical section will
 *              simply disable interrupts on entry and enable them at exit.
 */
#if !defined(PORT_CFG_MAX_ISR_PRIO)
# define PORT_CFG_MAX_ISR_PRIO          1u
#endif

/** @} *//*-------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of intr_cfg.h
 ******************************************************************************/
#endif /* INTR_CFG_H__ */
