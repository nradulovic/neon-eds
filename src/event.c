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
#include "mem/heap.h"
#include "mem/pool.h"
#include "eds/event.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EVENT_STORAGE_HEAP              0
#define EVENT_STORAGE_POOL              1

/*======================================================  LOCAL DATA TYPES  ==*/

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_POOL)
struct evtPools {
    struct esPoolMem *  handle[CONFIG_EVENT_STORAGE_NPOOLS];
    uint_fast8_t        nPools;
};
#endif

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

static PORT_C_INLINE void eventDestroyI(
    struct esEvent *    event);

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_POOL)
static PORT_C_INLINE struct esPoolMem * poolFindI(
    size_t              size);
#endif

/*=======================================================  LOCAL VARIABLES  ==*/

ES_MODULE_INFO_CREATE("EVT", "Event management", "Nenad Radulovic");

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_POOL)
static struct evtPools GlobalEvtStorage;
#endif

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static PORT_C_INLINE void eventInit(
    size_t              size,
    uint16_t            id,
    struct esEvent *    event) {

    ES_API_REQUIRE(ES_API_OBJECT, event->signature != ES_EVENT_SIGNATURE);

    event->id     = id;
    event->attrib = 0u;                                                           /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if (CONFIG_EVENT_TIMESTAMP == 1)
    event->timestamp = ES_SYSTIMER_GET_CVAL();
#endif
#if (CONFIG_EVENT_PRODUCER == 1)
    event->producer = appEvtGeneratorGet();
#endif
#if (CONFIG_EVENT_SIZE == 1)
    event->size = size;
#else
    (void)size;
#endif
    ES_API_OBLIGATION(event->signature = ES_EVENT_SIGNATURE);
}

static PORT_C_INLINE void eventTerm(
    struct esEvent *    event) {

    ES_API_REQUIRE(ES_API_OBJECT, event->signature == ES_EVENT_SIGNATURE);
    ES_API_OBLIGATION(event->signature = ~ES_EVENT_SIGNATURE);

#if (CONFIG_API_VALIDATION == 0)
    (void)event;
#endif
}

static PORT_C_INLINE esError eventCreateI(
    size_t              size,
    struct esEvent **   event) {

    ES_API_REQUIRE(ES_API_RANGE, size >= sizeof(struct esEvent));
    ES_API_REQUIRE(ES_API_POINTER, event != NULL);

#if   (CONFIG_EVENT_STORAGE == EVENT_STORAGE_HEAP)
    {
        esError         error;

        error = esHeapMemAllocI(
            CONFIG_EVENT_STORAGE_HEAP,
            size,
            (void **)event);

        if (error != ES_ERROR_NONE) {

            return (ES_ERROR_NO_MEMORY);
        }
    }
#else
    {
        esError         error;
        struct esPoolMem * pool;

        if ((pool = poolFindI(size)) == NULL) {

            return (ES_ERROR_NO_RESOURCE);
        }

        if ((error = esPoolMemAllocI(pool, (void **)event)) != ES_ERROR_NONE) {

            return (ES_ERROR_NO_MEMORY);
        }
    }
#endif
    return (ES_ERROR_NONE);
}

static PORT_C_INLINE void eventDestroyI(
    struct esEvent *    event) {

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_HEAP)
    esHeapMemFreeI(
        CONFIG_EVENT_STORAGE_HEAP,
        event);
#else
    esPoolMemFreeI(
        event->poolMem,
        event);
#endif
}

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_POOL)
static PORT_C_INLINE struct esPoolMem * poolFindI(
    size_t              size) {

    uint_fast8_t        cnt;

    cnt = 0u;

    while (cnt < GlobalEvtStorage.nPools) {
        size_t          currSize;

        esPoolMemGetBlockSizeI(
            GlobalEvtStorage.handle[cnt],
            &currSize);

        if (currSize >= size) {

            return (GlobalEvtStorage.handle[cnt]);
        }
        cnt++;
    }

    return (NULL);
}
#endif

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esEventPoolRegister(
    struct esPoolMem *  poolMem) {

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_HEAP)
    ES_API_REQUIRE(ES_API_USAGE, CONFIG_EVENT_STORAGE == 1);

    (void)poolMem;

    return (ES_ERROR_NONE);
