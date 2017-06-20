/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Software component information header
 * @addtogroup  base_intf
 *********************************************************************//** @{ */
/**@defgroup    base_component Software component information
 * @brief       Software component information
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_COMPONENT_H_
#define NEON_BASE_COMPONENT_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "port/compiler.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Declare a module information
 * @param       description
 *              Module description : string
 * @api
 */
#define NCOMPONENT_DEFINE(description)                                          \
    struct ncomponent_info PORT_C_UNUSED g_component_info =                     \
    {                                                                           \
        description,                                                            \
        PORT_C_FILE                                                             \
    }

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Defines module information structure
 * @api
 */
struct ncomponent_info
{
    const char * const desc;            /**<@brief Component description      */
    const char * const file;            /**<@brief Component source file      */
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct ncomponent_info PORT_C_UNUSED g_unknown_component;

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of component.h
 ******************************************************************************/
#endif /* NEON_BASE_COMPONENT_H_ */
