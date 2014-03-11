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
 * @brief       Implementacija State Machine Processor objekta.
 * @addtogroup  smp_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include "eds/smp.h"

/*=========================================================  LOCAL DEFINES  ==*/

/**
 * @brief       State machine signature
 * @details     This signature is used only during the debugging process
 */
#define SM_SIGNATURE                   ((esAtomic)0xdaafu)

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct esSm {
    struct esMem *      mem;
    const struct esSmTable * table;
    void *              wspace;
    esAction *          src;
    esAction *          dst;
#if (CONFIG_API_VALIDATION == 1)
    esAtomic            signature;
#endif
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

#if (CONFIG_API_VALIDATION == 1) || (CONFIG_SMP_HSM == 1)
static uint_fast8_t smFindDepth(
    const struct esSmTable * table);
#endif

#if (CONFIG_SMP_HSM == 1)
/**@brief       Finds transition path
 * @param       sm
 *              State machine
 * @param       srcCount
 *              Index for source starting position
 * @return      Index for destination starting position
 * @notapi
 */
static uint_fast8_t smFindPath(
    struct esSm *       sm,
    uint_fast8_t        srcCount);

static PORT_C_INLINE void smPathEnter(
    struct esSm *       sm,
    esAction *          entry);

static PORT_C_INLINE void smPathExit(
    struct esSm *       sm,
    esAction *          exit);
#endif

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("SMP", "State Machine Processor", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

const PORT_C_ROM struct esEvent esGlobalSmEvents[] = {
    {
         ES_ENTRY,
         ES_EVENT_CONST_Msk,
         NULL,
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
         NULL,
#endif
#if (CONFIG_EVENT_TIMESTAMP == 1)      || defined(__DOXYGEN__)
         0,
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
         sizeof(struct esEvent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
         ES_EVENT_SIGNATURE
#endif
    },
    {
         ES_EXIT,
         ES_EVENT_CONST_Msk,
         NULL,
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
         NULL,
#endif
#if (CONFIG_EVENT_TIMESTAMP == 1)      || defined(__DOXYGEN__)
         0,
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
         sizeof(struct esEvent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
         ES_EVENT_SIGNATURE
#endif
    },
    {
         ES_INIT,
         ES_EVENT_CONST_Msk,
         NULL,
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
         NULL,
#endif
#if (CONFIG_EVENT_TIMESTAMP == 1)      || defined(__DOXYGEN__)
         0,
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
         sizeof(struct esEvent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
         ES_EVENT_SIGNATURE
#endif
    }
};


/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

#if (CONFIG_API_VALIDATION == 1) || (CONFIG_SMP_HSM == 1)
static uint_fast8_t smFindDepth(
    const struct esSmTable * table) {

    esAction            cnt;
    uint_fast8_t        depth;

    cnt   = 1u;
    depth = 0u;

    while ((cnt <= (esAction)UINT8_MAX) && (table[cnt].state != NULL)) {
        esAction        inner;
        uint_fast8_t    level;

        inner = cnt;
        level = 2u;

        while ((inner <= (esAction)UINT8_MAX) && (table[inner].super != 0) &&
               (table[inner].state != NULL)) {
            inner = table[inner].super;
            level++;
        }

        if (depth < level) {
            depth = level;
        }
        cnt++;
    }

    if (cnt == UINT8_MAX) {
        depth = 0;
    }

    return (depth);
}
#endif

#if (CONFIG_SMP_HSM == 1)

static uint_fast8_t smFindPath(
    struct esSm *       sm,
    uint_fast8_t        srcCount) {

    uint_fast8_t        dstCount;
    esAction *          source;
    esAction *          destination;

    source      = &sm->src[srcCount];
    destination = &sm->dst[0];

/*--  path: a) source ?== destination  ---------------------------------------*/
    if (source[0] == destination[1]) {
        source[1] = 0;

        return (1u);
    }
/*--  path: b) source ?== super(destination)  --------------------------------*/
    destination[2] = sm->table[destination[1]].super;

    if (source[0] == destination[2]) {
        source[0] = 0;

        return (1u);
    }
/*--  path: c) super(source) ?== super(destination)  -------------------------*/
    source[1] = sm->table[source[0]].super;

    if (source[1] == destination[2]) {
        source[1] = 0;

        return (1u);
    }
/*--  path: d) super(source) ?== destination  --------------------------------*/

    if (source[1] == destination[1]) {
        source[2] = 0;

        return (0u);
    }
/*--  path: e) source ?== ...super(super(destination))  ----------------------*/
    dstCount = 2u;

    while (0 != destination[dstCount]) {
        esAction       stateId;

        stateId = destination[dstCount++];
        destination[dstCount] = sm->table[stateId].super;

        if (source[0] == destination[dstCount]) {
            source[0] = 0;

            return (dstCount);
        }
    }
/*--  path: f) super(source) ?== ...super(super(destination))  ---------------*/

    while (dstCount != 2u) {

        if (source[1] == destination[dstCount]) {
            source[1] = 0;

            return (dstCount);
        }
        dstCount--;
    }
/*--  path: g) ...super(super(source)) ?== ...super(super(destination))  -----*/
    srcCount = 1u;

    while (true) {
        esAction       stateId;

        stateId = source[srcCount++];
        source[srcCount] = sm->table[stateId].super;
        dstCount = 0u;

        while (0 != destination[dstCount]) {
            dstCount++;

            if (source[srcCount] == destination[dstCount]) {
                source[srcCount] = 0;

                return (dstCount);
            }
        }
    }
    return (0);
}

static PORT_C_INLINE void smPathEnter(
    struct esSm *       sm,
    esAction *          entry) {

    while (*entry != 0) {
#if (CONFIG_API_VALIDATION == 1)
        esAction        ret;

        ret = sm->table[*entry].state(sm->wspace, ES_SMP_EVENT(ES_ENTRY));
        ES_REQUIRE(
            ES_API_USAGE,
            (ret == ES_ACTION_IGNORED) ||
            (ret == ES_ACTION_HANDLED));
        --entry;
#else
        (void)sm->table[*entry].state(sm->wspace, ES_SMP_EVENT(ES_ENTRY));
#endif
    }
}

static PORT_C_INLINE void smPathExit(
    struct esSm *       sm,
    esAction *          exit) {

    while (*exit != 0) {
#if (CONFIG_API_VALIDATION == 1)
        esAction        ret;

        ret = sm->table[*exit].state(sm->wspace, ES_SMP_EVENT(ES_EXIT));
        ES_REQUIRE(
            ES_API_USAGE,
            ((ret > ES_ACTION_TOP)     &&
             (ret < ES_ACTION_BOTTOM)) ||
            (ret == ES_ACTION_IGNORED) ||
            (ret == ES_ACTION_HANDLED));
#else
        (void)sm->table[*exit].state(sm->wspace, ES_SMP_EVENT(ES_EXIT));
#endif
        ++exit;
    }
}
#endif /* (CONFIG_SMP_HSM == 1) */

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esSmCreate(
    const struct esSmDefine * define,
    struct esMem *      mem,
    struct esSm **      sm) {

    esError             error;

    ES_REQUIRE(ES_API_POINTER, define != NULL);
    ES_REQUIRE(ES_API_POINTER, define->table != NULL);
    ES_REQUIRE(ES_API_POINTER, sm != NULL);

    error = esMemAlloc(mem, sizeof(struct esSm), (void **)sm);

    if (error != ES_ERROR_NONE) {
        goto ERROR_ALLOC_SM;
    }
    (*sm)->mem    = mem;
    (*sm)->table  = define->table;
    (*sm)->wspace = NULL;

    if (define->wspaceSize != 0) {
        size_t          size;

        error = esMemGetSize(mem, &size);

        if ((error != ES_ERROR_NONE) || (size < define->wspaceSize)) {
            goto ERROR_ALLOC_WSPACE;
        }
        error = esMemAlloc(mem, define->wspaceSize, &(*sm)->wspace);                                                           /* We force the compiler to use array sizes                 */

        if (error != ES_ERROR_NONE) {
            goto ERROR_ALLOC_WSPACE;
        }
    }
#if (CONFIG_SMP_HSM == 1)
    {
        uint_fast8_t    depth;
        esAction *      levels;

        depth = smFindDepth(define->table);
        ES_REQUIRE(ES_API_RANGE, depth >= 2);
        ++depth;
        error = esMemAlloc(
            mem,
            depth * sizeof(esAction [2]),
            (void **)&levels);

        if (error != ES_ERROR_NONE) {
            goto ERROR_ALLOC_LEVELS;
        }
        (*sm)->src = &levels[0];
        (*sm)->dst = &levels[depth];

        while (depth != 0u) {
            depth--;
            (*sm)->src[depth] = 0;
            (*sm)->dst[depth] = 0;
        }
    }
    (*sm)->src[0] = define->initState;
#else
    {
        esAction *      levels;

        ES_REQUIRE(ES_API_RANGE, smFindDepth(define->table) == 2);

        error = esMemAlloc(
            mem,
            sizeof(esAction [2]),
            (void **)&levels);

        if (error != ES_ERROR_NONE) {
            goto ERROR_ALLOC_LEVELS;
        }
        (*sm)->src = &levels[0];
        (*sm)->dst = &levels[1];
        (*sm)->src[0] = 0;
        (*sm)->dst[0] = 0;
    }
    (*sm)->src[0] = define->initState;
#endif /* (CONFIG_SMP_HSM == 1) */
    ES_OBLIGATION((*sm)->signature = SM_SIGNATURE);

    return (ES_ERROR_NONE);
ERROR_ALLOC_LEVELS:
    if ((*sm)->wspace != NULL) {
        ES_ENSURE_INTERNAL(esMemFree((*sm)->mem, (*sm)->wspace));
    }
ERROR_ALLOC_WSPACE:
    ES_ENSURE_INTERNAL(esMemFree((*sm)->mem, *sm));
ERROR_ALLOC_SM:

    return (ES_ERROR_NO_MEMORY);
}

esError esSmDestroy(
    struct esSm *       sm) {

    esError             error;

    ES_REQUIRE(ES_API_POINTER, sm != NULL);
    ES_REQUIRE(ES_API_OBJECT,  sm->signature == SM_SIGNATURE);
    ES_OBLIGATION(sm->signature = (esAtomic)~SM_SIGNATURE);

    error = esMemFree(sm->mem, sm->src);

    if (error != ES_ERROR_NONE) {
        goto ERROR_FREE_LEVELS;
    }
    ES_OBLIGATION(sm->src = NULL);
    ES_OBLIGATION(sm->dst = NULL);
    error = esMemFree(sm->mem, sm);

    if (error != ES_ERROR_NONE) {
        goto ERROR_FREE_SM;
    }

    return (ES_ERROR_NONE);

ERROR_FREE_SM:
ERROR_FREE_LEVELS:

    return (ES_ERROR_NOT_PERMITTED);
}

esError esSmDispatch(
    struct esSm *       sm,
    struct esEvent *    event,
    esAction *          action) {

#if (CONFIG_SMP_HSM == 1)
    uint_fast8_t        count;
    esAction            ret;
    esAction            id;

    ES_REQUIRE(ES_API_POINTER, sm != NULL);
    ES_REQUIRE(ES_API_OBJECT,  sm->signature == SM_SIGNATURE);
    ES_REQUIRE(ES_API_POINTER, action != NULL);

    count = 0u;

    do {
        id = sm->src[count++];
        sm->src[count] = sm->table[id].super;
        ret = sm->table[id].state(sm->wspace, event);
        ES_REQUIRE(
            ES_API_USAGE,
            ((ret > ES_ACTION_TOP) &&
             (ret < ES_ACTION_BOTTOM)) ||
            (ret == ES_ACTION_IGNORED) ||
            (ret == ES_ACTION_HANDLED) ||
            (ret == ES_ACTION_DEFFERED));
    } while ((ret == ES_ACTION_IGNORED) && (sm->table[id].super != 0));
    count--;

    while (ret > ES_ACTION_TOP) {
        sm->dst[1] = ret;
        count = smFindPath(sm, count);
        smPathExit(sm, &sm->src[0]);
        smPathEnter(sm, &sm->dst[count]);
        ret = sm->table[sm->dst[1]].state(sm->wspace, ES_SMP_EVENT(ES_INIT));
        ES_REQUIRE(
            ES_API_USAGE,
            ((ret > ES_ACTION_TOP) &&
             (ret < ES_ACTION_BOTTOM)) ||
            (ret == ES_ACTION_IGNORED) ||
            (ret == ES_ACTION_HANDLED));
        sm->src[0] = sm->dst[1];
        count = 0u;
    }
    *action = ret;

    return (ES_ERROR_NONE);
#else /* (CONFIG_SMP_HSM == 1) */
    esAction            ret;

    ES_REQUIRE(ES_API_POINTER, sm != NULL);
    ES_REQUIRE(ES_API_OBJECT,  sm->signature == SM_SIGNATURE);
    ES_REQUIRE(ES_API_POINTER, action != NULL);

    ret = sm->table[sm->src[0]].state(sm->wspace, (struct esEvent *)event);
    ES_REQUIRE(
        ES_API_USAGE,
        ((ret > ES_ACTION_TOP) &&
         (ret < ES_ACTION_BOTTOM)) ||
        (ret == ES_ACTION_IGNORED) ||
        (ret == ES_ACTION_HANDLED) ||
        (ret == ES_ACTION_DEFFERED));

    while (ret > 0) {
        sm->dst[0] = ret;
        (void)sm->table[sm->src[0]].state(sm->wspace, ES_SMP_EVENT(ES_EXIT));
        (void)sm->table[sm->dst[0]].state(sm->wspace, ES_SMP_EVENT(ES_ENTRY));
        ret = sm->table[sm->dst[0]].state(sm->wspace, ES_SMP_EVENT(ES_INIT));
        ES_REQUIRE(
            ES_API_USAGE,
            ((ret > ES_ACTION_TOP) &&
             (ret < ES_ACTION_BOTTOM)) ||
            (ret == ES_ACTION_IGNORED) ||
            (ret == ES_ACTION_HANDLED));
        sm->src[0] = sm->dst[0];
    }
    *action = ret;

    return (ES_ERROR_NONE);
#endif /* !(CONFIG_SMP_HSM == 1) */
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of smp.c
 ******************************************************************************/
