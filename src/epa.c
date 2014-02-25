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

#include "plat/critical.h"
#include "base/queue.h"
#include "base/list.h"
#include "base/prio_queue.h"
#include "eds/epa.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EPA_SIGNATURE                   ((esAtomic)0xbeef00faul)

/*======================================================  LOCAL DATA TYPES  ==*/

enum epaKernelState {
    KERNEL_INVALID_STATE,
    KERNEL_STARTED,
    KERNEL_STOPPED
};

struct epaSchedElem {
    struct esPqElem     pqElem;
    struct esSls        regNode;
};

struct epaSched {
    struct esPq         ready;
    struct esSls        regSentinel;
    struct esEpa *      epa;
};

struct epaKernel {
    struct epaSched     sched;
    void             (* idle)(void);
    enum epaKernelState state;
};

struct epaEventQ {
    struct esQp         qp;
    uint32_t            max;
};

struct esEpa {
    struct esMem *      mem;
    struct esSm *       sm;
    struct epaSchedElem schedElem;
    struct epaEventQ    eventQ;
    const PORT_C_ROM char * name;
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    esAtomic            signature;
#endif
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/*--  Event Queue  -----------------------------------------------------------*/

static PORT_C_INLINE void eventQInit(
    struct epaEventQ *  eventQ,
    void **             buff,
    size_t              size);

static PORT_C_INLINE void eventQTerm(
    struct epaEventQ *  eventQ);

static PORT_C_INLINE void eventQPutItemI(
    struct epaEventQ *  eventQ,
    struct esEvent *    event);

static PORT_C_INLINE void eventQPutAheadItemI(
    struct epaEventQ *  eventQ,
    struct esEvent *    event);

static PORT_C_INLINE struct esEvent * eventQGetItemI(
    struct epaEventQ *  eventQ);

static PORT_C_INLINE bool eventQIsEmpty(
    const struct epaEventQ * eventQ);

static PORT_C_INLINE bool eventQIsFull(
    const struct epaEventQ * eventQ);

static PORT_C_INLINE void ** eventQBuff(
    const struct epaEventQ * eventQ);

/*--  Scheduler  -------------------------------------------------------------*/

static PORT_C_INLINE void schedInit(
    void);

static PORT_C_INLINE void schedTerm(
    void);

static PORT_C_INLINE void schedElementInit(
    struct epaSchedElem * elem,
    uint_fast8_t        priority);

static PORT_C_INLINE void schedElementTermI(
    struct epaSchedElem * elem);

static PORT_C_INLINE void schedReadyAddI(
    struct epaSchedElem * elem);

static PORT_C_INLINE void schedReadyRmI(
    struct epaSchedElem * elem);

static PORT_C_INLINE void schedReadyRmSafeI(
    struct epaSchedElem * elem);

static PORT_C_INLINE bool schedReadyIsEmptyI(
    void);

static PORT_C_INLINE struct esEpa * schedReadyGetHighestI(
    void);

static PORT_C_INLINE bool schedIsEmptyI(
    void);

static PORT_C_INLINE void schedSetCurrentI(
    struct esEpa *      epa);

static PORT_C_INLINE struct esEpa * schedGetHead(
    void);

/*--  EPA support  -----------------------------------------------------------*/

static PORT_C_INLINE esError epaSendEventI(
    struct esEpa *      epa,
    struct esEvent *    event);

static PORT_C_INLINE esError epaSendAheadEventI(
    struct esEpa *      epa,
    struct esEvent *    event);

static PORT_C_INLINE struct esEvent * epaFetchEventI(
    struct esEpa *      epa);

/*--  Kernel support  --------------------------------------------------------*/

static void kernelIdle(
    void);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("EPA", "Event Processing Agent", "Nenad Radulovic");
static struct epaKernel GlobalEpaKernel;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*--  Event Queue  -----------------------------------------------------------*/

static PORT_C_INLINE void eventQInit(
    struct epaEventQ *  eventQ,
    void **             buff,
    size_t              size) {

    esQpInit(&eventQ->qp, buff, size);
    eventQ->max = UINT32_C(0);
}

static PORT_C_INLINE void eventQTerm(
    struct epaEventQ *  eventQ) {

    esQpTerm(&eventQ->qp);
}

static PORT_C_INLINE void eventQPutItemI(
    struct epaEventQ *  eventQ,
    struct esEvent *    event) {

    uint32_t            occupied;

    esQpPutItem(&eventQ->qp, event);
    occupied = esQpOccupied(&eventQ->qp);

    if (eventQ->max < occupied) {
        eventQ->max = occupied;
    }
}

static PORT_C_INLINE void eventQPutAheadItemI(
    struct epaEventQ *  eventQ,
    struct esEvent *    event) {

