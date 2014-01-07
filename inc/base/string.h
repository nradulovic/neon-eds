/*
 * This file is part of esolid-base
 *
 * Template version: 1.1.18 (24.12.2013)
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-base is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with eSolid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	nenad
 * @brief       Interface of string.
 * @defgroup    def_group Group name
 * @brief       Group brief
 *********************************************************************//** @{ */

#ifndef ES_STRING_
#define ES_STRING_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "arch/compiler.h"
#include "base/debug.h"
#include "base/error.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Macro group
 * @brief       brief description
 * @{ *//*--------------------------------------------------------------------*/

#define ES_STRING_INITIALIZE_RO(string)                                         \
    {string, sizeof(string), 1}

#define ES_STRING_INITIALIZE_RW(string)                                         \
    {string, sizeof(string), 0}

/** @} *//*-------------------------------------------------------------------*/
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

/*===================================================  FUNCTION PROTOTYPES  ==*/


/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of string
 ******************************************************************************/
#endif /* ES_STRING_ */
