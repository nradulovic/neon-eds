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
 * @brief       Debug support Implementation
 * @addtogroup  base_dbg
 *********************************************************************//** @{ */
/**@defgroup    base_dbg_impl Implementation
 * @brief       Debug support Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stddef.h>

#include "plat/compiler.h"
#include "arch/cpu.h"
#include "arch/intr.h"
#include "base/debug.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/* 1)       This function will disable all interrupts to prevent any new
 *          interrupts to execute which can trigger another assert causing a
 *          very confusing situation of why it failed.
 */
PORT_C_NORETURN void debugAssert(
    const PORT_C_ROM struct debugCobject_ * cObject,
    const PORT_C_ROM char * expr,
    const PORT_C_ROM char * msg) {

    struct esDebugReport debugReport;

    ES_INTR_DISABLE();

    if (cObject->mod != NULL) {
        debugReport.modName   = cObject->mod->name;
        debugReport.modDesc   = cObject->mod->desc;
        debugReport.modAuthor = cObject->mod->auth;
        debugReport.modFile   = cObject->mod->file;
    } else {
        debugReport.modName   = "Unnamed";
        debugReport.modDesc   = "not specified";
        debugReport.modAuthor = "not specified";
        debugReport.modFile   = "not specified";
    }
    debugReport.fnName    = cObject->fn;
    debugReport.expr      = expr;
    debugReport.msg       = msg;
    debugReport.line      = cObject->line;
    userAssert(
        &debugReport);
    ES_CPU_TERM();

    while (true);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of dbg.c
 ******************************************************************************/
