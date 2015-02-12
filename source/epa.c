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


#include "port/sys_lock.h"
#include "port/cpu.h"
#include "shared/component.h"
#include "mem/mem_class.h"
#include "eds/epa.h"
#include "eds/event.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void idle(void);

/*=======================================================  LOCAL VARIABLES  ==*/

/**@brief       Provides the basic information about this module
 */
static const NCOMPONENT_DEFINE("Event Processing Agent", "Nenad Radulovic");

static void (* g_idle)(void) = idle;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void idle(void)
{

}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/



void neds_set_idle(
	void                	 (* idle)(void))
{
	g_idle = idle;
}



void neds_run(void)
{
	struct nsys_lock            lock;
	struct nthread *			thread;

	nsys_lock_enter(&lock);

	for (;;) {
		while ((thread = nsched_thread_fetch_i())) {
			struct nepa *			epa;
			const struct nevent *	event;
			naction					action;

			nsched_thread_remove_i(thread);
			epa   = THREAD_TO_EPA(thread);
			event = nequeue_get(&epa->working_fifo);
			nsys_lock_exit(&lock);
			action = nsm_dispatch(&epa->sm, event);
			nsys_lock_enter(&lock);

			if ((action == NACTION_DEFFERED) &&
				!nequeue_is_full(&epa->deffered_fifo)) {
				nequeue_put_fifo(&epa->deffered_fifo, event);
			} else {
				nevent_destroy_i(event);
			}
		}
		nsys_lock_exit(&lock);
		g_idle();
		nsys_lock_enter(&lock);
	}
	nsys_lock_exit(&lock);
}



void nepa_init(
	struct nepa *				epa,
	const struct nepa_define *	define)
{
	nsys_lock					sys_lock;


	epa->mem = NULL;
	nequeue_init(&epa->working_fifo, &define->working_fifo);
	nequeue_put_fifo(&epa->working_fifo, nsmp_event(NSMP_INIT));
	nequeue_init(&epa->deffered_fifo, &define->deffered_fifo);
	nsm_init(&epa->sm, &define->sm);
	nsched_thread_init(&epa->thread, &define->thread);
	nsys_lock_enter(&sys_lock);
	nsched_thread_insert_i(&epa->thread);
	nsys_lock_exit(&sys_lock);

	NOBLIGATION(epa->signature = EPA_SIGNATURE);
}



void nepa_term(
	struct nepa *				epa)
{
	nsys_lock					sys_lock;

	nsys_lock_enter(&sys_lock);

    while (!nequeue_is_empty(&epa->working_fifo)) {
        const struct nevent *   event;

        event = nequeue_get(&epa->working_fifo);
        nevent_destroy_i(event);
    }

	while (!nequeue_is_empty(&epa->deffered_fifo)) {
		const struct nevent * 	event;

		event = nequeue_get(&epa->deffered_fifo);
		nevent_destroy_i(event);
	}
	nsched_thread_term(&epa->thread);
	nsm_term(&epa->sm);
	nequeue_term(&epa->deffered_fifo);
	nequeue_term(&epa->working_fifo);
	nsys_lock_exit(&sys_lock);
}

struct nepa * nepa_create(
    const struct nepa_define *  define,
    struct nmem *               mem)
{
    struct nepa *               epa;
    struct nepa_define			l_define;
    struct nequeue_define 		l_working_define;
    struct nequeue_define		l_deffered_define;

    NREQUIRE(NAPI_POINTER, define != NULL);

    epa = nmem_alloc(mem, sizeof(struct nepa));

    if (!epa) {
        goto ERROR_ALLOC_EPA;
    }

	l_working_define.storage = nmem_alloc(mem, define->working_fifo.size);
	l_working_define.size    = define->working_fifo.size;

	if (!l_working_define.storage) {
		goto ERROR_ALLOC_WORKING_FIFO_BUFF;
	}
	l_deffered_define.storage = NULL;
	l_deffered_define.size	  = 0;

	if (define->deffered_fifo.size) {
		l_deffered_define.storage = nmem_alloc(mem, define->deffered_fifo.size);
		l_deffered_define.size    = define->deffered_fifo.size;

		if (!l_deffered_define.storage) {
			goto ERROR_ALLOC_DEFFERED_FIFO_BUFF;
		}
	}
	l_define.sm 			= define->sm;
	l_define.thread 		= define->thread;
	l_define.working_fifo	= l_working_define;
	l_define.deffered_fifo  = l_deffered_define;
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
    NOBLIGATION(epa->signature = (ndebug_magic)~EPA_SIGNATURE);

    nepa_term(epa);

    if (nequeue_storage(&epa->deffered_fifo)) {
    	nmem_free(epa->mem, nequeue_storage(&epa->deffered_fifo));
    }
    nmem_free(epa->mem, nequeue_storage(&epa->working_fifo));
    nmem_free(epa->mem, epa);
}



nerror nepa_send_event_i(
    struct nepa *      			epa,
    struct nevent *    			event)
{
    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    if (nevent_ref(event) < NEVENT_REF_LIMIT) {
    	nevent_ref_up(event);

    	if (!nequeue_is_full(&epa->working_fifo)) {
        	nequeue_put_fifo(&epa->working_fifo, event);
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
    nsys_lock                   sys_lock;

    nsys_lock_enter(&sys_lock);
    error = nepa_send_event_i(epa, event);
    nsys_lock_exit(&sys_lock);

    return (error);
}


nerror nepa_send_event_ahead_i(
    struct nepa *      			epa,
    struct nevent *    			event)
{
    NREQUIRE(NAPI_POINTER, epa != NULL);
    NREQUIRE(NAPI_OBJECT,  epa->signature == EPA_SIGNATURE);

    if (event->ref < NEVENT_REF_LIMIT) {
		nevent_ref_up(event);

		if (nequeue_is_empty(&epa->working_fifo) == true) {
			nequeue_put_lifo(&epa->working_fifo, event);
			nsched_thread_insert_i(&epa->thread);

			return (NERROR_NONE);
		} else if (!nequeue_is_full(&epa->working_fifo) == false) {
			nequeue_put_lifo(&epa->working_fifo, event);

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
    nsys_lock                   sys_lock;

    nsys_lock_enter(&sys_lock);
    error = nepa_send_event_ahead_i(epa, event);
    nsys_lock_exit(&sys_lock);

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.c
 ******************************************************************************/
