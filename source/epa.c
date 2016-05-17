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
 * @addtogroup  ep_epa
 *********************************************************************//** @{ */
/**@defgroup    ep_epa_impl Implementation
 * @brief       Event Processing Agent Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "port/core.h"
#include "base/component.h"
#include "mm/mem.h"
#include "ep/epa.h"
#include "ep/event.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Provides the basic information about this module
 */
static const NCOMPONENT_DEFINE("Event Processing Agent");

static void (* g_idle)(void) = ncore_idle;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void neds_set_idle(void (* idle)(void))
{
    if (idle) {
        g_idle = idle;
    } else {
        g_idle = ncore_idle;
    }
}



void * nepa_create_storage(size_t size)
{
    struct nepa *               epa;
    void *                      storage;

    epa = nepa_get_current();

    NASSERT_INTERNAL(NAPI_OBJECT, N_IS_MEM_OBJECT(epa->mem));

    storage = nmem_zalloc(epa->mem, size);

    return (storage);
}



void nepa_delete_storage(void * storage)
{
    struct nepa *               epa;

    epa = nepa_get_current();

    NASSERT_INTERNAL(NAPI_OBJECT, N_IS_MEM_OBJECT(epa->mem));

    nmem_free(epa->mem, storage);
}



nerror nepa_defer_event(struct nequeue * queue, const struct nevent * event)
{
    struct ncore_lock           lock;
    nerror                      error;

    error = NERROR_NO_RESOURCE;

    ncore_lock_enter(&lock);
    if (!nequeue_is_full(queue)) {
        nequeue_put_fifo(queue, event);
        nevent_ref_up(event);
        error = NERROR_NONE;
    }
    ncore_lock_exit(&lock);

    return (error);
}



nerror nepa_defer_fetch_one(struct nequeue * queue)
{
    struct ncore_lock           lock;
    struct nepa *               epa;
    nerror                      error;

    epa   = nepa_get_current();
    error = NERROR_NONE;
    
    ncore_lock_enter(&lock);

    if (!nequeue_is_empty(queue)) {
        const struct nevent *   event;
        
        event = nequeue_get(queue);
        nevent_ref_down(event);
        error = nepa_send_event_i(epa, event);
    }
    ncore_lock_exit(&lock);
    
    return (error);
}



nerror nepa_defer_fetch_all(struct nequeue * queue)
{
    struct ncore_lock           lock;
    struct nepa *               epa;
    nerror                      error;

    epa   = nepa_get_current();
    error = NERROR_NONE;
    
    ncore_lock_enter(&lock);

    while (!error && !nequeue_is_empty(queue)) {
        const struct nevent *   event;
        
        event = nequeue_get(queue);
        nevent_ref_down(event);
        error = nepa_send_event_i(epa, event);
    }
    ncore_lock_exit(&lock);
    
    return (error);
}



void neds_run(void)
{
    struct ncore_lock           lock;
    struct nthread *            thread;

    ncore_lock_enter(&lock);

    for (;ncore_os_should_exit();) {
                                   /* Fetch a new thread ready for execution. */
        while ((thread = nsched_schedule_i())) {
            struct nepa *           epa;
            const struct nevent *   event;

            epa   = NTHREAD_TO_EPA(thread);                /* Get EPA pointer */
            event = nequeue_get(&epa->working_queue);    /* Get Event pointer */
            ncore_lock_exit(&lock);
            /* ************************************************************** *
             * NOTE: Dispatch the state machine. This is a good place to      *
             * place a breakpoint when debugging state machines.              *
             * ************************************************************** */
            nsm_dispatch(&epa->sm, event);
            ncore_lock_enter(&lock);
            nevent_ref_down(event);
            nevent_destroy_i(event);
            nsched_thread_remove_i(thread);               /* Block the thread */
        }
        ncore_lock_exit(&lock);
        g_idle();
        ncore_lock_enter(&lock);
    }
    ncore_lock_exit(&lock);
}



void neds_term(void)
{
	ncore_os_exit();
    ncore_term();
}


void nepa_init(struct nepa * epa, const struct nepa_define * define)
{
    ncore_lock                  sys_lock;

    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  !N_IS_EPA_OBJECT(epa));
    NREQUIRE(NAPI_POINTER, define != NULL);

    epa->mem = NULL;
    nequeue_init(&epa->working_queue, &define->working_queue);
    nequeue_put_fifo(&epa->working_queue, nsm_event(NSM_INIT));
    nsm_init(&epa->sm, &define->sm);
    nsched_thread_init(&epa->thread, &define->thread);
    ncore_lock_enter(&sys_lock);
    nsched_thread_insert_i(&epa->thread);
    ncore_lock_exit(&sys_lock);

    NOBLIGATION(epa->signature = NSIGNATURE_EPA);
}



