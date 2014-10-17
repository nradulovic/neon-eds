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
 * @brief       Error handling header
 * @defgroup    base_error Error handling
 * @brief       Error handling
 *********************************************************************//** @{ */
/**@defgroup    base_error_intf Interface
 * @brief       Error handling API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NERROR_H
#define NERROR_H

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Error table
 * @details     This table defines error enumerators, number and associated text
 */
#define NERROR_TABLE_(entry)                                                    \
    entry(NERROR_NONE,                  0u,     "no error")                     \
    entry(NERROR_OBJECT_NFOUND,         100u,   "object not found")             \
    entry(NERROR_OBJECT_INVALID,        101u,   "object is invalid")            \
    entry(NERROR_DEVICE_FAIL,           201u,   "device failure")               \
    entry(NERROR_DEVICE_BUSY,           202u,   "device is busy")               \
    entry(NERROR_NO_DEVICE,             203u,   "no free device")               \
    entry(NERROR_NO_MEMORY,             300u,   "no free memory")               \
    entry(NERROR_NO_RESOURCE,           301u,   "no free resource")             \
    entry(NERROR_NO_REFERENCE,          302u,   "no free reference slot")       \
    entry(NERROR_NOT_IMPLEMENTED,       1000u,  "operation not implemented")    \
    entry(NERROR_NOT_PERMITTED,         1001u,  "operation not permitted")      \
    entry(NERROR_NOT_ENABLED,           1002u,  "operation not enabled")        \
    entry(NERROR_NOT_FOUND,             1002u,  "item not found")               \
    entry(NERROR_ARG_INVALID,           2000u,  "argument is invalid")          \
    entry(NERROR_ARG_OUT_OF_RANGE,      2001u,  "argument is out of range")     \
    entry(NERROR_ARG_NULL,              2002u,  "argument is null")

/**@brief       Expander for error enum
 */
#define NERROR_ENUM_(a, b, c)               a = b,

/*============================================================  DATA TYPES  ==*/

/**@brief       Error enumeration, see the error table
 */
enum nerror {
    NERROR_TABLE_(NERROR_ENUM_)
    NERROR_LAST_NUMBER                                                          /**< @brief This enum member is not used for indication     */
};

/**@brief       Error type
 */
typedef enum nerror nerror;

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Error text which is associated with error number
 */
extern const PORT_C_ROM char * const PORT_C_ROM_VAR g_error_text[];

/*===================================================  FUNCTION PROTOTYPES  ==*/

const char * nerror_no_to_text(enum nerror error_no);

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of nerror.h
 ******************************************************************************/
#endif /* NERROR_H */
