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

#include "arch/compiler.h"
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
    size_t              size;
    uint8_t             attrib;
    char                data[1];
};

typedef struct esString esString;

/*======================================================  GLOBAL VARIABLES  ==*/

esError esStringCreate(
    esString **         string,
    const char *        data);

static inline const char * esStringGetData(
    const esString *    string) {

    return (string->data);
}

static inline size_t esStringGetSize(
    const esString *    string) {

    return (string->size);
}

size_t esStringCopy(esString * destination, const esString * source);
uint32_t esStringFindChar(const esString * string, char find);
uint32_t esStringFindSubString(const esString * string, const esString * find);


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
