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
 * @brief       Heap Memory Management Implementation
 * @addtogroup  base_heap_mem
 *********************************************************************//** @{ */
/**@defgroup    base_heap_mem_impl Implementation
 * @brief       Heap Memory Management Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/heap_mem.h"
#include "base/critical.h"
#include "base/bitop.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Signature for dynamic memory manager
 */
#define HEAP_MEM_SIGNATURE              ((esAtomic)0xdeadbeefu)

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Dynamic allocator memory block header structure
 */
struct PORT_C_ALIGN(PORT_DEF_DATA_ALIGNMENT) heapMemBlock {
    size_t              phySize;                                                /**<@brief Block size (including header)                    */
    struct heapMemBlock *  phyPrev;                                                /**<@brief Previous block in linked list                    */
    struct heapMemBlock *  freeNext;                                               /**<@brief Next free block in linked list                   */
    struct heapMemBlock *  freePrev;                                               /**<@brief Previous free block in linked list               */
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Module information
 */
static ES_MODULE_INFO("HeapMem", "Heap Memory management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

esHeapMem esGlobalHeapMem;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esHeapMemInit(
    esHeapMem *         heapMem,
    void *              storage,
    size_t              storageSize) {

    struct heapMemBlock * begin;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, heapMem != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  heapMem->signature != HEAP_MEM_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, storage != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_RANGE,   storageSize != 0u);

    storageSize = ES_ALIGN(storageSize, sizeof(esAtomic));
    heapMem->sentinel =
        (struct heapMemBlock *)((uint8_t *)storage + storageSize) - 1;          /* Sentinel is the last element of the storage              */
    begin = (struct heapMemBlock *)storage;
    begin->phySize = storageSize - sizeof(struct heapMemBlock);
    begin->phyPrev = heapMem->sentinel;
    begin->freeNext = heapMem->sentinel;
    begin->freePrev = heapMem->sentinel;
    heapMem->sentinel->phySize = 0u;
    heapMem->sentinel->phyPrev = begin;
    heapMem->sentinel->freeNext = begin;
    heapMem->sentinel->freePrev = begin;

    ES_DEBUG_API_OBLIGATION(heapMem->signature = HEAP_MEM_SIGNATURE);

    return (ES_ERROR_NONE);
}

esError esHeapMemTerm(
    esHeapMem *         heapMem) {

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, heapMem != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  heapMem->signature == HEAP_MEM_SIGNATURE);

    heapMem->sentinel = NULL;

    ES_DEBUG_API_OBLIGATION(heapMem->signature = ~HEAP_MEM_SIGNATURE);

    return (ES_ERROR_NONE);
}

esError esHeapMemAllocI(
    esHeapMem *         heapMem,
    size_t              size,
    void **             mem) {

    struct heapMemBlock * curr;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, heapMem != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  heapMem->signature == HEAP_MEM_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_USAGE,   size != 0u);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, mem != NULL);

    size = ES_ALIGN_UP(size, sizeof(esAtomic));
    curr = heapMem->sentinel->freeNext;

    while (curr != heapMem->sentinel) {

        if (curr->phySize >= size) {

            if (curr->phySize > (size + sizeof(struct heapMemBlock))) {
                struct heapMemBlock * tmp;

                tmp = (struct heapMemBlock *)((uint8_t *)curr + size);
                tmp->phySize = curr->phySize - size;
                tmp->phyPrev = curr;
                tmp->freeNext = curr->freeNext;
                tmp->freePrev = curr->freePrev;
                tmp->freeNext->freePrev = tmp;
                tmp->freePrev->freeNext = tmp;
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */
                *mem = (void *)(curr + 1);

                return (ES_ERROR_NONE);
            } else {
                curr->freeNext->freePrev = curr->freePrev;
                curr->freePrev->freeNext = curr->freeNext;
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */
                *mem = (void *)(curr + 1);

                return (ES_ERROR_NONE);
            }
        }
        curr = curr->freeNext;
    }

    return (ES_ERROR_MEMORY_FULL);
}

esError esHeapMemAlloc(
    esHeapMem *         heapMem,
    size_t              size,
    void **             mem) {

    esError             error;
    esAtomic            intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    error = esHeapMemAllocI(
        heapMem,
        size,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (error);
}

esError esHeapMemFreeI(
    esHeapMem *         heapMem,
    void *              mem) {

    struct heapMemBlock * curr;
    struct heapMemBlock * tmp;

    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, heapMem != NULL);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_OBJECT,  heapMem->signature == HEAP_MEM_SIGNATURE);
    ES_DEBUG_API_REQUIRE(ES_DEBUG_POINTER, mem != NULL);

    curr = (struct heapMemBlock *)mem - 1;
    tmp  = (struct heapMemBlock *)((uint8_t *)curr + curr->phySize);

    if ((curr->phyPrev->freeNext != NULL) && (tmp->freeNext == NULL)) {         /* Previous block is free                                   */
        curr->phyPrev->phySize += curr->phySize;
        tmp->phyPrev = curr->phyPrev;
    } else if ((curr->phyPrev->freeNext == NULL) && (tmp->freeNext != NULL)) {  /* Next block is free                                     */
        curr->freeNext = tmp->freeNext;
        curr->freePrev = tmp->freePrev;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
        curr->phySize += tmp->phySize;
        tmp = (struct heapMemBlock *)((uint8_t *)curr + curr->phySize);
        tmp->phyPrev = curr;
    } else if ((curr->phyPrev->freeNext != NULL) && (tmp->freeNext != NULL)) {  /* Previous and next blocks are free                      */
        tmp->freePrev->freeNext = tmp->freeNext;
        tmp->freeNext->freePrev = tmp->freePrev;
        curr->phyPrev->phySize += curr->phySize + tmp->phySize;
        tmp = (struct heapMemBlock *)
            ((uint8_t *)curr->phyPrev + curr->phyPrev->phySize);
        tmp->phyPrev = curr->phyPrev;
    } else {                                                                    /* Previous and next blocks are allocated                   */
        curr->freeNext = heapMem->sentinel->freeNext;
        curr->freePrev = heapMem->sentinel;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
    }

    return (ES_ERROR_NONE);
}

esError esHeapMemFree(
    esHeapMem *         heapMem,
    void *              mem) {

    esError             error;
    esAtomic            intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    error = esHeapMemFreeI(
        heapMem,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (error);
}

#if 0
void esDMemUpdateStatusI(
    esHeapMem *    handle,
    esMemStatus_T *     status) {

    size_t              size;
    size_t              freeTotal;
    size_t              freeAvailable;
    struct heapMemBlock *       curr;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, HEAP_MEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

    size          = 0u;
    freeTotal     = 0u;
    freeAvailable = 0u;
    curr          = handle->sentinel->phyPrev;

    while (curr != handle->sentinel) {
        size += curr->phySize;

        if (NULL != curr->freeNext) {
            size_t freeSize;

            freeSize = curr->phySize - sizeof(struct heapMemBlock);
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
    esHeapMem *    handle,
    esMemStatus_T *     status) {

    esAtomic           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esDMemUpdateStatusI(
        handle,
        status);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of heap_mem.c
 ******************************************************************************/
