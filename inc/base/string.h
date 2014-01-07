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
 * @brief       String manipulation
 * @defgroup    base_string String manipulation
 * @brief       tring manipulation
 *********************************************************************//** @{ */
/**@defgroup    base_string_intf Interface
 * @brief       String API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_STRING_H_
#define ES_STRING_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "arch/compiler.h"
#include "base/debug.h"
#include "base/error.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct esString {
    size_t              length;
    char *              text;
#if (CONFIG_DEBUG_API_VALIDATION == 1) || defined(__DOXYGEN__)
    esAtomic             signature;
#endif
};

typedef struct esString esString;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

esError esStringCreate(
    const char *        text,
    esString **         string);

esError esStringCopy(
    esString *          destination,
    const esString *    source);

esError esStringFindChar(
    const esString *    string,
    char                find,
    size_t *            position);

esError esStringAreEqual(
    const esString *    string1,
    const esString *    string2,
    bool *              areEqual);

esError esStringFindSubString(
    const esString *    string,
    const esString *    find,
    size_t *            position);

esError esStringGetText(
    const esString *    string,
    char **             text);

esError esStringGetLength(
    const esString *    string,
    size_t *            length);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of string
 ******************************************************************************/
#endif /* ES_STRING_H_ */
