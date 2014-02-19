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

#include <xc.h>
#include <peripheral/int.h>

#include "plat/compiler.h"
#include "arch/intr_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

#define ES_INTR_ENABLE()                portIntrEnable_()

#define ES_INTR_DISABLE()               portIntrDisable_()

#define ES_INTR_MASK_SET(mask)          portIntrMaskSet_(mask)

#define ES_INTR_MASK_GET(mask)          portIntrMaskGet_(mask)

#define ES_INTR_MASK_REPLACE(oldPrio, newPrio)                                  \
    portIntrMaskReplace_(oldPrio, newPrio)

#define ES_INTR_PRIO_TO_CODE(prio)                                              \
    (((esIntrCtx)(prio) << PORT_STATUS_IPL_Pos) & PORT_STATUS_IPL_Msk)

#define ES_INTR_CODE_TO_PRIO(code)                                              \
    (((esIntrCtx)(code) & PORT_STATUS_IPL_Msk) >> PORT_STATUS_IPL_Pos)

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

/**@} *//*----------------------------------------------------------------*//**
 * @name        Specific port macros
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_STATUS_IPL_Pos             10

#define PORT_STATUS_IPL_Msk             (0x7u << PORT_STATUS_IPL_Pos)

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

static PORT_C_INLINE_ALWAYS void __attribute__((nomips16)) portIntrEnable_(
    void) {
    __asm __volatile__(
        "   ei                                              \n");
}

static PORT_C_INLINE_ALWAYS void __attribute__((nomips16)) portIntrDisable_(
    void) {
    __asm __volatile__(
        "   di                                              \n");
}

/* NOTE: This is some very bad design right here. You must apply R-M-W operation
 *       on a register to set up new ISR priority level. The R-M-W operation is
 *       not atomic so there is a possibility to scramble status register in
 *       some cases.
 */
static PORT_C_INLINE_ALWAYS void portIntrMaskSet_(
    esIntrCtx           intrCtx) {
    esIntrCtx           statusReg;

    statusReg  = _CP0_GET_STATUS();
    statusReg &= PORT_STATUS_IPL_Msk;
    statusReg |= intrCtx & PORT_STATUS_IPL_Msk;
    _CP0_SET_STATUS(statusReg);
}

static PORT_C_INLINE_ALWAYS void portIntrMaskGet_(
    esIntrCtx *         intrCtx) {

    *intrCtx = _CP0_GET_STATUS();
}

/* NOTE: See notes for portIntrMaskSet_()
 */
static PORT_C_INLINE_ALWAYS void portIntrMaskReplace_(
    esIntrCtx *         oldMask,
    esIntrCtx           newMask) {
    
    esIntrCtx           statusReg;

    statusReg  = *oldMask = _CP0_GET_STATUS();
    statusReg &= PORT_STATUS_IPL_Msk;
    statusReg |= newMask & PORT_STATUS_IPL_Msk;
    _CP0_SET_STATUS(statusReg);
}

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