    uint32_t            occupied;

    esQpPutTailItem(&eventQ->qp, event);
    occupied = esQpOccupied(&eventQ->qp);

    if (eventQ->max < occupied) {
        eventQ->max = occupied;
    }
}

static PORT_C_INLINE struct esEvent * eventQGetItemI(
    struct epaEventQ *  eventQ) {

    return ((struct esEvent *)esQpGetItem(&eventQ->qp));
}

static PORT_C_INLINE bool eventQIsEmpty(
    const struct epaEventQ * eventQ) {

    return (esQpIsEmpty(&eventQ->qp));
}

static PORT_C_INLINE bool eventQIsFull(
    const struct epaEventQ * eventQ) {

    return (esQpIsFull(&eventQ->qp));
}

static PORT_C_INLINE void ** eventQBuff(
    const struct epaEventQ * eventQ) {

    return (esQpBuff(&eventQ->qp));
}

/*--  Scheduler  -------------------------------------------------------------*/

static PORT_C_INLINE void schedInit(
    void) {

    esPqInit(&GlobalEpaKernel.sched.ready);
    esSlsSentinelInit(&GlobalEpaKernel.sched.regSentinel);
}

static PORT_C_INLINE void schedTerm(
    void) {

    esPqTerm(&GlobalEpaKernel.sched.ready);
}

static PORT_C_INLINE void schedElementInit(
    struct epaSchedElem * elem,
    uint_fast8_t        priority) {

    esPqElementInit(&elem->pqElem, priority);
    esSlsNodeInit(&elem->regNode);
    esSlsNodeAddHead(&GlobalEpaKernel.sched.regSentinel, &elem->regNode);
}

static PORT_C_INLINE void schedElementTermI(
    struct epaSchedElem * elem) {

    esPqElementTerm(&elem->pqElem);
    esSlsNodeRm(&GlobalEpaKernel.sched.regSentinel, &elem->regNode);
}

static PORT_C_INLINE void schedReadyAddI(
    struct epaSchedElem * elem) {

    esPqAdd(&GlobalEpaKernel.sched.ready, &elem->pqElem);
}

static PORT_C_INLINE void schedReadyRmI(
    struct epaSchedElem * elem) {

    esPqRm(&elem->pqElem);
}

static PORT_C_INLINE void schedReadyRmSafeI(
    struct epaSchedElem * elem) {

    if (esPqGetContainer_(&elem->pqElem) == NULL) {
        esPqRm(&elem->pqElem);
    }
}

static PORT_C_INLINE bool schedReadyIsEmptyI(
    void) {

    return (esPqIsEmpty(&GlobalEpaKernel.sched.ready));
}

static PORT_C_INLINE struct esEpa * schedReadyGetHighestI(
    void) {

    uint_fast8_t        prio;
    struct esEpa *      epa;
    struct esPqElem *   elem;

    elem = esPqGetHighest(&GlobalEpaKernel.sched.ready);
    prio = esPqGetPriority_(elem);
    (void)esPqRotate(&GlobalEpaKernel.sched.ready, prio);
    epa  = (struct esEpa *)((uint8_t *)elem - offsetof(struct esEpa, schedElem));

    return (epa);
}

static PORT_C_INLINE bool schedIsEmptyI(
    void) {

    return (esSlsIsEmpty(&GlobalEpaKernel.sched.regSentinel));
}

static PORT_C_INLINE void schedSetCurrentI(
    struct esEpa *      epa) {

    GlobalEpaKernel.sched.epa = epa;
}

static PORT_C_INLINE struct esEpa * schedGetHead(
    void) {

    struct esEpa *      epa;

    epa = ES_SLS_NODE_ENTRY(
        struct esEpa,
        schedElem.regNode,
        GlobalEpaKernel.sched.regSentinel.next);

    return (epa);
}

/*--  EPA support  -----------------------------------------------------------*/

static PORT_C_INLINE esError epaSendEventI(
    struct esEpa *      epa,
    struct esEvent *    event) {

    if (esEventRefGet(event) < ES_EVENT_REF_LIMIT) {
        esEventRefUp(event);

        if (eventQIsEmpty(&epa->eventQ) == true) {
            schedReadyAddI(&epa->schedElem);
            eventQPutItemI(&epa->eventQ, event);

            return (ES_ERROR_NONE);
        } else if (eventQIsFull(&epa->eventQ) == false) {
            eventQPutItemI(&epa->eventQ, event);

            return (ES_ERROR_NONE);
        } else {
            esEventDestroyI(event);

            return (ES_ERROR_NO_MEMORY);
        }
    }