void nepa_term(struct nepa * epa)
{
    ncore_lock                  sys_lock;

    NREQUIRE(NAPI_OBJECT, N_IS_EPA_OBJECT(epa));

    ncore_lock_enter(&sys_lock);

    while (!nequeue_is_empty(&epa->working_queue)) {
        const struct nevent *   event;

        event = nequeue_get(&epa->working_queue);
        nevent_destroy_i(event);
    }
    nsched_thread_term(&epa->thread);
    nsm_term(&epa->sm);
    nequeue_term(&epa->working_queue);
    ncore_lock_exit(&sys_lock);

    NOBLIGATION(epa->signature = ~NSIGNATURE_EPA);
}



struct nepa * nepa_create(const struct nepa_define * define, struct nmem * mem)
{
    struct nepa *               epa;
    struct nepa_define          l_define;
    struct nequeue_define       l_working_define;

    NREQUIRE(NAPI_POINTER, define != NULL);
    NREQUIRE(NAPI_OBJECT, N_IS_MEM_OBJECT(mem));

    epa = nmem_zalloc(mem, sizeof(struct nepa));

    if (!epa) {
        goto ERROR_ALLOC_EPA;
    }
    l_working_define.storage = NULL;
    l_working_define.size    = define->working_queue.size;
        /* Check if size != 0 in order to avoid calling malloc() with 0 bytes */
    if (l_working_define.size) {
        l_working_define.storage = nmem_alloc(mem, l_working_define.size);

        if (!l_working_define.storage) {
            goto ERROR_ALLOC_WORKING_FIFO_BUFF;
        }
    }
    l_define.sm             = define->sm;
    l_define.thread         = define->thread;
    l_define.working_queue  = l_working_define;
    nepa_init(epa, &l_define);
    epa->mem = mem;

    return (epa);
ERROR_ALLOC_WORKING_FIFO_BUFF:
    nmem_free(mem, epa);
ERROR_ALLOC_EPA:
    NENSURE("Event Processing Agent not created", epa != NULL);

    return (NULL);
}



void nepa_destroy(struct nepa * epa)
{
    NREQUIRE(NAPI_OBJECT, N_IS_EPA_OBJECT(epa));

    nepa_term(epa);
    nmem_free(epa->mem, nequeue_storage(&epa->working_queue));
    nmem_free(epa->mem, epa);
}



nerror nepa_send_event_i(struct nepa * epa, const struct nevent * event)
{
    nerror                      error;

    NREQUIRE(NAPI_OBJECT, N_IS_EPA_OBJECT(epa));
    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));

    if (nevent_ref(event) < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (!nequeue_is_full(&epa->working_queue)) {
            nequeue_put_fifo(&epa->working_queue, event);
            nsched_thread_insert_i(&epa->thread);
            error = NERROR_NONE;
        } else {
            nevent_ref_down(event);
            nevent_destroy_i(event);
            error = NERROR_NO_RESOURCE;
        }
    } else {
        error = NERROR_NO_REFERENCE;
    }

    NENSURE("event not sent", error == NERROR_NONE);

    return (error);
}



nerror nepa_send_event(struct nepa * epa, const struct nevent * event)
{
    nerror                      error;
    ncore_lock                   sys_lock;

    ncore_lock_enter(&sys_lock);
    error = nepa_send_event_i(epa, event);
    ncore_lock_exit(&sys_lock);

    return (error);
}



nerror nepa_send_event_ahead_i(struct nepa * epa, struct nevent * event)
{
    nerror                      error;

    NREQUIRE(NAPI_OBJECT, N_IS_EPA_OBJECT(epa));
    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));

    if (nevent_ref(event) < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (!nequeue_is_full(&epa->working_queue)) {
            nequeue_put_lifo(&epa->working_queue, event);
            nsched_thread_insert_i(&epa->thread);
            error = NERROR_NONE;
        } else {
            nevent_destroy_i(event);
            error = NERROR_NO_RESOURCE;
        }
    } else {
        error = NERROR_NO_REFERENCE;
    }

    NENSURE("event not sent", error == NERROR_NONE);

    return (error);
}



nerror nepa_send_event_ahead(struct nepa * epa, struct nevent * event)
{
    nerror                      error;
    ncore_lock                  sys_lock;

    ncore_lock_enter(&sys_lock);
    error = nepa_send_event_ahead_i(epa, event);
    ncore_lock_exit(&sys_lock);

    return (error);
}



nerror nepa_send_signal_i(struct nepa * epa, uint16_t event_id)
{
    nerror                      error;
    struct nevent *             event;

    event = nevent_create_i(sizeof(struct nevent), event_id);

    if (!event) {
        
        return (NERROR_NO_MEMORY);
    }
    error = nepa_send_event_i(epa, event);

    return (error);
}



nerror nepa_send_signal(struct nepa * epa, uint16_t event_id)
{
    nerror                      error;
    ncore_lock                  lock;

    ncore_lock_enter(&lock);
    error = nepa_send_signal_i(epa, event_id);
    ncore_lock_exit(&lock);

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.c
 ******************************************************************************/
