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
 * @brief       Event object implementation
 * @addtogroup  eds_event
 *********************************************************************//** @{ */
/**@defgroup    eds_event_impl Implementation
 * @brief       Event object Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "port/sys_lock.h"
#include "shared/component.h"
#include "mem/mem_class.h"
#include "eds/event.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EVENT_STORAGE_HEAP              0
#define EVENT_STORAGE_POOL              1

/*======================================================  LOCAL DATA TYPES  ==*/

struct evtStorage {
    struct storageInstance {
        struct nmem *      handle;
        size_t              block_size;
    }                   mem[CONFIG_EVENT_STORAGE_NPOOLS];
    uint_fast8_t        pools;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**@brief       Initialize new event
 * @param       size
 *              The size of event
 * @param       id
 *              Event identification
 * @param       event
 *              Event to initialize
 * @inline
 */
static void event_init(
    struct nevent *             event,
    size_t                      size,
    uint16_t                    id);

/**@brief       Terminate an event
 * @param       event
 *              Event to terminate
 * @inline
 */
static void event_term(
    struct nevent *             event);


static struct nevent * event_create_i(
    size_t                      size);

static void event_destroy_i(
    struct nevent *             event);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Event management", "Nenad Radulovic");

static struct evtStorage g_event_storage;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void event_init(
    struct nevent *             event,
    size_t                      size,
    uint16_t                    id)
{
    NREQUIRE(NAPI_OBJECT, event->signature != NEVENT_SIGNATURE);

    event->id     = id;
    event->attrib = 0u;                                                         /* Dogadjaj je dinamican, sa 0 korisnika.                   */
    event->ref    = 0u;

#if (CONFIG_EVENT_TIMESTAMP == 1)
    event->timestamp = NSYSTIMER_GET_CVAL();
#endif
#if (CONFIG_EVENT_PRODUCER == 1)
    CONFIG_GET_CURRENT_EPA(&event->producer);
#endif
#if (CONFIG_EVENT_SIZE == 1)
    event->size = size;
#else
    (void)size;
#endif
    NOBLIGATION(event->signature = NEVENT_SIGNATURE);
}

static void event_term(
    struct nevent *    event) {

    NREQUIRE(NAPI_OBJECT, event->signature == NEVENT_SIGNATURE);
    NOBLIGATION(event->signature = ~NEVENT_SIGNATURE);

#if (CONFIG_API_VALIDATION == 0)
    (void)event;
#endif
}

static struct nevent * event_create_i(
    size_t                      size)
{
    uint_fast8_t                cnt;

    NREQUIRE(NAPI_RANGE, size >= sizeof(struct nevent));

    for (cnt = 0u; cnt < g_event_storage.pools; cnt++) {

        if (g_event_storage.mem[cnt].block_size >= size) {
            struct nevent *     event;
            struct nmem *       mem;

            mem   = g_event_storage.mem[cnt].handle;
            event = nmem_alloc_i(mem, size);

            if (event) {
                event->mem = mem;
            }
            
            return (event);
        }
    }
    
    return (NULL);
}

static void event_destroy_i(
    struct nevent *    event)
{
    nmem_free_i(event->mem, event);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/



void nevent_register_mem(
    struct nmem *               mem)
{
    nsys_lock                   sys_lock;
    uint_fast8_t                cnt;
    size_t                      size;

    NREQUIRE(NAPI_POINTER, mem != NULL);

    nsys_lock_enter(&sys_lock);
    NENSURE_INTERNAL(size = nget_mem_size(mem));
    cnt = g_event_storage.pools;

    while (0u < cnt) {
        g_event_storage.mem[cnt].handle = g_event_storage.mem[cnt - 1].handle;
        g_event_storage.mem[cnt].block_size =
            g_event_storage.mem[cnt - 1].block_size;

        if (g_event_storage.mem[cnt].block_size <= size) {

            break;
        }
        cnt--;
    }
    g_event_storage.mem[cnt].handle     = mem;
    g_event_storage.mem[cnt].block_size = size;
    g_event_storage.pools++;
    nsys_lock_exit(&sys_lock);
}



void nevent_unregister_mem(
    struct nmem *               mem)
{
    nsys_lock                   sys_lock;
    uint_fast8_t                cnt;

    nsys_lock_enter(&sys_lock);
    cnt = g_event_storage.pools;

    while ((0u < cnt) && (mem != g_event_storage.mem[cnt].handle)) {
        cnt--;
    }
    g_event_storage.pools--;

    NREQUIRE(NAPI_RANGE, mem == g_event_storage.mem[cnt].handle);

    while (cnt < g_event_storage.pools) {
        g_event_storage.mem[cnt].handle = g_event_storage.mem[cnt + 1].handle;
        g_event_storage.mem[cnt].block_size =
            g_event_storage.mem[cnt + 1].block_size;
        cnt++;
    }
    g_event_storage.mem[g_event_storage.pools - 1].handle     = NULL;
    g_event_storage.mem[g_event_storage.pools - 1].block_size = 0;
    nsys_lock_exit(&sys_lock);
}



struct nevent * nevent_create(
    size_t                      size,
    uint16_t                    id)
{
    struct nevent *             event;
    nsys_lock                   sys_lock;

    nsys_lock_enter(&sys_lock);
    event = event_create_i(size);
    nsys_lock_exit(&sys_lock);

    if (event) {
        event_init(event, size, id);
    }

    return (event);
}



struct nevent * nevent_create_i(
    size_t                      size,
    uint16_t                    id)
{
    struct nevent *             event;

    event = event_create_i(size);
    
    if (event) {
        event_init(event, size, id);
    }

    return (event);
}





void nevent_destroy(
    struct nevent *             event)
{
    nsys_lock                   sys_lock;

    nsys_lock_enter(&sys_lock);
    nevent_destroy_i(event);
    nsys_lock_exit(&sys_lock);
}



void nevent_destroy_i(
    struct nevent *             event)
{
    NREQUIRE(NAPI_POINTER, event);
    NREQUIRE(NAPI_OBJECT, event->signature == NEVENT_SIGNATURE);

    if (nevent_ref_down(event) == 0u) {
        event_term(event);
        event_destroy_i(event);
    }
}



void nevent_lock(
    const struct nevent *       event)
{
	NREQUIRE(NAPI_POINTER, event != NULL);
	NREQUIRE(NAPI_OBJECT,  event->signature == NEVENT_SIGNATURE);

    if (event->attrib) {
    	((struct nevent *)event)->attrib = NEVENT_ATTR_RESERVED;
    }
}



void nevent_unlock(
    const struct nevent *      	event)
{
    NREQUIRE(NAPI_POINTER, event != NULL);
    NREQUIRE(NAPI_OBJECT,  event->signature == NEVENT_SIGNATURE);

    if (event->attrib) {
    	struct nevent * event_ = (struct nevent *)event;

    	event_->attrib = NEVENT_ATTR_DYNAMIC;

		if (event_->ref == 0u) {
			struct nsys_lock    sys_lock;

			event_term(event_);
			nsys_lock_enter(&sys_lock);
			event_destroy_i(event_);
			nsys_lock_exit(&sys_lock);
		}
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of event.c
 ******************************************************************************/
