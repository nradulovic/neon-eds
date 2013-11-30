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
 * @brief       Memory Management Implementation
 * @addtogroup  base_mem
 *********************************************************************//** @{ */
/**@defgroup    base_mem_impl Implementation
 * @brief       Memory Management Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/critical.h"
#include "base/bitop.h"
#include "base/mem.h"
#include "arch/cpu.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Signature for static memory manager
 */
#define SMEM_SIGNATURE                  ((portReg_T)0xDEADBEEDU)

/**@brief       Signature for pool memory manager
 */
#define PMEM_SIGNATURE                  ((portReg_T)0xDEADBEEEU)

/**@brief       Signature for dynamic memory manager
 */
#define DMEM_SIGNATURE                  ((portReg_T)0xDEADBEEFU)

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Dynamic allocator memory block header structure
 */
struct PORT_C_ALIGN(PORT_DEF_DATA_ALIGNMENT) dMemBlock {
    size_t              phySize;                                                /**<@brief Block size (including header)                    */
    struct dMemBlock *  phyPrev;                                                /**<@brief Previous block in linked list                    */
    struct dMemBlock *  freeNext;                                               /**<@brief Next free block in linked list                   */
    struct dMemBlock *  freePrev;                                               /**<@brief Previous free block in linked list               */
};

/**@brief       Dynamic allocator memory block header type
 */
typedef struct dMemBlock dMemBlock_T;

/**@brief       Pool allocator header structure
 */
struct pMemBlock {
    struct pMemBlock *  next;                                                   /**<@brief Next free block                                  */
};

/**@brief       Pool allocator header type
 */
typedef struct pMemBlock pMemBlock_T;

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Module information
 */
DECL_MODULE_INFO("MEM", "Memory management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

esSMemHandle_T DefSMemHandle;
esPMemHandle_T DefPMemHandle;
esDMemHandle_T DefDMemHandle;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*--  Static memory manager  -------------------------------------------------*/
void esSMemInit(
    esSMemHandle_T *    handle,
    void *              storage,
    size_t              storageSize) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != storage);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != storageSize);

    handle->begin = storage;
    handle->current = ES_ALIGN(storageSize, sizeof(portReg_T));
    handle->size = ES_ALIGN(storageSize, sizeof(portReg_T));

    ES_DBG_API_OBLIGATION(handle->signature = SMEM_SIGNATURE);
}

void * esSMemAllocI(
    esSMemHandle_T *    handle,
    size_t              size) {

    void *              mem;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, SMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != size);

    size = ES_DIV_ROUNDUP(size, sizeof(portReg_T));
    handle->current -= size;
    mem = (void *)&handle->begin[handle->current];

    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, size <= handle->current);

    return (mem);
}

void * esSMemAlloc(
    esSMemHandle_T *    handle,
    size_t              size) {

    portReg_T           intCtx;
    void *              mem;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    mem = esSMemAllocI(
        handle,
        size);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (mem);
}

void esSMemUpdateStatusI(
    esSMemHandle_T *    handle,
    esMemStatus_T *     status) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, SMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

    status->size = handle->size;
    status->freeSpaceContiguous = handle->current * sizeof(portReg_T);
    status->freeSpaceTotal = status->freeSpaceContiguous;
}

void esSMemUpdateStatus(
    esSMemHandle_T *    handle,
    esMemStatus_T *     status) {

    portReg_T           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esSMemUpdateStatusI(
        handle,
        status);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

/*--  Pool memory manager  ---------------------------------------------------*/
void esPMemInit(
    esPMemHandle_T *    handle,
    void *              array,
    size_t              arraySize,
    size_t              blockSize) {

    size_t              blockCnt;
    size_t              blocks;
    pMemBlock_T *       block;

    blockSize = ES_ALIGN_UP(blockSize, sizeof(portReg_T));

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != array);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != blockSize);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, blockSize <= arraySize);

    blocks = arraySize / blockSize;
    handle->size = arraySize;
    handle->blockSize = blockSize;
    handle->sentinel = (pMemBlock_T *)array;
    block = handle->sentinel;

    for (blockCnt = 0u; blockCnt < blocks - 1u; blockCnt++) {
        block->next = (struct pMemBlock *)((uint8_t *)block + handle->blockSize);
        block = block->next;
    }
    block->next = NULL;

    ES_DBG_API_OBLIGATION(handle->signature = PMEM_SIGNATURE);
}

