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
 * @brief       Event Processing Agent implementation
 * @addtogroup  eds_epa
 *********************************************************************//** @{ */
/**@defgroup    eds_epa_impl Implementation
 * @brief       Event Processing Agent Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/port/core.h"
#include "base/shared/component.h"
#include "kernel/mm/mem.h"
#include "eds/epa.h"
#include "eds/event.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void idle_handler(void);

/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Provides the basic information about this module
 */
static const NCOMPONENT_DEFINE("Event Processing Agent", "Nenad Radulovic");

static void (* g_idle)(void) = idle_handler;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void idle_handler(void)
{

}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/



void neds_set_idle(
    void                     (* idle)(void))
{
    g_idle = idle;
}



void neds_run(void)
{
    struct ncore_lock            lock;
    struct nthread *            thread;

    ncore_lock_enter(&lock);

    for (;;) {
        while ((thread = nsched_thread_fetch_i())) {
            struct nepa *           epa;
            const struct nevent *   event;
            naction                 action;

            nsched_thread_remove_i(thread);
            epa   = THREAD_TO_EPA(thread);
            event = nequeue_get(&epa->working_queue);
            ncore_lock_exit(&lock);
            action = nsm_dispatch(&epa->sm, event);
            ncore_lock_enter(&lock);

            if ((action == NACTION_DEFFERED) &&
                !nequeue_is_full(&epa->deffered_queue)) {
                nequeue_put_fifo(&epa->deffered_queue, event);
            } else {
                nevent_destroy_i(event);
            }
        }
        ncore_lock_exit(&lock);
        g_idle();
        ncore_lock_enter(&lock);
    }
    ncore_lock_exit(&lock);
}



void nepa_init(
    struct nepa *               epa,
    const struct nepa_define *  define)
{
    ncore_lock                   sys_lock;

    epa->mem = NULL;
    nequeue_init(&epa->working_queue, &define->working_queue);
    nequeue_put_fifo(&epa->working_queue, nsmp_event(NSMP_INIT));
    nequeue_init(&epa->deffered_queue, &define->deffered_queue);
    nsm_init(&epa->sm, &define->sm);
    nsched_thread_init(&epa->thread, &define->thread);
    ncore_lock_enter(&sys_lock);
    nsched_thread_insert_i(&epa->thread);
    ncore_lock_exit(&sys_lock);

    NOBLIGATION(epa->signature = EPA_SIGNATURE);
}



void nepa_term(
    struct nepa *               epa)
{
    ncore_lock                   sys_lock;

    ncore_lock_enter(&sys_lock);

    while (!nequeue_is_empty(&epa->working_queue)) {
        const struct nevent *   event;

        event = nequeue_get(&epa->working_queue);
        nevent_destroy_i(event);
    }

    while (!nequeue_is_empty(&epa->deffered_queue)) {
        const struct nevent *   event;

        event = nequeue_get(&epa->deffered_queue);
        nevent_destroy_i(event);
    }
    nsched_thread_term(&epa->thread);
    nsm_term(&epa->sm);
    nequeue_term(&epa->deffered_queue);
    nequeue_term(&epa->working_queue);
    ncore_lock_exit(&sys_lock);
}

struct nepa * nepa_create(
    const struct nepa_define *  define,
    struct nmem *               mem)
{
    struct nepa *               epa;
    struct nepa_define          l_define;
    struct nequeue_define       l_working_define;
    struct nequeue_define       l_deffered_define;

    NREQUIRE(NAPI_POINTER, define != NULL);

    epa = nmem_alloc(mem, sizeof(struct nepa));

    if (!epa) {
        goto ERROR_ALLOC_EPA;
    }

    l_working_define.storage = nmem_alloc(mem, define->working_queue.size);
    l_working_define.size    = define->working_queue.size;

    if (!l_working_define.storage) {
        goto ERROR_ALLOC_WORKING_FIFO_BUFF;
    }
    l_deffered_define.storage = NULL;
    l_deffered_define.size    = 0;

    if (define->deffered_queue.size) {
        l_deffered_define.storage = nmem_alloc(mem, define->deffered_queue.size);
        l_deffered_define.size    = define->deffered_queue.size;

        if (!l_deffered_define.storage) {
            goto ERROR_ALLOC_DEFFERED_FIFO_BUFF;
        }
    }
    l_define.sm             = define->sm;
    l_define.thread         = define->thread;
    l_define.working_queue   = l_working_define;
    l_define.deffered_queue  = l_deffered_define;
    nepa_init(epa, &l_define);
    epa->mem = mem;

    return (epa);
ERROR_ALLOC_DEFFERED_FIFO_BUFF:
    nmem_free(mem, l_working_define.storage);
ERROR_ALLOC_WORKING_FIFO_BUFF:
    nmem_free(mem, epa);
ERROR_ALLOC_EPA:

    return (NULL);
}



void nepa_destroy(
    struct nepa *               epa)
{
    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);
    NOBLIGATION(epa->signature = ~EPA_SIGNATURE);

    nepa_term(epa);

    if (nequeue_storage(&epa->deffered_queue)) {
        nmem_free(epa->mem, nequeue_storage(&epa->deffered_queue));
    }
    nmem_free(epa->mem, nequeue_storage(&epa->working_queue));
    nmem_free(epa->mem, epa);
}



nerror nepa_send_event_i(
    struct nepa *               epa,
    struct nevent *             event)
{
    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    if (nevent_ref(event) < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (!nequeue_is_full(&epa->working_queue)) {
            nequeue_put_fifo(&epa->working_queue, event);
            nsched_thread_insert_i(&epa->thread);

            return (NERROR_NONE);
        } else {
            nevent_destroy_i(event);

            return (NERROR_NO_MEMORY);
        }
    }

    return (NERROR_NO_REFERENCE);
}

nerror nepa_send_event(
    struct nepa *               epa,
    struct nevent *             event)
{
    nerror                      error;
    ncore_lock                   sys_lock;

    ncore_lock_enter(&sys_lock);
    error = nepa_send_event_i(epa, event);
    ncore_lock_exit(&sys_lock);

    return (error);
}


nerror nepa_send_event_ahead_i(
    struct nepa *               epa,
    struct nevent *             event)
{
    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    if (event->ref < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (nequeue_is_empty(&epa->working_queue) == true) {
            nequeue_put_lifo(&epa->working_queue, event);
            nsched_thread_insert_i(&epa->thread);

            return (NERROR_NONE);
        } else if (!nequeue_is_full(&epa->working_queue) == false) {
            nequeue_put_lifo(&epa->working_queue, event);

            return (NERROR_NONE);
        } else {
            nevent_destroy_i(event);

            return (NERROR_NO_MEMORY);
        }
    }

    return (NERROR_NO_REFERENCE);
}

nerror nepa_send_event_ahead(
    struct nepa *               epa,
    struct nevent *             event)
{
    nerror                      error;
    ncore_lock                   sys_lock;

    ncore_lock_enter(&sys_lock);
    error = nepa_send_event_ahead_i(epa, event);
    ncore_lock_exit(&sys_lock);

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.c
 ******************************************************************************/
