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
 * @brief       Event Processing Agent implementation
 * @addtogroup  eds_epa
 *********************************************************************//** @{ */
/**@defgroup    eds_epa_impl Implementation
 * @brief       Event Processing Agent Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "port/sys_lock.h"
#include "port/cpu.h"
#include "shared/component.h"
#include "lib/queue.h"
#include "lib/list.h"
#include "lib/bitop.h"
#include "vtimer/vtimer.h"
#include "mem/mem_class.h"
#include "kernel/kernel.h"
#include "eds/event.h"
#include "eds/smp.h"
#include "eds/epa.h"


/*=========================================================  LOCAL MACRO's  ==*/

#define EPA_SIGNATURE                   ((ndebug_magic)0xbeef00faul)

/*======================================================  LOCAL DATA TYPES  ==*/

struct event_fifo
{
    struct nqueue               qp;
    ncpu_reg                    max;
};



/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/*--  Event Queue  -----------------------------------------------------------*/

PORT_C_INLINE
void event_fifo_init(
    struct event_fifo *         event_fifo,
    void **                     buff,
    size_t                      size);

PORT_C_INLINE
void event_fifo_term(
    struct event_fifo *         event_fifo);

PORT_C_INLINE
void event_fifo_put_tail(
    struct event_fifo *         event_fifo,
    struct nevent *            event);

PORT_C_INLINE
void event_fifo_put_head_i(
    struct event_fifo *         event_fifo,
    struct nevent *            event);

PORT_C_INLINE
struct nevent * event_fifo_get(
    struct event_fifo *         event_fifo);

PORT_C_INLINE
bool event_fifo_is_empty(
    const struct event_fifo *   event_fifo);

PORT_C_INLINE
bool event_fifo_is_full(
    const struct event_fifo *   event_fifo);

PORT_C_INLINE
void ** event_fifo_get_buff(
    const struct event_fifo *   event_fifo);

/*--  EPA support  -----------------------------------------------------------*/

PORT_C_INLINE
nerror epa_send_event_i(
    struct nepa *              epa,
    struct nevent *            event);

PORT_C_INLINE
nerror epa_send_ahead_event_i(
    struct nepa *              epa,
    struct nevent *            event);

PORT_C_INLINE
struct nevent * epa_fetch_event_i(
    struct nepa *              epa);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Event Processing Agent", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*--  Event Queue  -----------------------------------------------------------*/

PORT_C_INLINE
void event_fifo_init(
    struct event_fifo *         event_fifo,
    void **                     buff,
    size_t                      size)
{
    nqueue_init(&event_fifo->qp, buff, size);
    event_fifo->max = UINT32_C(0);
}

PORT_C_INLINE
void event_fifo_term(
    struct event_fifo *         event_fifo)
{
    nqueue_term(&event_fifo->qp);
}

PORT_C_INLINE
void event_fifo_put_tail(
    struct event_fifo *         event_fifo,
    struct nevent *             event)
{
    uint32_t                    occupied;

    nqueue_put_tail(&event_fifo->qp, event);
    occupied = nqueue_occupied(&event_fifo->qp);

    if (event_fifo->max < occupied) {
        event_fifo->max = occupied;
    }
}

PORT_C_INLINE
void event_fifo_put_head_i(
    struct event_fifo *         event_fifo,
    struct nevent *             event)
{
    uint32_t                    occupied;

    nqueue_put_head(&event_fifo->qp, event);
    occupied = nqueue_occupied(&event_fifo->qp);

    if (event_fifo->max < occupied) {
        event_fifo->max = occupied;
    }
}

PORT_C_INLINE
struct nevent * event_fifo_get(
    struct event_fifo *         event_fifo)
{
    return ((struct nevent *)nqueue_get_head(&event_fifo->qp));
}

PORT_C_INLINE
bool event_fifo_is_empty(
    const struct event_fifo *   event_fifo)
{
    return (nqueue_is_empty(&event_fifo->qp));
}

PORT_C_INLINE
bool event_fifo_is_full(
    const struct event_fifo * eventQ) {

    return (nqueue_is_full(&eventQ->qp));
}

PORT_C_INLINE
void ** event_fifo_get_buff(
    const struct event_fifo * eventQ) {

    return (nqueue_buffer(&eventQ->qp));
}

/*--  EPA support  -----------------------------------------------------------*/

PORT_C_INLINE
nerror epa_send_event_i(
    struct nepa *              epa,
    struct nevent *             event)
{
    if (event->ref < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (event_fifo_is_empty(&epa->event_fifo) == true) {
            nthread_ready_i(epa->thread);
            event_fifo_put_tail(&epa->event_fifo, event);

            return (NERROR_NONE);
        } else if (event_fifo_is_full(&epa->event_fifo) == false) {
            event_fifo_put_tail(&epa->event_fifo, event);

            return (NERROR_NONE);
        } else {
            nevent_destroy_i(event);

            return (NERROR_NO_MEMORY);
        }
    }

    return (NERROR_NO_REFERENCE);
}

