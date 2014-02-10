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
 * @brief       Error handling header
 * @defgroup    base_error Error handling
 * @brief       Error handling
 *********************************************************************//** @{ */
/**@defgroup    base_error_intf Interface
 * @brief       Error handling API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_ERROR_H_
#define ES_ERROR_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Error table
 * @details     This table defines error enumerators, number and associated text
 */
#define ES_ERROR_TABLE_(entry)                                                  \
    entry(ES_ERROR_NONE,                0u,     "no error")                     \
    entry(ES_ERROR_OBJECT_NFOUND,       100u,   "object not found")             \
    entry(ES_ERROR_OBJECT_INVALID,      101u,   "object is invalid")            \
    entry(ES_ERROR_DEVICE_FAIL,         201u,   "device failure")               \
    entry(ES_ERROR_DEVICE_BUSY,         202u,   "device is busy")               \
    entry(ES_ERROR_NO_DEVICE,           203u,   "no free device")               \
    entry(ES_ERROR_NO_MEMORY,           300u,   "no free memory")               \
    entry(ES_ERROR_NO_RESOURCE,         301u,   "no free resource")             \
    entry(ES_ERROR_NO_REFERENCE,        302u,   "no free reference slot")       \
    entry(ES_ERROR_NOT_IMPLEMENTED,     1000u,  "operation not implemented")    \
    entry(ES_ERROR_NOT_PERMITTED,       1001u,  "operation not permitted")      \
    entry(ES_ERROR_NOT_ENABLED,         1002u,  "operation not enabled")        \
    entry(ES_ERROR_NOT_FOUND,           1002u,  "item not found")               \
    entry(ES_ERROR_ARG_INVALID,         2000u,  "argument is invalid")          \
    entry(ES_ERROR_ARG_OUT_OF_RANGE,    2001u,  "argument is out of range")     \
    entry(ES_ERROR_ARG_NULL,            2002u,  "argument is null")

/**@brief       Expander for error enum
 */
#define ES_ERROR_ENUM_(a, b, c)                                                 \
    a = b,

/*============================================================  DATA TYPES  ==*/

/**@brief       Error enumeration, see the error table
 */
enum esError {
    ES_ERROR_TABLE_(ES_ERROR_ENUM_)
    ES_LAST_ERROR_NUMBER                                                        /**< @brief This enum member is not used for indication     */
};

/**@brief       Error type
 */
typedef enum esError esError;

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Error text which is associated with error number
 */
extern const PORT_C_ROM char * const PORT_C_ROM_VAR esGlobalErrorText[];

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of error.h
 ******************************************************************************/
#endif /* ES_ERROR_H_ */