    return (ES_ERROR_NO_REFERENCE);
}

static PORT_C_INLINE esError epaSendAheadEventI(
    struct esEpa *      epa,
    struct esEvent *    event) {

    if (esEventRefGet(event) < ES_EVENT_REF_LIMIT) {
        esEventRefUp(event);

        if (eventQIsEmpty(&epa->eventQ) == true) {
            schedReadyAddI(&epa->schedElem);
            eventQPutAheadItemI(&epa->eventQ, event);

            return (ES_ERROR_NONE);
        } else if (eventQIsFull(&epa->eventQ) == false) {
            eventQPutAheadItemI(&epa->eventQ, event);

            return (ES_ERROR_NONE);
        } else {
            esEventDestroyI(event);

            return (ES_ERROR_NO_MEMORY);
        }
    }

    return (ES_ERROR_NO_REFERENCE);
}

static PORT_C_INLINE struct esEvent * epaFetchEventI(
    struct esEpa *      epa) {

    struct esEvent *    event;

    event = eventQGetItemI(&epa->eventQ);

    if (eventQIsEmpty(&epa->eventQ) == true) {
        schedReadyRmI(&epa->schedElem);
    }

    return(event);
}

/*--  Kernel support  --------------------------------------------------------*/

static void kernelIdle(
    void) {

}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esEpaKernelInit(
    void) {

    schedInit();
    GlobalEpaKernel.idle  = kernelIdle;
    GlobalEpaKernel.state = KERNEL_STOPPED;

    return (ES_ERROR_NONE);
}

esError esEpaKernelTerm(
    void) {

    esError             error;

    while (schedIsEmptyI() != true) {
        error = esEpaDestroy(schedGetHead());

        if (error != ES_ERROR_NONE) {

            return (ES_ERROR_NOT_PERMITTED);
        }
    }
    schedTerm();

    return (ES_ERROR_NONE);
}

esError esEpaKernelStart(
    void) {

    esIntrCtx           intrCtx;

    GlobalEpaKernel.state = KERNEL_STARTED;
    ES_CRITICAL_LOCK_ENTER(&intrCtx);

    while (GlobalEpaKernel.state == KERNEL_STARTED) {

        while (schedReadyIsEmptyI() == false) {
            struct esEpa *   epa;
            struct esEvent * event;
            esAction         action;

            epa = schedReadyGetHighestI();
            schedSetCurrentI(epa);
            event = epaFetchEventI(epa);
            ES_CRITICAL_LOCK_EXIT(intrCtx);
            ES_API_ENSURE_INTERNAL(esSmDispatch(epa->sm, event, &action));
            ES_CRITICAL_LOCK_ENTER(&intrCtx);

            if (action == ES_ACTION_DEFFERED) {
                epaSendEventI(epa, event);
            }
            ES_API_ENSURE_INTERNAL(esEventDestroyI(event));
        }
        schedSetCurrentI(NULL);
        ES_CRITICAL_LOCK_EXIT(intrCtx);
        GlobalEpaKernel.idle();
        ES_CRITICAL_LOCK_ENTER(&intrCtx);
    }
    ES_CRITICAL_LOCK_EXIT(intrCtx);

    return (ES_ERROR_NONE);
}

esError esEpaKernelStop(
    void) {

    GlobalEpaKernel.state = KERNEL_STOPPED;

    return (ES_ERROR_NONE);
}

esError esEpaKernelSetIdle(
    void (* idle)(void)) {

    if (idle == NULL) {
        idle = kernelIdle;
    }
    GlobalEpaKernel.idle = idle;

    return (ES_ERROR_NONE);
}

esError esEpaKernelGetIdle(
    void (** idle)(void)) {

    ES_API_REQUIRE(ES_API_POINTER, idle != NULL);

    if (GlobalEpaKernel.idle == kernelIdle) {
        *idle = NULL;
    } else {
        *idle = GlobalEpaKernel.idle;
    }

    return (ES_ERROR_NONE);
}

esError esEpaKernelGetCurrent(
    struct esEpa **     epa) {

    *epa = GlobalEpaKernel.sched.epa;

    return (ES_ERROR_NONE);
}

esError esEpaCreate(
    const struct esEpaDefine * epaDefine,
    const struct esSmDefine * smDefine,
    struct esMem *      mem,
    struct esEpa **     epa) {

    esError             error;
    esIntrCtx           intrCtx;
    struct esSm *       sm;
    void **             qBuff;

    ES_API_REQUIRE(ES_API_POINTER, epaDefine != NULL);
    ES_API_REQUIRE(ES_API_POINTER, smDefine != NULL);
    ES_API_REQUIRE(ES_API_POINTER, epa != NULL);