size_t esPMemCalcPoolSize(
    size_t              blocks,
    size_t              blockSize) {

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != blocks);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != blockSize);

    blockSize = ES_ALIGN_UP(blockSize, sizeof(portReg_T));

    return (blocks * blockSize);
}

void * esPMemAllocI(
    esPMemHandle_T *    handle) {

    pMemBlock_T *       block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, PMEM_SIGNATURE == handle->signature);

    block = handle->sentinel;
    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, NULL != block);
    handle->sentinel = block->next;

    return ((void *)block);
}

void * esPMemAlloc(
    esPMemHandle_T *    handle) {

    portReg_T           intCtx;
    void *              mem;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    mem = esPMemAllocI(
        handle);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (mem);
}

void esPMemDeAllocI(
    esPMemHandle_T *    handle,
    void *              mem) {

    pMemBlock_T *       block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, PMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != mem);

    block = (pMemBlock_T *)mem;
    block->next = handle->sentinel;
    handle->sentinel = block;
}

void esPMemDeAlloc(
    esPMemHandle_T *    handle,
    void *              mem) {

    portReg_T           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esPMemDeAllocI(
        handle,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

void esPMemUpdateStatusI(
    esPMemHandle_T *    handle,
    esMemStatus_T *     status) {

    size_t              freeTotal;
    pMemBlock_T *       block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, PMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

    freeTotal = 0u;
    block     = handle->sentinel;

    while (NULL != block) {
        freeTotal += handle->blockSize;
        block = block->next;
    }
    status->size = handle->size;
    status->freeSpaceTotal = freeTotal;
    status->freeSpaceContiguous = handle->blockSize;
}

void esPMemUpdateStatus(
    esPMemHandle_T *    handle,
    esMemStatus_T *     status) {

    portReg_T           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esPMemUpdateStatusI(
        handle,
        status);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}


/*--  Dynamic memory allocator  ----------------------------------------------*/
void esDMemInit(
    esDMemHandle_T *    handle,
    void *              storage,
    size_t              storageSize) {

    dMemBlock_T *       begin;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != storage);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != storageSize);

    storageSize = ES_ALIGN(storageSize, sizeof(portReg_T));
    handle->sentinel = (dMemBlock_T *)((uint8_t *)storage + storageSize) - 1u;  /* Sentinel is the last element of the storage              */
    begin = (dMemBlock_T *)storage;
    begin->phySize = storageSize - sizeof(dMemBlock_T);
    begin->phyPrev = handle->sentinel;
    begin->freeNext = handle->sentinel;
    begin->freePrev = handle->sentinel;
    handle->sentinel->phySize = 0u;
    handle->sentinel->phyPrev = begin;
    handle->sentinel->freeNext = begin;
    handle->sentinel->freePrev = begin;

    ES_DBG_API_OBLIGATION(handle->signature = DMEM_SIGNATURE);
}

