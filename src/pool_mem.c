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

#include "base/pool_mem.h"
#include "base/critical.h"
#include "base/bitop.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Signature for pool memory manager
 */
#define POOL_MEM_SIGNATURE              ((portReg)0xdeadbeeeu)

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
DECL_MODULE_INFO("PoolMem", "Pool Memory management", "Nenad Radulovic");

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

    blockSize = ES_ALIGN_UP(blockSize, sizeof(portReg));

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != poolMem);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != array);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != blockSize);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, blockSize <= arraySize);

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

    ES_DBG_API_OBLIGATION(poolMem->signature = POOL_MEM_SIGNATURE);
}

size_t esPoolMemComputeSize(
    size_t              blocks,
    size_t              blockSize) {

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != blocks);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != blockSize);

    blockSize = ES_ALIGN_UP(blockSize, sizeof(portReg));

    return (blocks * blockSize);
}

void * esPoolMemAllocI(
    esPoolMem *         poolMem) {

    struct poolMemBlock * block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != poolMem);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, POOL_MEM_SIGNATURE == poolMem->signature);

    block = poolMem->sentinel;
    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, NULL != block);
    poolMem->sentinel = block->next;

    return ((void *)block);
}

void * esPoolMemAlloc(
    esPoolMem *         poolMem) {

    portReg             intCtx;
    void *              mem;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    mem = esPoolMemAllocI(
        poolMem);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (mem);
}

void esPoolMemDeAllocI(
    esPoolMem *         poolMem,
    void *              mem) {

    struct poolMemBlock * block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != poolMem);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, POOL_MEM_SIGNATURE == poolMem->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != mem);

    block = (struct poolMemBlock *)mem;
    block->next = poolMem->sentinel;
    poolMem->sentinel = block;
}

void esPoolMemDeAlloc(
    esPoolMem *    handle,
    void *              mem) {

    portReg           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esPoolMemDeAllocI(
        handle,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of pool_mem.c
 ******************************************************************************/
