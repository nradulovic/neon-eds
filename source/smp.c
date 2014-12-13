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

#include "shared/component.h"
#include "eds/event.h"
#include "eds/smp.h"

/*=========================================================  LOCAL DEFINES  ==*/

/**
 * @brief       State machine signature
 * @details     This signature is used only during the debugging process
 */
#define SM_SIGNATURE                    ((ndebug_magic)0xdaafu)

#define HSM_STATE_SUPER(state, sm)      state(sm, NSMP_EVENT(NSMP_SUPER)), sm->state

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct hsm_path_index
{
    uint_fast8_t                exit;
    uint_fast8_t                enter;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

#if (CONFIG_SMP_HSM == 1)
static uint_fast8_t sm_compute_depth(
    const struct nsmTable * table);
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
static uint_fast8_t hsm_build_path(
    struct nsm *       sm,
    uint_fast8_t        srcCount);

PORT_C_INLINE
void hsm_path_enter(
    struct nsm *       sm,
    naction *          entry);

PORT_C_INLINE
void hsm_path_exit(
    struct nsm *       sm,
    naction *          exit);
#endif

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("State Machine Processor", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

const struct nevent esGlobalSmEvents[3] =
{
    {
         ES_ENTRY,
         NEVENT_ATTR_CONST,
         0,
         NULL,
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
         NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
         sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
         NEVENT_SIGNATURE
#endif
    },
    {
         ES_EXIT,
         NEVENT_ATTR_CONST,
         0,
         NULL,
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
         NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
         sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
         NEVENT_SIGNATURE
#endif
    },
    {
         ES_INIT,
         NEVENT_ATTR_CONST,
         0,
         NULL,
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
         NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
         sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
         NEVENT_SIGNATURE
#endif
    }
};


/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

#if (CONFIG_SMP_HSM == 1)

static uint_fast8_t hsm_build_path(
    struct nsm *                sm,
    struct hsm_path_index *     index) 
{
    naction *          source;
    naction *          destination;

    source      = &sm->path_exit[index->exit - 1u];
    destination = &sm->path_enter[0];

/*--  path: a) source ?== destination  ---------------------------------------*/
    if (source[0] == destination[0]) {
        index->enter = 1u;

        return;
    }
/*--  path: b) source ?== super(destination)  --------------------------------*/
    destination[1] = HSM_STATE_SUPER(sm, destination[0]);

    if (source[0] == destination[1]) {
        index->exit--;
        index->enter = 1;

        return;
    }
/*--  path: c) super(source) ?== super(destination)  -------------------------*/
    source[1] = HSM_STATE_SUPER(sm, source[0]);

    if (source[1] == destination[1]) {
        index->enter = 1u;

        return;
    }
/*--  path: d) super(source) ?== destination  --------------------------------*/

    if (source[1] == destination[0]) {
        index->exit++;
        index->enter = 0u;

        return;
    }
/*--  path: e) source ?== ...super(super(destination))  ----------------------*/
    index->enter = 1u;

    while (destination[index->enter]) {
        nstate *                current_state;

        current_state = HSM_STATE_SUPER(sm, destination[index->enter++]);
        destination[index->enter] = current_state;

        if (source[0] == current_state) {

            return;
        }
    }
/*--  path: f) super(source) ?== ...super(super(destination))  ---------------*/
    index->exit++;

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
        naction       stateId;

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



PORT_C_INLINE
void hsm_path_enter(struct nsm * sm, uint_fast8_t index) 
{
    while (index--) {
#if (CONFIG_API_VALIDATION == 1)
        naction                 ret;

        ret = sm->path_enter[index](sm, NSMP_EVENT(NSMP_ENTER));
        NREQUIRE(NAPI_USAGE, (ret == NACTION_IGNORED) || 
                             (ret == NACTION_DEFFERED));
#else
        (void)sm->path_enter[index](sm, NSMP_EVENT(NSMP_ENTER));
#endif
    }
}



PORT_C_INLINE
void hsm_path_exit(struct nsm * sm, uint_fast8_t index)
{
    uint_fast8_t                count;

    for (count = 0; count < index; count++) {
#if (CONFIG_API_VALIDATION == 1)
        naction                 ret;

        ret = sm->path_exit[count](sm, NSMP_EVENT(NSMP_EXIT));
        NREQUIRE(NAPI_USAGE, (ret == NACTION_IGNORED) || 
                             (ret == NACTION_DEFFERED));
#else
        (void)sm->path_exit[count](sm, NSMP_EVENT(NSMP_EXIT));
#endif
    }
}



static naction hsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    naction                     ret;
    struct hsm_path_index       index;
    index.exit = 0u;

    do {
        sm->path_exit[index.exit++] = sm->state;
        ret = sm->state(sm, event);
    } while (ret == NACTION_SUPER);

    while (ret == NACTION_TRANSIT_TO) {
        sm->path_enter[0] = sm->state;
        hsm_build_path(sm, &index);
        hsm_path_exit(sm, index.exit);
        hsm_path_enter(sm, index.enter);
        ret = sm->path_enter[0](sm, NSMP_EVENT(NSMP_INIT));
        sm->path_exit[0] = sm->path_enter[0];
        index.exit = 1u;
    }
}

static naction fsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    naction                     ret;
    nstate *                    current_state;

    current_state = sm->state;

    while ((ret = current_state(sm, event)) == NACTION_TRANSIT_TO) {
#if (CONFIG_API_VALIDATION == 1)
        ret = current_state(sm, NSMP_EVENT(NSMP_EXIT));
        NREQUIRE(NAPI_USAGE, (ret != NACTION_DEFFERED) &&
                             (ret != NACTION_SUPER)    &&
                             (ret != NACTION_TRANSIT_TO));
#else
        (void)current_state(sm, NSMP_EVENT(NSMP_EXIT);
#endif
        current_state = sm->state;
#if (CONFIG_API_VALIDATION == 1)
        ret = current_state(sm, NSMP_EVENT(NSMP_ENTRY));
        NREQUIRE(NAPI_USAGE, (ret != NACTION_DEFFERED) &&
                             (ret != NACTION_SUPER)    &&
                             (ret != NACTION_TRANSIT_TO));
#else
        (void)current_state(sm, NSMP_EVENT(NSMP_ENTRY));
#endif
        event = NSMP_EVENT(NSMP_INIT);
    }
    NREQUIRE(NAPI_USAGE, ret != NACTION_SUPER);
    sm->state = current_state;

    return (ret);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void nsm_init(struct nsm * sm, const struct nsm_define * sm_define)
{
    NREQUIRE(NAPI_POINTER, sm                    != NULL);
    NREQUIRE(NAPI_POINTER, sm->signature         != SM_SIGNATURE);
    NREQUIRE(NAPI_POINTER, sm_define             != NULL);
    NREQUIRE(NAPI_POINTER, sm_define->init_state != NULL);

    sm->state  = sm_define->init_state;
    sm->wspace = sm_define->wspace;

    if (sm_define->path_buffer) {
        uint32_t                path_buffer_elements;

        NREQUIRE(NAPI_RANGE,   sm_define->path_buffer_size >= sizeof(nstate *));
        path_buffer_elements = sm_define->path_buffer_size /  sizeof(nstate *);
        memset(sm_define->path_buffer, 0u, sm_define->path_buffer_size);
        sm->dispatch   = &hsm_dispatch;
        sm->path_exit  = &sm_define->path_buffer[0u];
        sm->path_enter = &sm_define->path_buffer[path_buffer_elements / 2u];
    } else {
        sm->dispatch   = &fsm_dispatch;
        sm->path_exit  = NULL;
        sm->path_enter = NULL;
    }
    NOBLIGATION(sm->signature = SM_SIGNATURE);
}



void nsm_term(struct nsm * sm)
{
    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == SM_SIGNATURE);
    sm->dispatch = NULL;
    NOBLIGATION(sm->signature = ~SM_SIGNATURE);
}



naction nsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == SM_SIGNATURE);

    return (sm->dispatch(sm, event));
}



