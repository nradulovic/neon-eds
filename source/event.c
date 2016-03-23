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
 * @brief       Event object implementation
 * @addtogroup  ep_event
 *********************************************************************//** @{ */
/**@defgroup    ep_event_impl Implementation
 * @brief       Event object Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <string.h>

#include "port/core.h"
#include "base/component.h"
#include "mm/mem.h"
#include "ep/event.h"
#include "ep/epa.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct event_storage
{
    struct nmem *               mem[CONFIG_EVENT_STORAGE_NPOOLS];
    uint_fast8_t                pools;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


/**@brief       Initialise new event
 * @param       size
 *              The size of event
 * @param       id
 *              Event identification
 * @param       event
 *              Event to initialise
 */
static void event_init(
    struct nevent *             event,
    uint16_t                    id,
    struct nmem *               mem,
    size_t                      size);



/**@brief       Terminate an event
 * @param       event
 *              Event to terminate
 */
static void event_term(
    struct nevent *             event);



static struct nmem * find_memory_i(
    size_t                      size);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Event management");

static struct event_storage     g_event_storage;

/*======================================================  GLOBAL VARIABLES  ==*/

const struct nevent 			g_default_event = NEVENT_INITIALIZER(UINT_FAST16_MAX, NULL, sizeof(struct nevent));

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static void event_init(
    struct nevent *             event,
    uint16_t                    id,
    struct nmem *               mem,
    size_t                      size)
{
    /* NOTE:
     * Size parameter is already checked by event_create_i()
     */

    event->id       = id;
    event->ref      = 0u;
    event->attrib   = NEVENT_ATTR_DYNAMIC;
    event->mem      = mem;
#if (CONFIG_EVENT_PRODUCER == 1)
    event->producer = nepa_get_current();
#endif
#if (CONFIG_EVENT_SIZE == 1)
    event->size     = size;
#else
    (void)size;
#endif
    NOBLIGATION(event->signature = NSIGNATURE_EVENT);
}



static void event_term(struct nevent * event)
{
    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));
    NOBLIGATION(event->signature = ~NSIGNATURE_EVENT);

#if (CONFIG_API_VALIDATION == 0)
    (void)event;                                            /* Remove compiler warning */
#endif
}



static struct nmem * find_memory_i(size_t size)
{
#if (CONFIG_EVENT_STORAGE_NPOOLS != 1)
    struct nmem *               mem;
    uint_fast8_t                cnt;

    NREQUIRE(NAPI_RANGE, size >= sizeof(struct nevent));

    mem = NULL;

    for (cnt = 0u; cnt < g_event_storage.pools; cnt++) {

        mem = g_event_storage.mem[cnt];

        if (nmem_get_size_i(mem) >= size) {
            break;
        }
    }
    NENSURE("event not allocated", mem != NULL);

    return (mem);
#else
    return (g_event_storage.mem[0]);
#endif
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nevent_register_mem(struct nmem * mem)
{
    ncore_lock                  sys_lock;
    uint_fast8_t                cnt;
    size_t                      size;

    NREQUIRE(NAPI_OBJECT, N_IS_MEM_OBJECT(mem));
    NREQUIRE(NAPI_USAGE, g_event_storage.pools < CONFIG_EVENT_STORAGE_NPOOLS);

    ncore_lock_enter(&sys_lock);
    size = nmem_get_size_i(mem);

    for (cnt = g_event_storage.pools; cnt > 0u; cnt--) {
        g_event_storage.mem[cnt] = g_event_storage.mem[cnt - 1];

        if (nmem_get_size_i(g_event_storage.mem[cnt]) <= size) {

            break;
        }
    }
    g_event_storage.mem[cnt] = mem;
    g_event_storage.pools++;
    ncore_lock_exit(&sys_lock);
}



void nevent_unregister_mem(struct nmem * mem)
{
    ncore_lock                  sys_lock;
    uint_fast8_t                cnt;

    NREQUIRE(NAPI_OBJECT, N_IS_MEM_OBJECT(mem));
    NREQUIRE(NAPI_USAGE, g_event_storage.pools != 0);

    ncore_lock_enter(&sys_lock);
    cnt = g_event_storage.pools;

    while ((0u < cnt) && (mem != g_event_storage.mem[cnt])) {
        cnt--;
    }
    NENSURE(NAPI_RANGE, mem == g_event_storage.mem[cnt]);

    g_event_storage.pools--;

    while (cnt < g_event_storage.pools) {
        g_event_storage.mem[cnt] = g_event_storage.mem[cnt + 1];
        cnt++;
    }
    g_event_storage.mem[g_event_storage.pools - 1] = NULL;
    ncore_lock_exit(&sys_lock);
}



struct nevent * nevent_create(size_t size, uint16_t id)
{
    struct nmem *               mem;
    struct nevent *             event;
    ncore_lock                  sys_lock;