PORT_C_INLINE
nerror epa_send_ahead_event_i(
    struct nepa *              epa,
    struct nevent *             event)
{
    if (event->ref < NEVENT_REF_LIMIT) {
        nevent_ref_up(event);

        if (event_fifo_is_empty(&epa->event_fifo) == true) {
            nthread_ready_i(epa->thread);
            event_fifo_put_head_i(&epa->event_fifo, event);

            return (NERROR_NONE);
        } else if (event_fifo_is_full(&epa->event_fifo) == false) {
            event_fifo_put_head_i(&epa->event_fifo, event);

            return (NERROR_NONE);
        } else {
            nevent_destroy_i(event);

            return (NERROR_NO_MEMORY);
        }
    }

    return (NERROR_NO_REFERENCE);
}

PORT_C_INLINE
struct nevent * epa_fetch_event_i(
    struct nepa *               epa)
{
    struct nevent *             event;

    event = event_fifo_get(&epa->event_fifo);

    if (event_fifo_is_empty(&epa->event_fifo) == true) {
        nthread_block_i(epa->thread);
    }

    return (event);
}

static void epa_dispatch(
    void *                      arg)
{
	struct nepa *				epa = arg;


}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


struct nepa * neds_get_current(void)
{
    /* NOTE: Since pointer loading is atomic operation there is no need to lock
     *       interrupts here.
     */
    
    return ((struct nepa *)nthread_get_current()->stack);
}

struct nepa * nepa_create(
    const struct ndefine_epa *  define_epa,
    const struct ndefine_sm *   define_sm,
    struct nmem *               mem)
{
    struct nepa *               epa;
    void *                      event_fifo_buff;

    NREQUIRE(NAPI_POINTER, define_epa != NULL);
    NREQUIRE(NAPI_POINTER, define_sm != NULL);

    epa = nmem_alloc(mem, sizeof(struct nepa));

    if (epa == NULL) {
        goto ERROR_ALLOC_EPA;
    }
    event_fifo_buff = nmem_alloc(mem, NQUEUE_SIZEOF(define_epa->queue_size));

    if (event_fifo_buff == NULL) {
        goto ERROR_ALLOC_EVENT_FIFO_BUFF;
    }
    epa->mem    = mem;
    epa->name   = define_epa->name;
	event_fifo_init(&epa->event_fifo, event_fifo_buff, define_epa->queue_size);
	event_fifo_put_head_i(&epa->event_fifo, (struct nevent *)nsmp_event(NSMP_INIT));
	nthread_init(&epa->thread, epa_dispatch, epa, define_epa->priority);

    NOBLIGATION(epa->signature = EPA_SIGNATURE);

    return (epa);
ERROR_ALLOC_EVENT_FIFO_BUFF:
    nmem_free(epa->mem, epa);
ERROR_ALLOC_EPA:

    return (NULL);
}

void nepa_init(
	struct nepa * 				epa,
	const struct ndefine_epa *	define_epa,
	const struct ndefine_sm *	define_sm,
	struct nmem *				mem,
	void * 						event_fifo_storage)
{
	epa->mem  = mem;
	epa->name = define_epa->name;
	nsm_init(&epa->sm, define_sm);
	nthread_init(&epa->thread, epa_dispatch, epa, define_epa->priority);
}

void esEpaDestroy(
    struct nepa *               epa)
{
    nsys_lock                   sys_lock;
    void **                     event_fifo_buff;

    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);
    NOBLIGATION(epa->signature = (natomic)~EPA_SIGNATURE);

    nsys_lock_enter(&sys_lock);

    while (event_fifo_is_empty(&epa->event_fifo) != true) {
        struct nevent *         event;

        event = event_fifo_get(&epa->event_fifo);
        nevent_destroy_i(event);
    }
    nthread_block_i(epa->thread);
    nthread_term(epa->thread);
    nsys_lock_exit(&sys_lock);

    event_fifo_buff = event_fifo_get_buff(&epa->event_fifo);
    event_fifo_term(&epa->event_fifo);
    esSmDestroy(epa->sm);
    nmem_free(epa->mem, event_fifo_buff);
    nmem_free(epa->mem, epa->thread);
    nmem_free(epa->mem, epa);
}

nerror esEpaSendEventI(
    struct nepa *      epa,
    struct nevent *    event) {

    nerror             error;

    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    error = epa_send_event_i(epa, event);

    return (error);
}

nerror esEpaSendEvent(
    struct nepa *               epa,
    struct nevent *             event)
{
    nerror                      error;
    nsys_lock                   sys_lock;

    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    nsys_lock_enter(&sys_lock);
    error = epa_send_event_i(epa, event);
    nsys_lock_exit(&sys_lock);

    return (error);
}


nerror esEpaSendAheadEventI(
    struct nepa *      epa,
    struct nevent *    event) {

    nerror             error;

    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    error = epa_send_ahead_event_i(epa, event);

    return (error);
}

nerror esEpaSendAheadEvent(
    struct nepa *               epa,
    struct nevent *             event)
{
    nerror                      error;
    nsys_lock                   sys_lock;

    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    nsys_lock_enter(&sys_lock);
    error = epa_send_ahead_event_i(epa, event);
    nsys_lock_exit(&sys_lock);

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.c
 ******************************************************************************/
