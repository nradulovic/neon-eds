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
 * @brief       Pool Memory management implementation
 * @addtogroup  mm_pool
 *********************************************************************//** @{ */
/**@defgroup    mm_pool_impl Implementation
 * @brief       Pool Memory management implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "port/core.h"
#include "base/bitop.h"
#include "mm/pool.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Pool allocator header structure
 */
struct pool_block
{
    struct pool_block *         next;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void * pool_alloc(struct nmem * pool_obj, size_t size);

static void pool_free(struct nmem * pool_obj, void * mem);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void * pool_alloc(struct nmem * pool_obj, size_t size)
{
    struct pool_block *         block;

    NREQUIRE(NSIGNATURE_OF(pool_obj) == NSIGNATURE_POOL);
    NREQUIRE(ncore_is_lock_valid());

    block = pool_obj->base;
    (void)size;

    if (block != NULL) {
        pool_obj->base  = block->next;
        pool_obj->free -= pool_obj->size / pool_obj->no_blocks;
    }
    NENSURE(block);

    return ((void *)block);
}

static void pool_free(struct nmem * pool_obj, void * mem)
{
    struct pool_block *         block;

    NREQUIRE(NSIGNATURE_OF(pool_obj) == NSIGNATURE_POOL);
    NREQUIRE(mem);
    NREQUIRE(ncore_is_lock_valid());

    block           = (struct pool_block *)mem;
    block->next     = pool_obj->base;
    pool_obj->base  = block;
    pool_obj->free += pool_obj->size / pool_obj->no_blocks;
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void * pool_init_alloc(struct nmem * pool_obj, size_t size)
{
    size_t                      block_size;
    size_t                      block_cnt;
    size_t                      nblocks;
    struct pool_block *         block;

    NREQUIRE(NSIGNATURE_OF(pool_obj) == NSIGNATURE_POOL);
    NREQUIRE(pool_obj->base);
    NREQUIRE(pool_obj->size < INT32_MAX);
    NREQUIRE(pool_obj->no_blocks >= 1);

    for (block_cnt = 0u, block_size = pool_obj->size / pool_obj->no_blocks;
        block_cnt < pool_obj->no_blocks - 1u; 
        block_cnt++) {
        block->next =
            (struct pool_block *)((uint8_t *)block + pool_obj->size);
        block = block->next;
    }
    block->next = NULL;

    pool_obj->vf_alloc = pool_alloc;
    pool_obj->vf_free  = pool_free;

    return (pool_alloc(pool_obj, 0));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of pool.c
 ******************************************************************************/
