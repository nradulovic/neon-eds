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
 * @brief       Scheduler implementation
 * @addtogroup  sched_sched
 *********************************************************************//** @{ */
/**@defgroup    sched_sched_impl Implementation
 * @brief       Scheduler Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "port/core.h"
#include "base/debug.h"
#include "base/component.h"
#include "base/bitop.h"
#include "sched/sched.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define NPRIO_ARRAY_BUCKET_BITS                                                 \
    NLOG2_8(NDIVISION_ROUNDUP(CONFIG_PRIORITY_LEVELS, CONFIG_PRIORITY_BUCKETS))

#define NODE_TO_THREAD(node_ptr)                                                \
    PORT_C_CONTAINER_OF(node_ptr, struct nthread, node)

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Priority bitmap structure
 */
struct prio_bitmap
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) || defined(__DOXYGEN__)
                                        /**<@brief Bit group indicator        */
    ncore_reg                    group;
#endif  /* (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
                                        /**<@brief Bucket indicator           */
    ncore_reg                    bit[NDIVISION_ROUNDUP(CONFIG_PRIORITY_BUCKETS,
                                     NCPU_DATA_WIDTH)];
};

/**@brief       Priority queue structure
 * @details     A priority queue consists of an array of sub-queues. There is
 *              one sub-queue per priority level. Each sub-queue contains the
 *              nodes at the corresponding priority level. There is also a
 *              bitmap corresponding to the array that is used to determine
 *              effectively the highest priority node on the queue.
 */
struct prio_queue
{
#if (CONFIG_PRIORITY_BUCKETS != 1)
                                        /**<@brief Priority bitmap            */
    struct prio_bitmap          bitmap;
#endif  /* (CONFIG_PRIORITY_BUCKETS != 1) */
    struct nbias_list           sentinel[CONFIG_PRIORITY_BUCKETS];
};

/**@brief       Scheduler context structure
 * @details     This structure holds important status data for the scheduler.
 */
struct sched_ctx
{
                                        /**<@brief The current thread         */
    struct nbias_list *         current;
                                        /**<@brief Run queue of threads       */
    struct prio_queue           run_queue;
    bool                        is_initialized;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Scheduler");

static struct sched_ctx         g_sched_ctx;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

#if (CONFIG_PRIORITY_BUCKETS != 1)


PORT_C_INLINE void
bitmap_init(struct prio_bitmap * bitmap)
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



PORT_C_INLINE void
bitmap_set(struct prio_bitmap * bitmap, uint_fast8_t priority)
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



PORT_C_INLINE void
bitmap_clear(struct prio_bitmap * bitmap, uint_fast8_t priority)
{
#if   (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH)
    uint_fast8_t                group;
    uint_fast8_t                index;

    index = priority &
        ((uint_fast8_t)~0u >> (sizeof(priority) * 8u - NLOG2_8(NCPU_DATA_WIDTH)));
    group = priority >> NLOG2_8(NCPU_DATA_WIDTH);
    bitmap->bit[group] &= ~ncore_exp2(index);

                                        /* If this is the last bit cleared in */
                                        /* this array_entry then clear bit    */
                                        /* group indicator, too.              */
    if (bitmap->bit[group] == 0u) {
        bitmap->group &= ~ncore_exp2(group);
    }
#else   /*  (CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
    bitmap->bit[0] &= ~ncore_exp2(priority);
#endif  /* !(CONFIG_PRIORITY_BUCKETS > NCPU_DATA_WIDTH) */
}



PORT_C_INLINE uint_fast8_t
bitmap_get_highest(const struct prio_bitmap * bitmap)
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

#endif  /* (CONFIG_PRIORITY_BUCKETS != 1) */



PORT_C_INLINE void
prio_queue_init(struct prio_queue * queue)
{
    uint_fast8_t                count;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bitmap_init(&queue->bitmap);
#endif
    count = NARRAY_DIMENSION(queue->sentinel);

    while (count-- != 0u) {
                                        /* Initialize each list entry.        */
        nbias_list_init(&queue->sentinel[count], NBIAS_LIST_MAX_PRIO);
    }
}



PORT_C_INLINE void
prio_queue_insert(struct prio_queue * queue, struct nbias_list  * node)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbias_list_get_bias(node) >> NPRIO_ARRAY_BUCKET_BITS;
#else
    bucket = 0u;
#endif

#if (CONFIG_PRIORITY_BUCKETS != 1)
                                        /* If adding the first entry in list. */
                                        /* Mark the bucket list as used.      */
    if (nbias_list_is_empty(&queue->sentinel[bucket])) {
        bitmap_set(&queue->bitmap, bucket);
    }
#endif
#if (CONFIG_PRIORITY_BUCKETS != CONFIG_PRIORITY_LEVELS)
                                        /* Priority search and insertion.     */
    nbias_list_sort_insert(&queue->sentinel[bucket], node);
#else
                                        /* FIFO insertion.                    */
    nbias_list_fifo_insert(&queue->sentinel[bucket], node);
#endif
}



