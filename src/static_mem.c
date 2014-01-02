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
#define SMEM_SIGNATURE                  ((portReg)0xDEADBEEDU)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Module information
 */
DECL_MODULE_INFO("StaticMem", "Static Memory management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

esStaticMem esGlobalStaticMem;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void esStaticMemInit(
    esStaticMem *    handle,
    void *              storage,
    size_t              storageSize) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != storage);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != storageSize);

    handle->base = storage;
    handle->free = ES_ALIGN(storageSize, sizeof(portReg));
    handle->size = ES_ALIGN(storageSize, sizeof(portReg));

    ES_DBG_API_OBLIGATION(handle->signature = SMEM_SIGNATURE);
}

void * esStaticMemAllocI(
    esStaticMem *    handle,
    size_t              size) {

    void *              mem;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, SMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != size);

    size = ES_DIVISION_ROUNDUP(size, sizeof(portReg));

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

    portReg           intCtx;
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
