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
 * @brief       Static Memory Management Implementation
 * @addtogroup  base_static_mem
 *********************************************************************//** @{ */
/**@defgroup    base_static_mem_impl Implementation
 * @brief       Static Memory Management Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>

#include "arch/compiler.h"
#include "base/static_mem.h"
#include "base/debug.h"
#include "base/bitop.h"
#include "base/critical.h"
#include "base/error.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Signature for static memory manager
 */
#define STATIC_MEM_SIGNATURE            ((esAtomic)0xDEADBEEDU)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Module information
 */
static ES_MODULE_INFO("StaticMem", "Static Memory management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

esStaticMem esGlobalStaticMem;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void esStaticMemInit(
    esStaticMem *       staticMem,
    void *              storage,
    size_t              storageSize) {

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, NULL != staticMem);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, NULL != storage);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE, 0u != storageSize);

    staticMem->base = storage;
    staticMem->size = ES_ALIGN(storageSize, sizeof(esAtomic));
    staticMem->free = staticMem->size;

    ES_DEBUG_API_OBLIGATION(staticMem->signature = STATIC_MEM_SIGNATURE);
}

void * esStaticMemAllocI(
    esStaticMem *    handle,
    size_t              size) {

    void *              mem;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, NULL != handle);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT, STATIC_MEM_SIGNATURE == handle->signature);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE, 0u != size);

    size = ES_DIVISION_ROUNDUP(size, sizeof(esAtomic));

    if (size > handle->free)
    {
        return ((void *)ES_ERROR_MEMORY_FULL);
    }
    handle->free -= size;
    mem = (void *)&handle->base[handle->free];

    return (mem);
}

void * esStaticMemAlloc(
    esStaticMem *    handle,
    size_t              size) {

    esAtomic           intCtx;
    void *              mem;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    mem = esStaticMemAllocI(
        handle,
        size);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (mem);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of mem.c
 ******************************************************************************/