    ncore_lock_enter(&sys_lock);
    mem   = find_memory_i(size);
    event = nmem_alloc_i(mem, size);
    ncore_lock_exit(&sys_lock);

    if (event) {
        event_init(event, id, mem, size);
    }

    return (event);
}



struct nevent * nevent_create_i(size_t size, uint16_t id)
{
    struct nmem *               mem;
    struct nevent *             event;

    mem   = find_memory_i(size);
    event = nmem_alloc_i(mem, size);
    
    if (event) {
        event_init(event, id, mem, size);
    }

    NENSURE("event not created", event != NULL);

    return (event);
}



struct nevent * nevent_create_from_i(struct nmem * mem, size_t size, uint16_t id)
{
    struct nevent *             event;

    NREQUIRE(NAPI_RANGE, size >= sizeof(struct nevent));

    event = nmem_alloc_i(mem, size);

    if (event) {
        event_init(event, id, mem, size);
    }

    return (event);
}



void nevent_destroy(const struct nevent * event)
{
    ncore_lock                  sys_lock;

    ncore_lock_enter(&sys_lock);
    nevent_destroy_i(event);
    ncore_lock_exit(&sys_lock);
}



void nevent_destroy_i(const struct nevent * event)
{
    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));

    if (nevent_ref(event) == 0u) {
        event_term((struct nevent *)event);
        nmem_free_i(event->mem, (void *)event);
    }
}



struct nevent * nevent_forward(const struct nevent * event, uint16_t id)
{
#if (CONFIG_EVENT_SIZE == 1)
    ncore_lock                  lock;
    struct nmem *               mem;
    struct nevent *             ret;

    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));

    ncore_lock_enter(&lock);

    if (event->mem) {
        mem = event->mem;
    } else {
        mem = find_memory_i(event->size);
    }
    ret = nmem_alloc_i(mem, event->size);
    ncore_lock_exit(&lock);

    if (ret) {
        memcpy(ret, event, event->size);
        ret->id = id;
    }

    NENSURE("event not forwarded", ret != NULL);

    return (ret);
#else
    struct nevent *             ret;

    (void)event;
    (void)id;
    ret = NULL;

    NENSURE("event forward depends on event size configuration", ret != NULL);

    return (ret);
#endif
}



void nevent_lock(const struct nevent * event)
{
    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));

    if (event->attrib) {
        ((struct nevent *)event)->attrib = NEVENT_ATTR_RESERVED;
    }
}



void nevent_unlock(const struct nevent * event)
{
    NREQUIRE(NAPI_OBJECT, N_IS_EVENT_OBJECT(event));

    if (event->attrib) {
        struct nevent * event_ = (struct nevent *)event;

        event_->attrib = NEVENT_ATTR_DYNAMIC;

        if (event_->ref == 0u) {
            struct ncore_lock    sys_lock;

            event_term(event_);
            ncore_lock_enter(&sys_lock);
            nmem_free_i(event_->mem, event_);
            ncore_lock_exit(&sys_lock);
        }
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of event.c
 ******************************************************************************/
