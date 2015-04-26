/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
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
 * @brief       Priority queue header
 * @defgroup    priority_queue Priority queue
 * @brief       Priority queue
 *********************************************************************//** @{ */

#ifndef NEON_SCHED_PRIO_QUEUE_H
#define NEON_SCHED_PRIO_QUEUE_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

#include "port/compiler.h"
#include "port/core.h"
#include "base/bitop.h"
#include "base/config.h"
#include "base/bias_list.h"

/*===============================================================  MACRO's  ==*/

#define NPRIO_ARRAY_BUCKET_BITS                                                 \
    NLOG2_8(NDIVISION_ROUNDUP(CONFIG_PRIORITY_LEVELS, CONFIG_PRIORITY_BUCKETS))

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Priority queue structure
 * @details     A priority queue consists of an array of sub-queues. There is
 *              one sub-queue per priority level. Each sub-queue contains the
 *              nodes at the corresponding priority level. There is also a
 *              bitmap corresponding to the array that is used to determine
 *              effectively the highest priority node on the queue.
 * @api
 */
struct nprio_queue
{
#if (CONFIG_PRIORITY_BUCKETS != 1)
    /**@brief       Priority bitmap structure
     * @notapi
     */
    struct nprio_bitmap
    {
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) || defined(__DOXYGEN__)
        ncore_reg                    group;                                     /**<@brief Bit group indicator        */
#endif  /* (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
        ncore_reg                    bit[NDIVISION_ROUNDUP(
                                         CONFIG_PRIORITY_BUCKETS,
                                         NCPU_DATA_WIDTH)];                     /**<@brief Bucket indicator           */
    }                           bitmap;                                         /**<@brief Priority bitmap            */
#endif  /* (CONFIG_PRIORITY_BUCKETS != 1) */
    struct nbias_list           sentinel[CONFIG_PRIORITY_BUCKETS];
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

#if (CONFIG_PRIORITY_BUCKETS != 1)


PORT_C_INLINE
void nbitmap_init(
    struct nprio_bitmap *       bitmap)
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH)
    uint_fast8_t                group;

    bitmap->group = 0u;
    group = NARRAY_DIMENSION(bitmap->bit);

    while (group-- != 0u) {
        bitmap->bit[group] = 0u;
    }
#else   /*  (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
    bitmap->bit[0] = 0u;
#endif  /* !(CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
}



PORT_C_INLINE
void nbitmap_set(
    struct nprio_bitmap *       bitmap,
    uint_fast8_t                priority)
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH)
    uint_fast8_t                group;
    uint_fast8_t                index;

    index = priority &
        ((uint_fast8_t)~0u >> (sizeof(priority) * 8u - NLOG2_8(NCPU_DATA_WIDTH)));
    group = priority >> NLOG2_8(NCPU_DATA_WIDTH);
    bitmap->group      |= ncore_exp2(group);
    bitmap->bit[group] |= ncore_exp2(index);
#else   /*  (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
    bitmap->bit[0] |= ncore_exp2(priority);
#endif  /* !(CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
}



PORT_C_INLINE
void nbitmap_clear(
    struct nprio_bitmap *       bitmap,
    uint_fast8_t                priority)
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH)
    uint_fast8_t                group;
    uint_fast8_t                index;

    index = priority &
        ((uint_fast8_t)~0u >> (sizeof(priority) * 8u - NLOG2_8(NCPU_DATA_WIDTH)));
    group = priority >> NLOG2_8(NCPU_DATA_WIDTH);
    bitmap->bit[group] &= ~ncore_exp2(index);

    if (bitmap->bit[group] == 0u) {                                             /* If this is the last bit cleared in */
        bitmap->group &= ~ncore_exp2(group);                                     /* this array_entry then clear bit    */
    }                                                                           /* group indicator, too.              */
#else   /*  (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
    bitmap->bit[0] &= ~ncore_exp2(priority);
#endif  /* !(CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
}



PORT_C_INLINE
uint_fast8_t nbitmap_get_highest(
    const struct nprio_bitmap * bitmap)
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH)
    uint_fast8_t                group;
    uint_fast8_t                index;

    group = ncore_log2(bitmap->group);
    index = ncore_log2(bitmap->bit[group]);

    return ((group << NLOG2_8(NCPU_DATA_WIDTH)) | index);
