/*
 * This file is part of esolid-base
 *
 * Template version: 1.1.16 (24.12.2013)
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
#define DMEM_SIGNATURE                  ((portReg)0xDEADBEEFU)

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

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Module information
 */
static DECL_MODULE_INFO("HeapMem", "Heap Memory management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

esHeapMem esGlobalHeapMem;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void esHeapMemInit(
    esHeapMem *    handle,
    void *              storage,
    size_t              storageSize) {

    dMemBlock_T *       begin;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != storage);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != storageSize);

    storageSize = ES_ALIGN(storageSize, sizeof(portReg));
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

esError esHeapMemAllocI(
    void **             mem,
    esHeapMem *         handle,
    size_t              size) {

    dMemBlock_T *       curr;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0u != size);

    size = ES_ALIGN_UP(size, sizeof(portReg)) + sizeof(dMemBlock_T);
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
                curr++;
                *mem = (void *)curr;

                return (ES_ERROR_NONE);
            } else {
                curr->freeNext->freePrev = curr->freePrev;
                curr->freePrev->freeNext = curr->freeNext;
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */
                curr++;
                *mem = (void *)curr;

                return (ES_ERROR_NONE);
            }
        }
        curr = curr->freeNext;
    }

    return (ES_ERROR_MEMORY_FULL);
}

esError esHeapMemAlloc(
    void **             mem,
    esHeapMem *         handle,
    size_t              size) {

    esError             retval;
    portReg             intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    retval = esHeapMemAllocI(
        mem,
        handle,
        size);
    ES_CRITICAL_LOCK_EXIT(intCtx);

    return (retval);
}

void esHeapMemReclaimI(
    esHeapMem *    handle,
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

void esHeapMemReclaim(
    esHeapMem *    handle,
    void *              mem) {

    portReg           intCtx;

    ES_CRITICAL_LOCK_ENTER(&intCtx);
    esHeapMemReclaimI(
        handle,
        mem);
    ES_CRITICAL_LOCK_EXIT(intCtx);
}

#if 0
void esDMemUpdateStatusI(
    esHeapMem *    handle,
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
    esHeapMem *    handle,
    esMemStatus_T *     status) {

    portReg           intCtx;

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
