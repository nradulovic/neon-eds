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
 * @brief       Error enumeration header
 * @defgroup    common_error Error
 * @brief       Error enumeration
 *********************************************************************//** @{ */
/**@defgroup    common_error_intf Interface
 * @brief       Error enumeration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_SHARED_ERROR_H_
#define NEON_BASE_SHARED_ERROR_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "port/compiler.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Error table
 * @details     This table defines error enumerators, number and associated text
 */
#define ERROR_TABLE_(entry)                                                     \
    entry(NERROR_NONE,                  0u,     "no error")                     \
    entry(NERROR_TIMEOUT,               1u,     "general timeout")              \
    entry(NERROR_OP_ABORT,              2u,     "operation aborted")            \
    entry(NERROR_OBJECT_NFOUND,         100u,   "object not found")             \
    entry(NERROR_OBJECT_INVALID,        101u,   "object is invalid")            \
    entry(NERROR_DEVICE_FAIL,           201u,   "device failure")               \
    entry(NERROR_DEVICE_BUSY,           202u,   "device is busy")               \
    entry(NERROR_DEVICE_NO_COMM,        203u,   "no communication with device") \
    entry(NERROR_NO_DEVICE,             204u,   "no free device")               \
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
#define ERROR_ENUM_(a, b, c)                a = b,

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Error enumeration, see the error table
 */
enum nerror 
{
    ERROR_TABLE_(ERROR_ENUM_)
    N_LAST_ERROR_NUMBER                     /**< @brief Last enum member      */
};

/**@brief       Error type
 */
typedef enum nerror nerror;

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Error text which is associated with error number
 */
extern const PORT_C_ROM char * const PORT_C_ROM_VAR g_error_text[];

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of error.h
 ******************************************************************************/
#endif /* NEON_BASE_SHARED_ERROR_H_ */
