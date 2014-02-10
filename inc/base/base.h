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
 * @brief       Base module header
 * @defgroup    base Base
 * @brief       Debugging support
 *********************************************************************//** @{ */
/**@defgroup    base_intf Interface
 * @brief       Base API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_BASE_H_
#define ES_BASE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Declare a module information card
 * @param       modName
 *              Module name : string
 * @param       modDesc
 *              Module description : string
 * @param       modAuth
 *              Module author : string
 * @api
 */
#define ES_MODULE_INFO_CREATE(name, description, author)                        \
    PORT_C_ROM struct PORT_C_UNUSED esModuleInfo LocalModuleInfo = {            \
        name,                                                                   \
        description,                                                            \
        author,                                                                 \
        PORT_C_FILE                                                             \
    }

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct esModuleInfo {
    const PORT_C_ROM char * const PORT_C_ROM_VAR name;                      /**< @brief Module name                                     */
    const PORT_C_ROM char * const PORT_C_ROM_VAR desc;                      /**< @brief Module description                              */
    const PORT_C_ROM char * const PORT_C_ROM_VAR auth;                      /**< @brief Module author                                   */
    const PORT_C_ROM char * const PORT_C_ROM_VAR file;                      /**< @brief Module source file                              */
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern PORT_C_UNUSED const PORT_C_ROM struct esModuleInfo GlobalUnnamedModule;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Function group
 * @brief       brief description
 * @{ *//*--------------------------------------------------------------------*/

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of base.h
 ******************************************************************************/
#endif /* ES_BASE_H_ */