#else   /*  (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
    uint_fast8_t                index;

    index = ncore_log2(bitmap->bit[0]);

    return (index);
#endif  /* !(CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
}



PORT_C_INLINE
bool nbitmap_is_empty(
    const struct nprio_bitmap * bitmap)
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH)
    if (bitmap->group == 0u) {
        return (true);
    } else {
        return (false);
    }
#else   /*  (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
    if (bitmap->bit[0] == 0u) {
        return (true);
    } else {
        return (false);
    }
#endif  /* !(CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
}
#endif  /* (CONFIG_PRIORITY_BUCKETS != 1) */



PORT_C_INLINE
void nprio_queue_init(
    struct nprio_queue *        queue)
{
    uint_fast8_t                count;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    nbitmap_init(&queue->bitmap);
#endif
    count = NARRAY_DIMENSION(queue->sentinel);

    while (count-- != 0u) {                                                     /* Initialize each list entry.        */
        nbias_list_init(&queue->sentinel[count], NBIAS_LIST_MAX_PRIO);
    }
}



PORT_C_INLINE
void nprio_queue_insert(
    struct nprio_queue *        queue,
    struct nbias_list  *        node)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbias_list_get_bias(node) >> NPRIO_ARRAY_BUCKET_BITS;
#else
    bucket = 0u;
#endif

#if (CONFIG_PRIORITY_BUCKETS != 1)
    if (nbias_list_is_empty(&queue->sentinel[bucket])) {                        /* If adding the first entry in list. */
        nbitmap_set(&queue->bitmap, bucket);                                    /* Mark the bucket list as used.      */
    }
#endif
#if (CONFIG_PRIORITY_BUCKETS != CONFIG_PRIORITY_LEVELS)
    nbias_list_sort_insert(&queue->sentinel[bucket], node);                     /* Priority search and insertion.     */
#else
    nbias_list_fifo_insert(&queue->sentinel[bucket], node);                     /* FIFO insertion.                    */
#endif
}



PORT_C_INLINE
void nprio_queue_remove(
    struct nprio_queue *        queue,
    struct nbias_list  *        node)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbias_list_get_bias(node) >> NPRIO_ARRAY_BUCKET_BITS;
#else
    bucket = 0u;
#endif
    nbias_list_remove(node);

#if (CONFIG_PRIORITY_BUCKETS != 1)
    if (nbias_list_is_empty(&queue->sentinel[bucket])) {                        /* If this was the last node in list. */
        nbitmap_clear(&queue->bitmap, bucket);                                  /* Mark the bucket as unused.         */
    }
#endif
}



PORT_C_INLINE
void nprio_queue_rotate(
    struct nprio_queue *        queue,
    struct nbias_list  *        node)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbias_list_get_bias(node) >> NPRIO_ARRAY_BUCKET_BITS;
#else
    bucket = 0u;
#endif

    nbias_list_remove(node);                                                    /* Remove node from bucket.           */
#if (CONFIG_PRIORITY_BUCKETS != CONFIG_PRIORITY_LEVELS)
    nbias_list_sort_insert(&queue->sentinel[bucket], node);                     /* Insert the thread at new position. */
#else
    nbias_list_fifo_insert(&queue->sentinel[bucket], node);                     /* Insert the thread at new position. */
#endif
}



PORT_C_INLINE
struct nbias_list * nprio_queue_peek(
    const struct nprio_queue *  queue)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbitmap_get_highest(&queue->bitmap);
#else
    bucket = 0u;
#endif

    return (nbias_list_next(&queue->sentinel[bucket]));
}



PORT_C_INLINE
bool nprio_queue_is_empty(
    const struct nprio_queue *  queue)
{
#if (CONFIG_PRIORITY_BUCKETS != 1)
    return (nbitmap_is_empty(&queue->bitmap));
#else
    return (nbias_list_is_empty(&queue->sentinel[0]));
#endif
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of nprio_queue.h
 ******************************************************************************/
#endif /* NEON_SCHED_PRIO_QUEUE_H */
