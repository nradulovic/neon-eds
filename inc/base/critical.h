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
 * @brief       Interface of Critical code section locking management.
 * @defgroup    base_critical Critical code section locking management
 * @brief       These methods are used to protect concurrent access to a shared
 *              resource.
 *********************************************************************//** @{ */

#if !defined(CRITICAL_H__)
#define CRITICAL_H__

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/intr.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Critical code lock management
 * @brief       Disable/enable interrupts by preserving the interrupt context
 * @details     Generally speaking these macros would store the interrupt
 *              context in the local variable of @ref portReg_T type and then
 *              disable interrupts. Local variable is allocated in all of eSolid
 *              functions that need to disable interrupts. Macros would restore
 *              the interrupt context by copying back the allocated variable
 *              into the interrupt controller status/control register.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Enter critical code section
 * @param       inteCtx
 *              Interrupt context, pointer to portable type variable which will
 *              hold the interrupt context state during the critical code
 *              section.
 */
#define ES_CRITICAL_LOCK_ENTER(intrCtx)                                         \
    PORT_INTR_MASK_REPLACE(intrCtx, PORT_DEF_MAX_ISR_PRIO)

/**@brief       Exit critical code section
 * @param       intrCtx
 *              Interrupt context, portable type variable which is holding a
 *              previously saved interrupt context state.
 */
#define ES_CRITICAL_LOCK_EXIT(intrCtx)                                          \
    PORT_INTR_MASK_SET(intrCtx)

/**@} *//**@} *//*--------------------------------------  C++ extern begin  --*/
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
 * END of critical.h
 ******************************************************************************/
#endif /* CRITICAL_H__ */
