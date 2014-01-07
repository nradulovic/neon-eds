/*
 * This file is part of eSolid
 *
 * Copyright (C) 2011 - 2013 - Nenad Radulovic
 *
 * eSolid is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with eSolid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Pool Memory management Implementation
 * @addtogroup  base_pool_mem
 *********************************************************************//** @{ */
/**@defgroup    base_pool_mem_impl Implementation
 * @brief       Pool Memory management Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "base/pool_mem.h"
#include "base/critical.h"
#include "base/bitop.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Signature for pool memory manager
 */
#define POOL_MEM_SIGNATURE              ((esAtomic)0xdeadbeeeu)

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Pool allocator header structure
 */
struct poolMemBlock {
    struct poolMemBlock * next;                                                 /**<@brief Next free block                                  */
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Module information
 */
static ES_MODULE_INFO("PoolMem", "Pool Memory management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

esPoolMem esGlobalPoolMem;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void esPoolMemInit(
    esPoolMem *         poolMem,
    void *              array,
    size_t              arraySize,
    size_t              blockSize) {

    size_t              blockCnt;
    size_t              nBlocks;
    struct poolMemBlock * block;

    blockSize = ES_ALIGN_UP(blockSize, sizeof(esAtomic));

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, NULL != poolMem);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, NULL != array);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE, 0u != blockSize);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE, blockSize <= arraySize);

    nBlocks = arraySize / blockSize;
    poolMem->size = arraySize;
    poolMem->blockSize = blockSize;
    poolMem->sentinel = (struct poolMemBlock *)array;
    block = poolMem->sentinel;

    for (blockCnt = 0u; blockCnt < nBlocks - 1u; blockCnt++) {
        block->next = (struct poolMemBlock *)((uint8_t *)block + poolMem->blockSize);
        block = block->next;
    }
    block->next = NULL;

    ES_DEBUG_API_OBLIGATION(poolMem->signature = POOL_MEM_SIGNATURE);
}

size_t esPoolMemComputeSize(
    size_t              blocks,
    size_t              blockSize) {

    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE, 0u != blocks);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE, 0u != blockSize);

    blockSize = ES_ALIGN_UP(blockSize, sizeof(esAtomic));

    return (blocks * blockSize);
}

esError esPoolMemAllocI(
    esPoolMem *         poolMem,
    void **             mem) {

    struct poolMemBlock * block;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, poolMem != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  poolMem->signature == POOL_MEM_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, mem != NULL);

    if ((block = poolMem->sentinel) == NULL) {

        return (ES_ERROR_MEMORY_FULL);
    }
    poolMem->sentinel = block->next;
    *mem = (void *)block;

    return (ES_ERROR_NONE);
}

esError esPoolMemAlloc(
    esPoolMem *         poolMem,
    void **             mem) {

    esError             error;
    esAtomic             intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    error = esPoolMemAllocI(
        poolMem,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (error);
}

void esPoolMemFreeI(
    esPoolMem *         poolMem,
    void *              mem) {

    struct poolMemBlock * block;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, poolMem != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  poolMem->signature == POOL_MEM_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, mem != NULL);

    block = (struct poolMemBlock *)mem;
    block->next = poolMem->sentinel;
    poolMem->sentinel = block;
}

void esPoolMemFree(
    esPoolMem *    handle,
    void *              mem) {

    esAtomic           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esPoolMemFreeI(
        handle,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of pool_mem.c
 ******************************************************************************/
