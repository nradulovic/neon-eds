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
#include "sched/prio_queue.h"
#include "sched/sched.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define NODE_TO_THREAD(node_ptr)                                                \
    PORT_C_CONTAINER_OF(node_ptr, struct nthread, node)

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Scheduler context structure
 * @details     This structure holds important status data for the scheduler.
 */
struct sched_ctx
{
    struct nbias_list *         current;    /**<@brief The current thread     */
    struct nprio_queue          run_queue;  /**<@brief Run queue of threads   */
    bool                        is_initialized;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Scheduler");

static struct sched_ctx         g_sched_ctx;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void
local_dispatcher(struct nthread * thread, ncore_lock * lock)
{
	(void)thread;
    (void)lock;
}



static struct nthread * 
nsched_schedule_i(void) 
{
    struct sched_ctx *          ctx = &g_sched_ctx;
    struct nbias_list *         new_node;
    struct nthread *            thread;

    new_node = nprio_queue_peek(&ctx->run_queue);
    nprio_queue_rotate(&ctx->run_queue, new_node);
    ctx->current = new_node;
    thread       = NODE_TO_THREAD(new_node);

    return (thread);
}




/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nsched_init(
    struct nthread *            thread,
    const struct nthread_define * define)
{
    struct sched_ctx *          ctx = &g_sched_ctx;
    
    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature != NSIGNATURE_THREAD);

    /* Prepare run_queue for usage */
    if (ctx->is_initialized != true) {
        ctx->is_initialized  = true;
        ctx->current         = NULL;
        nprio_queue_init(&ctx->run_queue);     /* Initialize run_queue structure. */
    }
    nbias_list_init(&thread->node, define->priority);
    thread->ref = 0u;
    thread->vf_dispatch_i = local_dispatcher;

#if (CONFIG_REGISTRY == 1)
    memset(thread->name, 0, sizeof(thread->name));

    if (define->name) {
        strncpy(thread->name, define->name, sizeof(thread->name));
    }
    ndlist_init(&thread->registry_node);
#endif /* (CONFIG_REGISTRY == 1) */

    NOBLIGATION(thread->signature = NSIGNATURE_THREAD);
}



void nsched_term(struct nthread * thread)
{
    struct sched_ctx *          ctx = &g_sched_ctx;
    ncore_lock                  sys_lock;

    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature == NSIGNATURE_THREAD);

    ncore_lock_enter(&sys_lock);

    if (thread->ref != 0u) {
        nprio_queue_remove(&ctx->run_queue, &thread->node);
    }
    nbias_list_term(&thread->node);
    ncore_lock_exit(&sys_lock);

    NOBLIGATION(thread->signature = ~NSIGNATURE_THREAD);
}



void nsched_insert_i(struct nthread * thread)
{
    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature == NSIGNATURE_THREAD);

    if (thread->ref == 0u) {
        struct sched_ctx *      ctx = &g_sched_ctx;

        nprio_queue_insert(&ctx->run_queue, &thread->node);
    }
    thread->ref++;
    ncore_os_ready(thread);
}



void nsched_remove_i(struct nthread * thread)
{
    NREQUIRE(NAPI_POINTER, thread != NULL);
    NREQUIRE(NAPI_OBJECT,  thread->signature == NSIGNATURE_THREAD);

    thread->ref--;

    if (thread->ref == 0u) {
        struct sched_ctx *      ctx = &g_sched_ctx;

        nprio_queue_remove(&ctx->run_queue, &thread->node);
    }
    ncore_os_block(thread);
}



void nsched_run(void)
{
    struct ncore_lock           lock;

    ncore_lock_enter(&lock);

    for (;!ncore_os_should_exit();) {
        struct nthread *        thread;
                                   /* Fetch a new thread ready for execution. */
        thread = nsched_schedule_i();
        thread->vf_dispatch_i(thread, &lock);
    }
    ncore_lock_exit(&lock);
}



struct nthread * nsched_get_current(void)
{
    struct sched_ctx *          ctx = &g_sched_ctx;

    return (NODE_TO_THREAD(ctx->current));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of sched.c
 ******************************************************************************/
