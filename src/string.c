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
 * @brief       String implementation
 * @addtogroup  base_string
 *********************************************************************//** @{ */
/**@defgroup    base_string_impl Implementation
 * @brief       String Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/string.h"
#include "base/heap_mem.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define STRING_SIGNATURE                ((esAtomic)0xdeadbeaeu)

/*======================================================  LOCAL DATA TYPES  ==*/

ES_MODULE_INFO("String", "String operations", "Nenad Radulovic");

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esStringCreate(
    const char *        text,
    esString **         string) {

    esError             error;
    size_t              length;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, text != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, string != NULL);

    error = esHeapMemAlloc(&esGlobalHeapMem, sizeof(esString), (void **)string);

    if (error != ES_ERROR_NONE) {
        goto UNDO_MEM_ALLOC_STRING;
    }
    length = 0u;

    while (text[length] != '\0') {
        length++;
    }
    (*string)->length = length;
    error = esHeapMemAlloc(&esGlobalHeapMem, length, (void **)&(*string)->text);

    if (error != ES_ERROR_NONE) {
        goto UNDO_MEM_ALLOC_TEXT;
    }

    while (length != 0u) {
        length--;
        (*string)->text[length] = text[length];
    }
    ES_DEBUG_API_OBLIGATION((*string)->signature = STRING_SIGNATURE);

    return (ES_ERROR_NONE);

UNDO_MEM_ALLOC_TEXT:
    (void)esHeapMemFree(&esGlobalHeapMem, string);

UNDO_MEM_ALLOC_STRING:

    return (error);
}

esError esStringCopy(
    esString *          destination,
    const esString *    source) {

    esError             error;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, destination != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  destination->signature == STRING_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, source != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  source->signature == STRING_SIGNATURE);

    if (destination->length != source->length) {
        char *          text;

        error = esHeapMemAlloc(&esGlobalHeapMem, source->length, (void **)&text);

        if (error != ES_ERROR_NONE) {
            goto UNDO_MEM_ALLOC_TEXT;
        }
        (void)esHeapMemFree(&esGlobalHeapMem, destination->text);
        destination->text   = text;
    }
    destination->length = 0u;

    while (destination->length < source->length) {
        destination->text[destination->length] = source->text[destination->length   ];
        destination->length++;
    }

    return (ES_ERROR_NONE);

UNDO_MEM_ALLOC_TEXT:

    return (error);
}

esError esStringFindChar(
    const esString *    string,
    char                find,
    size_t *            position) {

    size_t              cnt;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, string != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  string->signature == STRING_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, position != NULL);

    cnt = 0u;

    while (cnt < string->length) {

        if (string->text[cnt] == find) {
            *position = cnt;

            return (ES_ERROR_NONE);
        }
        cnt++;
    }

    return (ES_ERROR_NOT_FOUND);
}

esError esStringAreEqual(
    const esString *    string1,
    const esString *    string2,
    bool *              areEqual) {

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, string1 != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  string1->signature == STRING_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, string2 != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  string2->signature == STRING_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, areEqual != NULL);

    *areEqual = false;

    if (string1->length == string2->length) {
        size_t          length;

        length = string1->length;

        while (length != 0) {
            length--;

            if (string1->text[length] != string2->text[length]) {

                return (ES_ERROR_NONE);
            }
        }
        *areEqual = true;
    }

    return (ES_ERROR_NONE);
}

esError esStringFindSubString(
    const esString *    string,
    const esString *    find,
    size_t *            position) {

    (void)string;
    (void)find;
    (void)position;

    return (ES_ERROR_NOT_IMPLEMENTED);
}

esError esStringGetText(
    const esString *    string,
    char **             text) {

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, string != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  string->signature == STRING_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, text != NULL);

    *text = string->text;

    return (ES_ERROR_NONE);
}

esError esStringGetLength(
    const esString *    string,
    size_t *            length) {

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, string != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  string->signature == STRING_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, length != NULL);

    *length = string->length;

    return (ES_ERROR_NONE);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of string.c
 ******************************************************************************/
