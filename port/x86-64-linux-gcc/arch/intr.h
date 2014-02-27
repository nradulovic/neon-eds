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
 * @brief       Interrupt module header
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-v7-m-intr ARM Cortex M3/M4 Interrupt module
 * @brief       Interrupt module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_INTR_H_
#define ES_INTR_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"
#include "family/profile.h"
#include "arch/intr_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

#define ES_INTR_ENABLE()                (void)0

#define ES_INTR_DISABLE()               (void)0

#define ES_INTR_MASK_SET(mask)          (void)0

#define ES_INTR_MASK_GET(mask)          (void)0

#define ES_INTR_MASK_REPLACE(oldPrio, newPrio)                                  \
    do {                                                                        \
        (void)oldPrio;                                                          \
        (void)newPrio;                                                          \
    } while (0)

#define ES_INTR_PRIO_TO_CODE(prio)                                              \
    (prio)

#define ES_INTR_CODE_TO_PRIO(code)                                              \
    (code)

#define ES_INTR_PRIO_SET(intrNum, prio) (void)0

#define ES_INTR_PRIO_GET(intrNum, prio) (void)0

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

#define ES_INTR_INIT_EARLY()            (void)0

#define ES_INTR_INIT()                  (void)0

#define ES_INTR_INIT_LATE()             (void)0

#define ES_INTR_TERM()                  (void)0

/**@} *//*----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Interrupt context type
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
typedef unsigned int esIntrCtx;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of intr.h
 ******************************************************************************/
#endif /* ES_INTR_H_ */
