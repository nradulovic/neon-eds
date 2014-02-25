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
 * @author      Nenad Radulovic
 * @brief       Implementation of PIC32 interrupt port.
 * @addtogroup  pic32-none-gcc-intr_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>

#include "arch/intr.h"
#include "arch/cpu.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void portModuleIntrInit(
    void) {
    esCpuReg            cause;
    esCpuReg            status;

    ES_INTR_DISABLE();
    /*--  Use vectored interrupt table  --------------------------------------*/
    cause   = _CP0_GET_CAUSE();
    cause  |= _CP0_CAUSE_IV_MASK;
    _CP0_SET_CAUSE(cause);
    status  = _CP0_GET_STATUS();
    status &= ~_CP0_STATUS_BEV_MASK;
    _CP0_SET_STATUS(status);
    INTCONSET = _INTCON_MVEC_MASK;
    ES_INTR_ENABLE();
}

void portModuleIntrTerm(
    void) {

    ES_INTR_DISABLE();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of intr.c
 ******************************************************************************/
