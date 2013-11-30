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
 * @brief       Interface of Interrupt port - Template
 * @addtogroup  template_intr
 * @brief       Interrupt port macros and functions
 *********************************************************************//** @{ */

#if !defined(INTR_H__)
#define INTR_H__

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/intr_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Defines maximum interrupt priority which can use kernel services
 */
#define PORT_DEF_MAX_ISR_PRIO                                                   \
    (((PORT_CFG_MAX_ISR_PRIO) << (8u - CPU_DEF_ISR_PRIO_BITS)) & 0xfful)

/**@} *//*----------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Global Enable interrupt sources
 */
#define PORT_INTR_ENABLE()              (void)0

/**@brief       Global Disable all interrupt sources
 */
#define PORT_INTR_DISABLE()             (void)0

/**@brief       Set interrupt priority mask
 * @param       mask
 *              Priority : @ref portReg_T type value for interrupt priority mask
 */
#define PORT_INTR_MASK_SET(mask)        (void)0

/**@brief       Get current interrupt priority mask
 * @param       mask
 *              Priority : pointer to variable of type @ref portReg_T which will
 *              hold the priority value.
 */
#define PORT_INTR_MASK_GET(mask)        (void)0

/**@brief       Get current and set new interrupt priority mask
 * @param       oldPrio
 *              Old priority : pointer to variable of type @ref portReg_T which
 *              will hold old priority value.
 * @param       newPrio : @ref portReg_T type value
 *              New priority mask value
 */
#define PORT_INTR_MASK_REPLACE(oldPrio, newPrio)                                \
    (void)0

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Early port initialization
 * @details     This macro will be called at early initialization stage. It is
 *              called before any system data initialization.
 */
#define PORT_INTR_INIT_EARLY()          (void)0

/**@brief       Port initialization
 * @details     This macro will be called after initialization of system data
 *              structures.
 */
#define PORT_INTR_INIT()                (void)0

/**@brief       Late port initialization
 * @details     This macro will be called just a moment before the multitasking
 *              is started.
 */
#define PORT_INTR_INIT_LATE()           (void)0

/**@brief       Terminate port
 * @details     This macro will be called when there is a need to stop any
 *              further execution.
 */
#define PORT_INTR_TERM()                (void)0

/**@} *//*----------------------------------------------  C++ extern begin  --*/
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
/** @endcond *//** @} *//******************************************************
 * END of int.h
 ******************************************************************************/
#endif /* INTR_H__ */