PORT_C_INLINE void
prio_queue_remove(struct prio_queue * queue, struct nbias_list * node)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbias_list_get_bias(node) >> NPRIO_ARRAY_BUCKET_BITS;
#else
    bucket = 0u;
#endif
    nbias_list_remove(node);

#if (CONFIG_PRIORITY_BUCKETS != 1)
                                        /* If this was the last node in list. */
                                        /* Mark the bucket as unused.         */
    if (nbias_list_is_empty(&queue->sentinel[bucket])) {
        bitmap_clear(&queue->bitmap, bucket);
    }
#endif
}



PORT_C_INLINE void
prio_queue_rotate(struct prio_queue * queue, struct nbias_list * node)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = nbias_list_get_bias(node) >> NPRIO_ARRAY_BUCKET_BITS;
#else
    bucket = 0u;
#endif
                                        /* Remove node from bucket.           */
    nbias_list_remove(node);
#if (CONFIG_PRIORITY_BUCKETS != CONFIG_PRIORITY_LEVELS)
                                        /* Insert the thread at new position. */
    nbias_list_sort_insert(&queue->sentinel[bucket], node);
#else
                                        /* Insert the thread at new position. */
    nbias_list_fifo_insert(&queue->sentinel[bucket], node);
#endif
}



PORT_C_INLINE struct nbias_list *
prio_queue_peek(const struct prio_queue * queue)
{
    uint_fast8_t                bucket;

#if (CONFIG_PRIORITY_BUCKETS != 1)
    bucket = bitmap_get_highest(&queue->bitmap);
#else
    bucket = 0u;
#endif

    return (nbias_list_next(&queue->sentinel[bucket]));
}



static struct nthread * 
sched_schedule_i(struct sched_ctx * ctx)
{
    struct nbias_list *         new_node;
    struct nthread *            thread;

    new_node = prio_queue_peek(&ctx->run_queue);
    ctx->current = new_node;
    prio_queue_rotate(&ctx->run_queue, new_node);
    thread = NODE_TO_THREAD(new_node);

    return (thread);
}



static void
sched_idle_dispatch_i(struct nthread * thread, struct ncore_lock * lock)
{
    (void)thread;

    ncore_lock_exit(lock);
    ncore_idle();
    ncore_lock_enter(lock);
}



static void
sched_task_dispatch_i(struct nthread * thread, struct ncore_lock * lock)
{
    struct ntask *              task;

    ncore_lock_exit(lock);
    task = PORT_C_CONTAINER_OF(thread, struct ntask, thread);
    task->vf_task(task, task->arg);
    ncore_lock_enter(lock);
}


