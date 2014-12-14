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

#define HSM_STATE_SUPER(state, sm)                                              \
    state((sm), NSMP_EVENT(NSMP_SUPER)), (sm)->state

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

/*
 * NOTE: We don't use indexed intialization here, so it must be enusured that 
 *       the order of events in this array match the enumerator nsmp_events.
 */
const struct nevent esGlobalSmEvents[4] =
{
    {
        NSMP_SUPER,
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
        NSMP_ENTRY,
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
        NSMP_EXIT,
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
        NSMP_INIT,
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
        index->enter = 0u;

        return;
    }
/*--  path: e) source ?== ...super(super(destination))  ----------------------*/
    index->enter = 1u;

    while (destination[index->enter] != &ntop_state) {
        nstate *                current_state;

        current_state = HSM_STATE_SUPER(sm, destination[index->enter++]);
        destination[index->enter] = current_state;

        if (source[0] == current_state) {

            return;
        }
    }
/*--  path: f) super(source) ?== ...super(super(destination))  ---------------*/
    index->exit++;

    while (index->enter != 2u) {

        if (source[1] == destination[index->enter]) {

            return;
        }
        index->enter--;
    }
/*--  path: g) ...super(super(source)) ?== ...super(super(destination))  -----*/

    for (;;) {
        nstate *                current_state;

        current_state = HSM_STATE_SUPER(sm, source[index->exit++]);
        source[index->exit] = current_state;
        index->exit = 0u;

        while (destination[index->enter] != &ntop_state) {
            index_enter++;

            if (source[index->exit] == destination[index->enter]) {

                return;
            }
        }
    }
}



static void hsm_path_enter(struct nsm * sm, uint_fast8_t index) 
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



static void hsm_path_exit(struct nsm * sm, uint_fast8_t index)
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
    nstate *                    current_state;
    struct hsm_path_index       index;

    index.exit = 0u;
    current_state = sm->state;

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
        current_state    = sm->path_exit[0];
        index.exit = 1u;
    }
    sm->state = current_state;
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

    if (sm_define->type == NTYPE_HSM) {
        sm->dispatch   = &hsm_dispatch;
    } else {
        sm->dispatch   = &fsm_dispatch;
    }
    NOBLIGATION(sm->signature = SM_SIGNATURE);
}



void nsm_term(struct nsm * sm)
{
    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == SM_SIGNATURE);
    sm->dispatch = NULL;
    sm->state    = NULL;
    sm->wspace   = NULL;
    NOBLIGATION(sm->signature = ~SM_SIGNATURE);
}



naction nsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == SM_SIGNATURE);

    return (sm->dispatch(sm, event));
}



naction ntop_state(struct nsm * sm, const struct nevent * event)
{
    (void)nsm;
    (void)event;

    return (NACTION_IGNORED);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of smp.c
 ******************************************************************************/
