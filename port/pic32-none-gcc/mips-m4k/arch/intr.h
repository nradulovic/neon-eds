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
 * @brief       Interface of Interrupt module port.
 * @addtogroup  pic32-none-gcc
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-intr Interrupt module
 * @brief       Interrupt module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_INTR_H_
#define ES_INTR_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/int.h>

#include "plat/compiler.h"
#include "arch/intr_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

#define ES_INTR_ENABLE()                INTEnableInterrupts()

#define ES_INTR_DISABLE()               INTDisableInterrupts()

#define ES_INTR_MASK_SET(mask)          INTRestoreInterrupts(mask)

#define ES_INTR_MASK_GET(mask)          *(mask) = 0

#define ES_INTR_MASK_REPLACE(oldPrio, newPrio)                                  \
    do {                                                                        \
        *(oldPrio) = INTDisableInterrupts();                                    \
    } while (0)

#define ES_INTR_PRIO_TO_CODE(prio)                                              \
    ((prio))

#define ES_INTR_CODE_TO_PRIO(code)                                              \
    ((code))

#define ES_INTR_PRIO_SET(intrNum, prio) INTSetVectorPriority(intrNum, prio)

#define ES_INTR_PRIO_GET(intrNum, prio)                                         \
    do {                                                                        \
        *(prio) = INTGetVectorPriority(intrNum);                                \
    } while (0)

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

#define ES_INTR_INIT_EARLY()            (void)0                                 /**< @brief This port does not need this function call      */

#define ES_INTR_INIT()                  portIntrInit()

#define ES_INTR_INIT_LATE()             (void)0                                 /**< @brief This port does not need this function call      */

#define ES_INTR_TERM()                  portIntrTerm()

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

/*------------------------------------------------------------------------*//**
 * @name        Generic port functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize port
 * @details     Function will set up sub-priority bits to zero and handlers
 *              interrupt priority.
 */
void portIntrInit(
    void);

/**@brief       Terminate port
 */
void portIntrTerm(
    void);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of intr.h
 ******************************************************************************/
#endif /* ES_INTR_H_ */