void * esDMemAllocI(
    esDMemHandle_T *    handle,
    size_t              size) {

    dMemBlock_T *       curr;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != size);

    size = ES_ALIGN_UP(size, sizeof(portReg_T)) + sizeof(dMemBlock_T);
    curr = handle->sentinel->freeNext;

    while (curr != handle->sentinel) {

        if (curr->phySize >= size) {

            if (curr->phySize > size + sizeof(dMemBlock_T)) {
                dMemBlock_T * tmp;

                tmp = (dMemBlock_T *)((uint8_t *)curr + size);
                tmp->phySize = curr->phySize - size;
                tmp->phyPrev = curr;
                tmp->freeNext = curr->freeNext;
                tmp->freePrev = curr->freePrev;
                tmp->freeNext->freePrev = tmp;
                tmp->freePrev->freeNext = tmp;
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */

                return ((void *)(curr + 1u));
            } else {
                curr->freeNext->freePrev = curr->freePrev;
                curr->freePrev->freeNext = curr->freeNext;
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */

                return ((void *)(curr + 1u));
            }
        }
        curr = curr->freeNext;
    }
    curr = NULL;

    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, NULL != curr);

    return ((void *)curr);
}

void * esDMemAlloc(
    esDMemHandle_T *    handle,
    size_t              size) {

    portReg_T           intCtx;
    void *              mem;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    mem = esDMemAllocI(
        handle,
        size);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (mem);
}

void esDMemDeAllocI(
    esDMemHandle_T *    handle,
    void *              mem) {

    dMemBlock_T *       curr;
    dMemBlock_T *       tmp;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != mem);

    curr = (dMemBlock_T *)mem - 1u;
    tmp = (dMemBlock_T *)((uint8_t *)curr + curr->phySize);

    if ((NULL != curr->phyPrev->freeNext) && (NULL == tmp->freeNext)) {         /* Previous block is free                                   */
        curr->phyPrev->phySize += curr->phySize;
        tmp->phyPrev = curr->phyPrev;
    } else if ((NULL == curr->phyPrev->freeNext) && (NULL != tmp->freeNext)) {  /* Next block is free                                     */
        curr->freeNext = tmp->freeNext;
        curr->freePrev = tmp->freePrev;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
        curr->phySize += tmp->phySize;
        tmp = (dMemBlock_T *)((uint8_t *)curr + curr->phySize);
        tmp->phyPrev = curr;
    } else if ((NULL != curr->phyPrev->freeNext) && (NULL != tmp->freeNext)) {  /* Previous and next blocks are free                      */
        tmp->freePrev->freeNext = tmp->freeNext;
        tmp->freeNext->freePrev = tmp->freePrev;
        curr->phyPrev->phySize += curr->phySize + tmp->phySize;
        tmp = (dMemBlock_T *)((uint8_t *)curr->phyPrev + curr->phyPrev->phySize);
        tmp->phyPrev = curr->phyPrev;
    } else {                                                                    /* Previous and next blocks are allocated                   */
        curr->freeNext = handle->sentinel->freeNext;
        curr->freePrev = handle->sentinel;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
    }
}

void esDMemDeAlloc(
    esDMemHandle_T *    handle,
    void *              mem) {

    portReg_T           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esDMemDeAllocI(
        handle,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

void esDMemUpdateStatusI(
    esDMemHandle_T *    handle,
    esMemStatus_T *     status) {

    size_t              size;
    size_t              freeTotal;
    size_t              freeAvailable;
    dMemBlock_T *       curr;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

    size          = 0u;
    freeTotal     = 0u;
    freeAvailable = 0u;
    curr          = handle->sentinel->phyPrev;

    while (curr != handle->sentinel) {
        size += curr->phySize;

        if (NULL != curr->freeNext) {
            size_t freeSize;

            freeSize = curr->phySize - sizeof(dMemBlock_T);
            freeTotal += freeSize;

            if (freeSize > freeAvailable) {
                freeAvailable = freeSize;
            }
        }
        curr = curr->phyPrev;
    }
    status->size = size;
    status->freeSpaceTotal = freeTotal;
    status->freeSpaceContiguous = freeAvailable;
}

void esDMemUpdateStatus(
    esDMemHandle_T *    handle,
    esMemStatus_T *     status) {

    portReg_T           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esDMemUpdateStatusI(
        handle,
        status);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of mem.c
 ******************************************************************************/
