/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2017 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Heap Memory management implementation
 * @addtogroup  mm_heap
 *********************************************************************//** @{ */
/**@defgroup    mm_heap_impl Implementation
 * @brief       Heap Memory management implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "port/core.h"
#include "base/component.h"
#include "base/debug.h"
#include "base/bitop.h"
#include "mm/heap.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Validate the pointer to heap memory object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#define N_IS_HEAP_OBJECT(mem_obj)                                               \
    (((mem_obj) != NULL) && ((mem_obj)->signature == NSIGNATURE_HEAP))

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Dynamic allocator memory block header structure
 */
struct PORT_C_ALIGN(NCPU_DATA_ALIGNMENT) heap_block
{
    struct heap_phy
    {
        struct heap_block *         prev;
        ncpu_ssize                  size;
    }                           phy;
    struct heap_free
    {
        struct heap_block *         next;
        struct heap_block *         prev;
    }                           free;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void * heap_alloc_i(struct nmem * mem_obj, size_t size);

static void heap_free_i(struct nmem * mem_obj, void * mem);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Heap Memory Management");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static void * heap_alloc_i(struct nmem * mem_obj, size_t size)
{
    struct heap_block *         sentinel;
    struct heap_block *         curr;
    void *                      mem;

    NREQUIRE(NAPI_OBJECT, N_IS_HEAP_OBJECT(mem_obj));
    NREQUIRE(NAPI_RANGE, (size != 0u) && (size < NCPU_SSIZE_MAX));
    NREQUIRE(NAPI_USAGE, ncore_is_lock_valid());

    mem      = NULL;
    size     = NALIGN_UP(size, sizeof(struct heap_phy [1]));
    sentinel = mem_obj->base;
    curr     = sentinel->free.next;

    while (curr != sentinel) {

        if (curr->phy.size >= (ncpu_ssize)size) {

            if (curr->phy.size >
                (ncpu_ssize)(size + sizeof(struct heap_block [1]))) {
                struct heap_block * tmp;
                                       /* Create smaller free block           */
                mem            = (void *)(&curr->free);
                                       /* Point back to the current block     */
                tmp            = (struct heap_block *)
                    ((uint8_t *)curr + size + sizeof(struct heap_phy [1]));
                tmp->phy.prev  = curr;
                tmp->phy.size  = curr->phy.size;
                tmp->phy.size -= (ncpu_ssize)size;
                tmp->phy.size -= (ncpu_ssize)sizeof(struct heap_phy [1]);
                                       /* Mark block as allocated             */
                curr->phy.size = (ncpu_ssize)size * (-1);
                                       /* Remove current and add smaller free */
                                       /* block back to free list             */
                tmp->free.next = curr->free.next;
                tmp->free.prev = curr->free.prev;
                tmp->free.next->free.prev = tmp;
                tmp->free.prev->free.next = tmp;
                curr           = (struct heap_block *)
                    ((uint8_t *)tmp + tmp->phy.size + 
                     sizeof(struct heap_phy [1]));
                                       /* Point to the newly created block    */
                curr->phy.prev = tmp;

                break;
            } else {
                                      /* Remove current block from free list  */
                                      /* and mark it block as allocated       */
                mem                        = (void *)(&curr->free);
                curr->free.next->free.prev = curr->free.prev;
                curr->free.prev->free.next = curr->free.next;
                curr->phy.size             = curr->phy.size * (-1);

                break;
            }
        }
        curr = curr->free.next;
    }

    NENSURE("heap memory not allocated", mem != NULL);

    return (mem);
}


static void heap_free_i(struct nmem * mem_obj, void * mem)
{
    struct heap_block *         curr;
    struct heap_block *         tmp;

    NREQUIRE(NAPI_OBJECT, N_IS_HEAP_OBJECT(mem_obj));
    NREQUIRE(NAPI_POINTER, mem != NULL);
    NREQUIRE(NAPI_USAGE, ncore_is_lock_valid());

    curr           = (struct heap_block *)
        ((uint8_t *)mem - offsetof(struct heap_block, free));
    curr->phy.size = (ncpu_ssize)curr->phy.size * (-1); /* Mark block as free */
    tmp            = (struct heap_block *)
        ((uint8_t *)curr + curr->phy.size + sizeof(struct heap_phy [1]));

                                                    /* Previous block is free */
    if ((curr->phy.prev->phy.size > 0) && (tmp->phy.size < 0)) {                
        curr->phy.prev->phy.size  += curr->phy.size;
        curr->phy.prev->phy.size  += (ncpu_ssize)sizeof(struct heap_phy [1]);
        tmp->phy.prev              = curr->phy.prev;
                                                        /* Next block is free */
    } else if ((curr->phy.prev->phy.size < 0) && (tmp->phy.size > 0)) {         
        curr->free.next            = tmp->free.next;
        curr->free.prev            = tmp->free.prev;
        curr->free.prev->free.next = curr;
        curr->free.next->free.prev = curr;
        curr->phy.size            += tmp->phy.size;
        curr->phy.size            += (ncpu_ssize)sizeof(struct heap_phy [1]);
        tmp                        = (struct heap_block *)
            ((uint8_t *)curr + curr->phy.size + sizeof(struct heap_phy [1]));
        tmp->phy.prev              = curr;
                                         /* Previous and next blocks are free */
    } else if ((curr->phy.prev->phy.size > 0) && (tmp->phy.size > 0)) {         
        tmp->free.prev->free.next  = tmp->free.next;
        tmp->free.next->free.prev  = tmp->free.prev;
        curr->phy.prev->phy.size  += curr->phy.size + tmp->phy.size;
        curr->phy.prev->phy.size  += (ncpu_ssize)sizeof(struct heap_phy [2]);
        tmp                        = (struct heap_block *)
            ((uint8_t *)curr->phy.prev + curr->phy.prev->phy.size +
            sizeof(struct heap_phy [1]));
        tmp->phy.prev              = curr->phy.prev;
                                      /* Previous and next blocks are used    */
    } else {                                                                    
        struct heap_block *     sentinel = mem_obj->base;

        curr->free.next            = sentinel->free.next;
        curr->free.prev            = sentinel;
        curr->free.prev->free.next = curr;
        curr->free.next->free.prev = curr;
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void nheap_init(struct nheap * heap_obj, void * storage, size_t size)
{
    struct heap_block *         sentinel;
    struct heap_block *         begin;

    NREQUIRE(NAPI_POINTER, heap_obj != NULL);
    NREQUIRE(NAPI_OBJECT,  heap_obj->mem_class.signature != NSIGNATURE_HEAP);
    NREQUIRE(NAPI_POINTER, storage != NULL);
    NREQUIRE(NAPI_RANGE,   size > sizeof(struct heap_block [2]));
    NREQUIRE(NAPI_RANGE,   size < NCPU_SSIZE_MAX);

    size = NALIGN(size, NCPU_DATA_ALIGNMENT);
                                            /* Sentinel is the last element   */
    sentinel = (struct heap_block *)((uint8_t *)storage + size) - 1;
    begin    = (struct heap_block *)storage;
    begin->phy.size  = (ncpu_ssize)size;
    begin->phy.size -= (ncpu_ssize)sizeof(struct heap_block [1]);
    begin->phy.size -= (ncpu_ssize)sizeof(struct heap_phy [1]);
    begin->phy.prev  = sentinel;
    begin->free.next = sentinel;
    begin->free.prev = sentinel;

    sentinel->phy.size   = -1;
    sentinel->phy.prev   = begin;
    sentinel->free.next  = begin;
    sentinel->free.prev  = begin;
    heap_obj->mem_class.base = sentinel;
    heap_obj->mem_class.size = (size_t)begin->phy.size;
    heap_obj->mem_class.free = (size_t)begin->phy.size;
    heap_obj->mem_class.vf_alloc = heap_alloc_i;
    heap_obj->mem_class.vf_free  = heap_free_i;

    NOBLIGATION(heap_obj->mem_class.signature = NSIGNATURE_HEAP);
}



void nheap_term(struct nheap * heap_obj)
{
    NREQUIRE(NAPI_POINTER, heap_obj != NULL);
    NREQUIRE(NAPI_OBJECT,  heap_obj->mem_class.signature == NSIGNATURE_HEAP);

    heap_obj->mem_class.base = NULL;

    NOBLIGATION(heap_obj->mem_class.signature = ~NSIGNATURE_HEAP);
}



void * nheap_alloc_i(struct nheap * heap_obj, size_t size)
{
    return (heap_alloc_i(&heap_obj->mem_class, size));
}



void * nheap_alloc(struct nheap * heap, size_t size)
{
    ncore_lock                  sys_lock;
    void *                      mem;

    ncore_lock_enter(&sys_lock);
    mem = heap_alloc_i(&heap->mem_class, size);
    ncore_lock_exit(&sys_lock);

    return (mem);
}



void nheap_free_i(struct nheap * heap, void * mem)
{
    heap_free_i(&heap->mem_class, mem);
}



void nheap_free(struct nheap * heap, void * mem)
{
    ncore_lock                   sys_lock;

    ncore_lock_enter(&sys_lock);
    heap_free_i(&heap->mem_class, mem);
    ncore_lock_exit(&sys_lock);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of heap_mem.c
 ******************************************************************************/
