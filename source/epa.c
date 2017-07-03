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

#include "ep/epa.h"

#include "ep/event.h"
#include "mm/mem.h"
#include "port/core.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static void
epa_dispatch_i(struct nthread * thread, ncore_lock * lock)
{
    struct nepa *               epa;
    const struct nevent *       event;
                                                           
    epa = NP_THREAD_TO_EPA(thread);                        /* Get EPA pointer */
    event = nqueue_get(epa->queue);              /* Get Event pointer */
    ncore_lock_exit(lock);
    /* ********************************************************************** *
     * NOTE: Dispatch the state machine. This is a good place to              *
     * place a breakpoint when debugging state machines.                      *
     * ********************************************************************** */
    nsm_dispatch(epa->sm, event);
    nevent_ref_down(event);
    ncore_lock_enter(lock);
    nevent_destroy_i(event);
    nthread_remove_i(thread);                             /* Block the thread */
}



static void
epa_init_i(struct nthread * thread, ncore_lock * lock)
{
    ncore_lock_exit(lock);
    nsm_dispatch(NP_THREAD_TO_EPA(thread)->sm, nsm_event(NSM_INIT));
    ncore_lock_enter(lock);
    nthread_remove_i(thread);                             /* Block the thread */
    nthread_set_dispatch(thread, epa_dispatch_i);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

#if (CONFIG_DYNAMIC_EPA == 1)
struct nepa * nepa_alloc(struct nmem * mem, const char * name, size_t q_size, 
    uint8_t prio, size_t wspace_size, nstate * init, enum nsm_type type)
{
    struct nepa *               epa;
    struct nqueue *             queue;
    struct nsm *                sm;

    NREQUIRE(N_IS_MEM_OBJECT(mem));

    epa = nmem_zalloc(mem, sizeof(struct nepa));

    if (!epa) {
        goto ERROR_ALLOC_EPA;
    }
    queue = nqueue_alloc(mem, q_size);
    
    if (!queue) {
        goto ERROR_ALLOC_QUEUE;
    }
    sm = nsm_alloc(mem, wspace_size, init, type);
    
    if (!sm) {
        goto ERROR_ALLOC_SM;
    }
    epa->mem = mem;
    epa->sm = sm;
    epa->queue = queue;
    nthread_init(&epa->thread, name, prio, NULL);
    
    NOBLIGATION(NSIGNATURE_IS(epa, NSIGNATURE_EPA));
    
    return (epa);
ERROR_ALLOC_SM:
    nqueue_free(queue);
ERROR_ALLOC_QUEUE:
    nmem_free(mem, epa);
    epa = NULL;
ERROR_ALLOC_EPA:
    NENSURE(epa);

    return (epa);
}
#endif



#if (CONFIG_DYNAMIC_EPA == 1)
void nepa_destroy(struct nepa * epa)
{
    NREQUIRE(N_IS_EPA_OBJECT(epa));
    
    nthread_term(&epa->thread);
    nsm_free(epa->sm);
    NOBLIGATION(NSIGNATURE_IS(epa, ~NSIGNATURE_EPA));
    
    while (!nqueue_is_empty(epa->queue)) {
        nevent_destroy(nqueue_get(epa->queue));
    }
    nqueue_free(epa->queue);
    nmem_free(epa->mem, epa);
}
#endif



void nepa_register(struct nepa * epa)
{
    ncore_lock                  sys_lock;
    
    NREQUIRE(N_IS_EPA_OBJECT(epa));

    nthread_set_dispatch(&epa->thread, epa_init_i);
    ncore_lock_enter(&sys_lock);
    nthread_insert_i(&epa->thread);
    ncore_lock_exit(&sys_lock);
}



void * nepa_create_storage(size_t size)
{
    struct nmem *               mem = NMEM_GENERIC_HEAP;

#if (CONFIG_DYNAMIC_EPA == 1)
    struct nepa *               epa;
    epa = nepa_get_current();

    if (epa->mem) {
        mem = epa->mem;
    }
#endif
    NASSERT_INTERNAL(N_IS_MEM_OBJECT(epa->mem));

    return (nmem_zalloc(mem, size));
}



void nepa_delete_storage(void * storage)
{
    struct nmem *               mem = NMEM_GENERIC_HEAP;

#if (CONFIG_DYNAMIC_EPA == 1)
    struct nepa *               epa;
    epa = nepa_get_current();

    if (epa->mem) {
        mem = epa->mem;
    }
#endif
    NASSERT_INTERNAL(N_IS_MEM_OBJECT(epa->mem));
    nmem_free(mem, storage);
}



nerror nepa_defer_event(struct nqueue * queue, const struct nevent * event)
{
    /* NOTE:
     * This function can be called only from loop context. Because of this the 
     * queue does not need to be protected from interrupt context.
     */
    nerror                      error;

    error = NERROR_NO_RESOURCE;

    if (!nqueue_is_full(queue)) {
        nevent_ref_up(event);
        nqueue_put_fifo(queue, (struct nevent *)event);
        error = NERROR_NONE;
    }

    return (error);
}



nerror nepa_defer_fetch_one(struct nqueue * queue)
{
    /* NOTE:
     * This function can be called only from loop context. Because of this the 
     * queue does not need to be protected from interrupt context.
     */
    struct nepa *               epa;
    nerror                      error;

    epa   = nepa_get_current();
    error = NERROR_NONE;
    
    if (!nqueue_is_empty(queue)) {
        const struct nevent *   event;

        event = nqueue_get(queue);
        nevent_ref_down(event);
        error = nepa_send_event(epa, event);
    }
    
    return (error);
}



nerror nepa_defer_fetch_all(struct nqueue * queue)
{
    /* NOTE:
     * This function can be called only from loop context. Because of this the 
     * queue does not need to be protected from interrupt context.
     */
    struct nepa *               epa;
    nerror                      error;

    epa   = nepa_get_current();
    error = NERROR_NONE;
    
    while (!error && !nqueue_is_empty(queue)) {
        const struct nevent *   event;
        
        event = nqueue_get(queue);
        nevent_ref_down(event);
        error = nepa_send_event(epa, event);
    }
    
    return (error);
}



nerror nepa_send_event_i(struct nepa * epa, const struct nevent * event)
{
    nerror                      error;

    NREQUIRE(N_IS_EPA_OBJECT(epa));
    NREQUIRE(N_IS_EVENT_OBJECT(event));
    NREQUIRE(ncore_is_lock_valid());

    if (nevent_ref(event) < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (!nqueue_is_full(epa->queue)) {
            nqueue_put_fifo(epa->queue, (struct nevent *)event);
            nthread_insert_i(&epa->thread);
            error = NERROR_NONE;
        } else {
            nevent_ref_down(event);
            nevent_destroy_i(event);
            error = NERROR_NO_RESOURCE;
        }
    } else {
        error = NERROR_NO_REFERENCE;
    }

    NENSURE(error == NERROR_NONE);

    return (error);
}



nerror nepa_send_event(struct nepa * epa, const struct nevent * event)
{
    nerror                      error;
    ncore_lock                  sys_lock;

    ncore_lock_enter(&sys_lock);
    error = nepa_send_event_i(epa, event);
    ncore_lock_exit(&sys_lock);

    return (error);
}



nerror nepa_send_event_ahead_i(struct nepa * epa, struct nevent * event)
{
    nerror                      error;

    NREQUIRE(N_IS_EPA_OBJECT(epa));
    NREQUIRE(N_IS_EVENT_OBJECT(event));
    NREQUIRE(ncore_is_lock_valid());

    if (nevent_ref(event) < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (!nqueue_is_full(epa->queue)) {
        	nqueue_put_lifo(epa->queue, event);
            nthread_insert_i(&epa->thread);
            error = NERROR_NONE;
        } else {
            nevent_destroy_i(event);
            error = NERROR_NO_RESOURCE;
        }
    } else {
        error = NERROR_NO_REFERENCE;
    }

    NENSURE(error == NERROR_NONE);

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

    NREQUIRE(ncore_is_lock_valid());

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