/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nthread_init(struct nthread * thread, const struct nthread_define * define)
{
    struct sched_ctx *          ctx = &g_sched_ctx;
    
    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature != NSIGNATURE_THREAD);

    /* Prepare run_queue for usage */
    if (ctx->is_initialized != true) {
        struct ncore_lock       lock;

        static const struct nthread_define idle_thread_define =
                NTHREAD_DEF_INIT(sched_idle_dispatch_i, "idle thread", 0);
        static struct nthread       idle_thread;

        ctx->is_initialized  = true;
        ctx->current         = NULL;
        prio_queue_init(&ctx->run_queue); /* Initialize run_queue structure. */
        nthread_init(&idle_thread, &idle_thread_define);
        ncore_lock_enter(&lock);
        nthread_insert_i(&idle_thread);
        ncore_lock_exit(&lock);
    }
    nbias_list_init(&thread->node, define->priority);
    thread->ref = 0u;
    thread->vf_dispatch_i = define->vf_dispatch;

#if (CONFIG_REGISTRY == 1)
    memset(thread->name, 0, sizeof(thread->name));

    if (define->name) {
        strncpy(thread->name, define->name, sizeof(thread->name));
    }
    ndlist_init(&thread->registry_node);
#endif /* (CONFIG_REGISTRY == 1) */

    NOBLIGATION(thread->signature = NSIGNATURE_THREAD);
}



void nthread_term(struct nthread * thread)
{
    struct sched_ctx *          ctx = &g_sched_ctx;
    ncore_lock                  lock;

    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature == NSIGNATURE_THREAD);

    ncore_lock_enter(&lock);

    if (thread->ref != 0u) {
        prio_queue_remove(&ctx->run_queue, &thread->node);
    }
    nbias_list_term(&thread->node);
    ncore_lock_exit(&lock);

    NOBLIGATION(thread->signature = ~NSIGNATURE_THREAD);
}



void nthread_insert_i(struct nthread * thread)
{
    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature == NSIGNATURE_THREAD);

    if (thread->ref == 0u) {
        struct sched_ctx *      ctx = &g_sched_ctx;

        prio_queue_insert(&ctx->run_queue, &thread->node);
    }
    thread->ref++;
    ncore_os_ready(thread);
}



void nthread_remove_i(struct nthread * thread)
{
    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature == NSIGNATURE_THREAD);

    thread->ref--;

    if (thread->ref == 0u) {
        struct sched_ctx *      ctx = &g_sched_ctx;

        prio_queue_remove(&ctx->run_queue, &thread->node);
    }
    ncore_os_block(thread);
}



void nthread_schedule(void)
{
    struct sched_ctx *          ctx = &g_sched_ctx;
    struct ncore_lock           lock;
    struct nthread *            thread;

    ncore_lock_enter(&lock);

    for (;!ncore_os_should_exit();) {
        thread = sched_schedule_i(ctx);  /* Fetch a new thread for execution. */
        thread->vf_dispatch_i(thread, &lock);
    }
    ncore_lock_exit(&lock);
}



struct nthread * nthread_get_current(void)
{
    struct sched_ctx *          ctx = &g_sched_ctx;

    return (NODE_TO_THREAD(ctx->current));
}



void ntask_init(struct ntask * task, const struct ntask_define * define,
        void * arg)
{
    struct nthread_define       thread_define;

    thread_define.name = define->name;
    thread_define.priority = define->priority;
    thread_define.vf_dispatch = sched_task_dispatch_i;

    task->vf_task = define->vf_task;
    task->arg = arg;

    nthread_init(&task->thread, &thread_define);
}



void ntask_ready(struct ntask * task)
{
    struct ncore_lock           lock;

    ncore_lock_enter(&lock);
    nthread_insert_i(&task->thread);
    ncore_lock_exit(&lock);
}



void ntask_block(struct ntask * task)
{
    struct ncore_lock           lock;

    ncore_lock_enter(&lock);
    nthread_remove_i(&task->thread);
    ncore_lock_exit(&lock);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of sched.c
 ******************************************************************************/