esError esSmDispatch(
    struct nsm *       sm,
    struct esEvent *    event,
    naction *          action) {

#if (CONFIG_SMP_HSM == 1)
    uint_fast8_t        count;
    naction            ret;
    naction            id;

    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == SM_SIGNATURE);
    NREQUIRE(NAPI_POINTER, action != NULL);

    count = 0u;

    do {
        id = sm->src[count++];
        sm->src[count] = sm->table[id].super;
        ret = sm->table[id].state(sm->wspace, event);
        NREQUIRE(
            NAPI_USAGE,
            ((ret > ES_ACTION_TOP) &&
             (ret < ES_ACTION_BOTTOM)) ||
            (ret == ES_ACTION_IGNORED) ||
            (ret == ES_ACTION_HANDLED) ||
            (ret == ES_ACTION_DEFFERED));
    } while ((ret == ES_ACTION_IGNORED) && (sm->table[id].super != 0));
    count--;

    while (ret > ES_ACTION_TOP) {
        sm->dst[1] = ret;
        count = hsm_build_path(sm, count);
        hsm_path_exit(sm, &sm->src[0]);
        hsm_path_enter(sm, &sm->dst[count]);
        ret = sm->table[sm->dst[1]].state(sm->wspace, ES_SMP_EVENT(ES_INIT));
        NREQUIRE(
            NAPI_USAGE,
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
    naction            ret;

    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == SM_SIGNATURE);
    NREQUIRE(NAPI_POINTER, action != NULL);

    ret = sm->table[sm->src[0]].state(sm->wspace, (struct esEvent *)event);
    NREQUIRE(
        NAPI_USAGE,
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
        NREQUIRE(
            NAPI_USAGE,
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