    error = esMemAlloc(mem, sizeof(struct esEpa), (void **)epa);

    if (error != ES_ERROR_NONE) {
        goto ERROR_ALLOC_EPA;
    }
    error = esMemAlloc(mem, ES_QP_SIZEOF(epaDefine->queueSize), (void **)&qBuff);

    if (error != ES_ERROR_NONE) {
        goto ERROR_ALLOC_QBUFF;
    }
    error = esSmCreate(smDefine, mem, &sm);

    if (error != ES_ERROR_NONE) {
        goto ERROR_ALLOC_SM;
    }
    (*epa)->mem  = mem;
    (*epa)->sm   = sm;
    (*epa)->name = epaDefine->name;
    schedElementInit(&(*epa)->schedElem, epaDefine->priority);
    eventQInit(&(*epa)->eventQ, qBuff, epaDefine->queueSize);
    ES_CRITICAL_LOCK_ENTER(&intrCtx);
    schedReadyAddI(&(*epa)->schedElem);
    eventQPutItemI(&(*epa)->eventQ, ES_SMP_EVENT(ES_INIT));
    ES_CRITICAL_LOCK_EXIT(intrCtx);

    ES_API_OBLIGATION((*epa)->signature = EPA_SIGNATURE);

    return (ES_ERROR_NONE);
ERROR_ALLOC_SM:
    ES_API_ENSURE_INTERNAL(esSmDestroy(sm));
ERROR_ALLOC_QBUFF:
    ES_API_ENSURE_INTERNAL(esMemFree(mem, epa));
ERROR_ALLOC_EPA:

    return (ES_ERROR_NO_MEMORY);
}

esError esEpaDestroy(
    struct esEpa *      epa) {

    esError             error;
    esIntrCtx           intrCtx;
    void **             qBuff;

    ES_API_REQUIRE(ES_API_POINTER, epa != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  epa->signature == EPA_SIGNATURE);
    ES_API_OBLIGATION(epa->signature = (esAtomic)~EPA_SIGNATURE);

    ES_CRITICAL_LOCK_ENTER(&intrCtx);

    while (eventQIsEmpty(&epa->eventQ) != true) {
        struct esEvent * event;

        event = eventQGetItemI(&epa->eventQ);
        esEventDestroyI(event);
    }
    schedReadyRmSafeI(&epa->schedElem);
    schedElementTermI(&epa->schedElem);
    ES_CRITICAL_LOCK_EXIT(intrCtx);

    qBuff = eventQBuff(&epa->eventQ);
    eventQTerm(&epa->eventQ);
    error = esSmDestroy(epa->sm);

    if (error != ES_ERROR_NONE) {
        goto ERROR_FREE_SM;
    }
    error = esMemFree(epa->mem, qBuff);

    if (error != ES_ERROR_NONE) {
        goto ERROR_FREE_QBUFF;
    }
    error = esMemFree(epa->mem, epa);

    if (error != ES_ERROR_NONE) {
        goto ERROR_FREE_EPA;
    }

    return (ES_ERROR_NONE);
ERROR_FREE_EPA:
ERROR_FREE_QBUFF:
ERROR_FREE_SM:

    return (ES_ERROR_NOT_PERMITTED);
}

esError esEpaSendEventI(
    struct esEpa *      epa,
    struct esEvent *    event) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, epa != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  epa->signature == EPA_SIGNATURE);

    error = epaSendEventI(epa, event);

    return (error);
}

esError esEpaSendEvent(
    struct esEpa *      epa,
    struct esEvent *    event) {

    esError             error;
    esIntrCtx           intrCtx;

    ES_API_REQUIRE(ES_API_POINTER, epa != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  epa->signature == EPA_SIGNATURE);

    ES_CRITICAL_LOCK_ENTER(&intrCtx);
    error = epaSendEventI(epa, event);
    ES_CRITICAL_LOCK_EXIT(intrCtx);

    return (error);
}


esError esEpaSendAheadEventI(
    struct esEpa *      epa,
    struct esEvent *    event) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, epa != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  epa->signature == EPA_SIGNATURE);

    error = epaSendAheadEventI(epa, event);

    return (error);
}

esError esEpaSendAheadEvent(
    struct esEpa *      epa,
    struct esEvent *    event) {

    esError             error;
    esIntrCtx           intrCtx;

    ES_API_REQUIRE(ES_API_POINTER, epa != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  epa->signature == EPA_SIGNATURE);

    ES_CRITICAL_LOCK_ENTER(&intrCtx);
    error = epaSendAheadEventI(epa, event);
    ES_CRITICAL_LOCK_EXIT(intrCtx);

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.c
 ******************************************************************************/