#else
    esLockCtx           lockCtx;
    uint_fast8_t        cnt;
    size_t              size;

    ES_API_REQUIRE(ES_API_USAGE, GlobalEvtStorage.nPools != CONFIG_EVENT_STORAGE_NPOOLS);

    esPoolMemGetBlockSizeI(
        poolMem,
        &size);
    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    cnt  = GlobalEvtStorage.nPools;

    while (0u < cnt) {
        size_t          currSize;

        GlobalEvtStorage.handle[cnt] = GlobalEvtStorage.handle[cnt - 1];
        esPoolMemGetBlockSizeI(
            GlobalEvtStorage.handle[cnt],
            &currSize);

        if (currSize <= size) {

            break;
        }
        cnt--;
    }
    GlobalEvtStorage.handle[cnt] = poolMem;
    GlobalEvtStorage.nPools++;
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (ES_ERROR_NONE);
#endif
}

esError esEventPoolUnregister(
    struct esPoolMem *  poolMem) {

#if (CONFIG_EVENT_STORAGE == EVENT_STORAGE_HEAP)
    ES_API_REQUIRE(ES_API_USAGE, CONFIG_EVENT_STORAGE == 1);

    (void)poolMem;

    return (ES_ERROR_NONE);
#else
    esLockCtx           lockCtx;
    uint_fast8_t        cnt;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    cnt = GlobalEvtStorage.nPools;

    while ((0u < cnt) && (poolMem != GlobalEvtStorage.handle[cnt])) {
        cnt--;
    }
    GlobalEvtStorage.nPools--;

    ES_API_REQUIRE(ES_API_RANGE, poolMem == GlobalEvtStorage.handle[cnt]);

    while (cnt < GlobalEvtStorage.nPools) {
        GlobalEvtStorage.handle[cnt] = GlobalEvtStorage.handle[cnt + 1];
        cnt++;
    }
    GlobalEvtStorage.handle[GlobalEvtStorage.nPools - 1] = NULL;
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (ES_ERROR_NONE);
#endif
}


/*----------------------------------------------------------------------------*/
esError esEventCreate(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = eventCreateI(
        size,
        event);
    ES_CRITICAL_LOCK_EXIT(lockCtx);
    eventInit(
        size,
        id,
        *event);

    return (error);
}

/*----------------------------------------------------------------------------*/
esError esEventCreateI(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event) {

    esError             error;

    error = eventCreateI(
        size,
        event);
    eventInit(
        size,
        id,
        *event);

    return (error);
}

/*----------------------------------------------------------------------------*/
void esEventReserve(
    struct esEvent *           evt) {

    ES_API_REQUIRE(ES_API_POINTER, NULL != evt);
    ES_API_REQUIRE(ES_API_OBJECT, ES_EVENT_SIGNATURE == evt->signature);

    evt->attrib |= ES_EVENT_RESERVED_Msk;
}

/*----------------------------------------------------------------------------*/
void esEventUnReserve(
    struct esEvent *       event) {

    ES_API_REQUIRE(ES_API_POINTER, event != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  event->signature == ES_EVENT_SIGNATURE);

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

    ES_API_REQUIRE(ES_API_POINTER, event);
    ES_API_REQUIRE(ES_API_OBJECT, event->signature == ES_EVENT_SIGNATURE);

    esEventReferenceDown(
        event);

    if (esEventRefGet(event) == 0u) {
        eventTerm(
            event);
        eventDestroyI(
            event);
    }

    return (ES_ERROR_NONE);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of event.c
 ******************************************************************************/
