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

#include "plat/critical.h"
#include "eds/event.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EVENT_STORAGE_HEAP              0
#define EVENT_STORAGE_POOL              1

/*======================================================  LOCAL DATA TYPES  ==*/

struct evtStorage {
    struct storageInstance {
        struct esMem *      handle;
        size_t              blockSize;
    }                   mem[CONFIG_EVENT_STORAGE_NPOOLS];
    uint_fast8_t        nPools;
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
static PORT_C_INLINE void eventInit(
    size_t              size,
    uint16_t            id,
    struct esEvent *    event);

/**@brief       Terminate an event
 * @param       event
 *              Event to terminate
 * @inline
 */
static PORT_C_INLINE_ALWAYS void eventTerm(
    struct esEvent *    event);


static PORT_C_INLINE esError eventCreateI(
    size_t              size,
    struct esEvent **   event);

static PORT_C_INLINE esError eventDestroyI(
    struct esEvent *    event);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("EVT", "Event management", "Nenad Radulovic");

static struct evtStorage GlobalEvtStorage;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static PORT_C_INLINE void eventInit(
    size_t              size,
    uint16_t            id,
    struct esEvent *    event) {

    ES_REQUIRE(ES_API_OBJECT, event->signature != ES_EVENT_SIGNATURE);

    event->id     = id;
    event->attrib = 0u;                                                         /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if (CONFIG_EVENT_TIMESTAMP == 1)
    event->timestamp = ES_SYSTIMER_GET_CVAL();
#endif
#if (CONFIG_EVENT_PRODUCER == 1)
    CONFIG_GET_CURRENT_EPA(&event->producer);
#endif
#if (CONFIG_EVENT_SIZE == 1)
    event->size = size;
#else
    (void)size;
#endif
    ES_OBLIGATION(event->signature = ES_EVENT_SIGNATURE);
}

static PORT_C_INLINE void eventTerm(
    struct esEvent *    event) {

    ES_REQUIRE(ES_API_OBJECT, event->signature == ES_EVENT_SIGNATURE);
    ES_OBLIGATION(event->signature = ~ES_EVENT_SIGNATURE);

#if (CONFIG_API_VALIDATION == 0)
    (void)event;
#endif
}

static PORT_C_INLINE esError eventCreateI(
    size_t              size,
    struct esEvent **   event) {

    esError             error;
    uint_fast8_t        cnt;

    ES_REQUIRE(ES_API_RANGE, size >= sizeof(struct esEvent));
    ES_REQUIRE(ES_API_POINTER, event != NULL);

    cnt = 0u;

    while (cnt < GlobalEvtStorage.nPools) {

        if (GlobalEvtStorage.mem[cnt].blockSize >= size) {
            struct esMem *      mem;

            mem = GlobalEvtStorage.mem[cnt].handle;
            error = esMemAllocI(mem, size, (void **)event);

            if (error != ES_ERROR_NONE) {

                return (ES_ERROR_NO_MEMORY);
            }
            (*event)->mem = mem;
            
            return (ES_ERROR_NONE);
        }
        cnt++;
    }
    
    return (ES_ERROR_NO_RESOURCE);
}

static PORT_C_INLINE esError eventDestroyI(
    struct esEvent *    event) {

    esError             error;
    
    ES_ENSURE_INTERNAL(
        error = esMemFreeI(
            event->mem,
            event));

    return (error);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esEventRegisterMem(
    struct esMem *      mem) {

    esLockCtx           lockCtx;
    uint_fast8_t        cnt;
    size_t              size;

    ES_REQUIRE(ES_API_POINTER, mem != NULL);

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    ES_ENSURE_INTERNAL(esMemGetBlockSizeI(mem, &size));
    cnt = GlobalEvtStorage.nPools;

    while (0u < cnt) {
        GlobalEvtStorage.mem[cnt].handle    =
            GlobalEvtStorage.mem[cnt - 1].handle;
        GlobalEvtStorage.mem[cnt].blockSize =
            GlobalEvtStorage.mem[cnt - 1].blockSize;

        if (GlobalEvtStorage.mem[cnt].blockSize <= size) {

            break;
        }
        cnt--;
    }
    GlobalEvtStorage.mem[cnt].handle    = mem;
    GlobalEvtStorage.mem[cnt].blockSize = size;
    GlobalEvtStorage.nPools++;
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (ES_ERROR_NONE);
}

esError esEventUnregisterMem(
    struct esMem *      mem) {

    esLockCtx           lockCtx;
    uint_fast8_t        cnt;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    cnt = GlobalEvtStorage.nPools;

    while ((0u < cnt) && (mem != GlobalEvtStorage.mem[cnt].handle)) {
        cnt--;
    }
    GlobalEvtStorage.nPools--;

    ES_REQUIRE(ES_API_RANGE, mem == GlobalEvtStorage.mem[cnt].handle);

    while (cnt < GlobalEvtStorage.nPools) {
        GlobalEvtStorage.mem[cnt].handle    =
            GlobalEvtStorage.mem[cnt + 1].handle;
        GlobalEvtStorage.mem[cnt].blockSize =
            GlobalEvtStorage.mem[cnt + 1].blockSize;
        cnt++;
    }
    GlobalEvtStorage.mem[GlobalEvtStorage.nPools - 1].handle    = NULL;
    GlobalEvtStorage.mem[GlobalEvtStorage.nPools - 1].blockSize = 0;
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (ES_ERROR_NONE);
}

/*----------------------------------------------------------------------------*/
esError esEventCreate(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = eventCreateI(size, event);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    if (error == ES_ERROR_NONE) {
        eventInit(size, id, *event);
    }

    return (error);
}

/*----------------------------------------------------------------------------*/
esError esEventCreateI(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event) {

    esError             error;

    error = eventCreateI(size, event);
    
    if (error == ES_ERROR_NONE) {
        eventInit(size, id, *event);
    }

    return (error);
}

/*----------------------------------------------------------------------------*/
void esEventReserve(
    struct esEvent *           evt) {

    ES_REQUIRE(ES_API_POINTER, NULL != evt);
    ES_REQUIRE(ES_API_OBJECT, ES_EVENT_SIGNATURE == evt->signature);

    evt->attrib |= ES_EVENT_RESERVED_Msk;
}

/*----------------------------------------------------------------------------*/
void esEventUnReserve(
    struct esEvent *       event) {

    ES_REQUIRE(ES_API_POINTER, event != NULL);
    ES_REQUIRE(ES_API_OBJECT,  event->signature == ES_EVENT_SIGNATURE);

    event->attrib &= (uint16_t)~ES_EVENT_RESERVED_Msk;

    if (esEventRefGet(event) == 0u) {
        eventTerm(
            event);
        eventDestroyI(
            event);
    }
}

/*----------------------------------------------------------------------------*/
esError esEventDestroy(
    struct esEvent *    evt) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = esEventDestroyI(
        evt);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (error);
}

/*----------------------------------------------------------------------------*/
esError esEventDestroyI(
    struct esEvent *    event) {

    esError             error;

    ES_REQUIRE(ES_API_POINTER, event);
    ES_REQUIRE(ES_API_OBJECT, event->signature == ES_EVENT_SIGNATURE);

    error = ES_ERROR_NONE;
    esEventReferenceDown(
        event);

    if (esEventRefGet(event) == 0u) {
        eventTerm(
            event);
        error = eventDestroyI(
            event);
    }

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of event.c
 ******************************************************************************/
