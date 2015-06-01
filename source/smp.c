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
 * @brief       State Machine Processor Implementation
 * @addtogroup  ep_impl
 *********************************************************************//** @{ */
/**@defgroup    ep_impl Implementation
 * @brief       State Machine Processor Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

#include "base/component.h"
#include "base/debug.h"
#include "ep/event.h"
#include "ep/smp.h"

/*=========================================================  LOCAL DEFINES  ==*/

#define NSMP_EVENT(event)               &g_smp_events[(event)]

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct hsm_path
{
    nstate *                    buff[CONFIG_SMP_HSM_PATH_DEPTH];
    uint_fast8_t                index;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static nstate * hsm_get_state_super(struct nsm * sm, nstate * state);



/**@brief       Finds transition path
 * @param       sm
 *              State machine
 * @param       srcCount
 *              Index for source starting position
 * @return      Index for destination starting position
 * @notapi
 */
static void hsm_build_path(
    struct nsm *                sm,
    struct hsm_path *           entry,
    struct hsm_path *           exit);



static void hsm_path_enter(struct nsm * sm, const struct hsm_path * entry);



static void hsm_path_exit(struct nsm * sm, const struct hsm_path * exit);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("State Machine Processor");

/*
 * NOTE: We don't use indexed initialisation here, so it must be ensured that
 *       the order of events in this array match the enumerator nsmp_events.
 */
static const struct nevent g_smp_events[4] =
{
    {
        NSM_SUPER,
        0,
        0,
        NULL,
#if (CONFIG_EVENT_PRODUCER == 1) || defined(__DOXYGEN__)
        NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)     || defined(__DOXYGEN__)
        sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
        NSIGNATURE_EVENT
#endif
    }, {
        NSM_ENTRY,
        0,
        0,
        NULL,
#if (CONFIG_EVENT_PRODUCER == 1) || defined(__DOXYGEN__)
        NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)     || defined(__DOXYGEN__)
        sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
        NSIGNATURE_EVENT
#endif
    }, {
        NSM_EXIT,
        0,
        0,
        NULL,
#if (CONFIG_EVENT_PRODUCER == 1) || defined(__DOXYGEN__)
        NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)     || defined(__DOXYGEN__)
        sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
        NSIGNATURE_EVENT
#endif
    }, {
        NSM_INIT,
        0,
        0,
        NULL,
#if (CONFIG_EVENT_PRODUCER == 1) || defined(__DOXYGEN__)
        NULL,
#endif
#if (CONFIG_EVENT_SIZE == 1)     || defined(__DOXYGEN__)
        sizeof(struct nevent),
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
        NSIGNATURE_EVENT
#endif
    }
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static nstate * hsm_get_state_super(struct nsm * sm, nstate * state)
{
#if (CONFIG_API_VALIDATION == 1)
    naction                     ret;

    ret = state(sm, NSMP_EVENT(NSM_SUPER));

    NREQUIRE(NAPI_USAGE, ret == NACTION_SUPER);
#else
    state(sm, NSMP_EVENT(NSM_SUPER));
#endif

    return (sm->state);
}



static void hsm_build_path(
    struct nsm *                sm,
    struct hsm_path *           entry,
    struct hsm_path *           exit)
{
    nstate *                    current_state;
    entry->index = 0;
    exit->index--;

/*--  path: a) source ?== destination  ---------------------------------------*/
    if (exit->buff[exit->index] == entry->buff[entry->index]) {
        entry->index++;
        exit->index++;

        return;
    }
/*--  path: b) source ?== super(destination)  --------------------------------*/
    current_state = hsm_get_state_super(sm, entry->buff[entry->index++]);
    entry->buff[entry->index] = current_state;

    if (exit->buff[exit->index] == entry->buff[entry->index]) {

        return;
    }
/*--  path: c) super(source) ?== super(destination)  -------------------------*/
    current_state = hsm_get_state_super(sm, exit->buff[exit->index++]);
    exit->buff[exit->index] = current_state;

    if (exit->buff[exit->index] == entry->buff[entry->index]) {

        return;
    }
/*--  path: d) super(source) ?== destination  --------------------------------*/
    entry->index--;

    if (exit->buff[exit->index] == entry->buff[entry->index]) {

        return;
    }
/*--  path: e) source ?== ...super(super(destination))  ----------------------*/
    exit->index--;
    entry->index++;

    while (entry->buff[entry->index] != &ntop_state) {
        current_state = hsm_get_state_super(sm, entry->buff[entry->index++]);
        entry->buff[entry->index] = current_state;

        if (exit->buff[exit->index] == entry->buff[entry->index]) {

            return;
        }
    }
/*--  path: f) super(source) ?== ...super(super(destination))  ---------------*/
    exit->index++;

    while (entry->index != 1u) {

        if (exit->buff[exit->index] == entry->buff[entry->index]) {

            return;
        }
        entry->index--;
    }
/*--  path: g) ...super(super(source)) ?== ...super(super(destination))  -----*/

    for (;;) {
        current_state = hsm_get_state_super(sm, exit->buff[exit->index++]);
        exit->buff[exit->index] = current_state;
        entry->index = 0u;

        while (entry->buff[entry->index] != &ntop_state) {

            if (exit->buff[exit->index] == entry->buff[entry->index]) {

                return;
            }
            entry->index++;
        }
    }
}



static void hsm_path_enter(struct nsm * sm, const struct hsm_path * entry)
{
    uint_fast8_t                index = entry->index;

    while (index--) {
#if (CONFIG_API_VALIDATION == 1)
        naction                 ret;

        ret = entry->buff[index](sm, NSMP_EVENT(NSM_ENTRY));
        NREQUIRE(NAPI_USAGE, (ret == NACTION_IGNORED) || 
                             (ret == NACTION_HANDLED) ||
                             (ret == NACTION_SUPER));
#else
        (void)entry->buff[index](sm, NSMP_EVENT(NSM_ENTRY));
#endif
    }
}



static void hsm_path_exit(struct nsm * sm, const struct hsm_path * exit)
{
    uint_fast8_t                count;

    for (count = 0; count < exit->index; count++) {
#if (CONFIG_API_VALIDATION == 1)
        naction                 ret;

        ret = exit->buff[count](sm, NSMP_EVENT(NSM_EXIT));
        NREQUIRE(NAPI_USAGE, (ret == NACTION_IGNORED) || 
                             (ret == NACTION_HANDLED) ||
                             (ret == NACTION_SUPER));
#else
        (void)exit->buff[count](sm, NSMP_EVENT(NSM_EXIT));
#endif
    }
}



static naction hsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    naction                     ret;
    nstate *                    current_state;
    struct hsm_path             entry;
    struct hsm_path             exit;

    exit.index = 0u;
    current_state = sm->state;

    do {
        exit.buff[exit.index++] = sm->state;
        ret = sm->state(sm, event);
    } while (ret == NACTION_SUPER);

    while (ret == NACTION_TRANSIT_TO) {
        entry.buff[0] = sm->state;
        hsm_build_path(sm, &entry, &exit);
        hsm_path_exit(sm, &exit);
        hsm_path_enter(sm, &entry);
        ret = entry.buff[0](sm, NSMP_EVENT(NSM_INIT));
        exit.buff[0]  = entry.buff[0];
        current_state = entry.buff[0];
        exit.index = 1u;
    }
    sm->state = current_state;

    return (ret);
}



static naction fsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    naction                     ret;
    nstate *                    current_state;

    current_state = sm->state;

    while ((ret = current_state(sm, event)) == NACTION_TRANSIT_TO) {
#if (CONFIG_API_VALIDATION == 1)
        ret = current_state(sm, NSMP_EVENT(NSM_EXIT));
        NREQUIRE(NAPI_USAGE, (ret == NACTION_IGNORED) ||
                             (ret == NACTION_HANDLED));
#else
        (void)current_state(sm, NSMP_EVENT(NSM_EXIT));
#endif
        current_state = sm->state;
#if (CONFIG_API_VALIDATION == 1)
        ret = current_state(sm, NSMP_EVENT(NSM_ENTRY));
        NREQUIRE(NAPI_USAGE, (ret == NACTION_IGNORED) ||
                             (ret == NACTION_HANDLED));
#else
        (void)current_state(sm, NSMP_EVENT(NSM_ENTRY));
#endif
        event = NSMP_EVENT(NSM_INIT);
    }
    NREQUIRE(NAPI_USAGE, ret != NACTION_SUPER);
    sm->state = current_state;

    return (ret);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nsm_init(struct nsm * sm, const struct nsm_define * sm_define)
{
    NREQUIRE(NAPI_POINTER, sm                    != NULL);
    NREQUIRE(NAPI_POINTER, sm->signature         != NSIGNATURE_SM);
    NREQUIRE(NAPI_POINTER, sm_define             != NULL);
    NREQUIRE(NAPI_POINTER, sm_define->init_state != NULL);
    NREQUIRE(NAPI_USAGE, (sm_define->type == NSM_TYPE_FSM) ||
                         (sm_define->type == NSM_TYPE_HSM));

    sm->state  = sm_define->init_state;
    sm->wspace = sm_define->wspace;

    if (sm_define->type == NSM_TYPE_HSM) {
        sm->vf_dispatch = &hsm_dispatch;
    } else {
        sm->vf_dispatch = &fsm_dispatch;
    }
    NOBLIGATION(sm->signature = NSIGNATURE_SM);
}



void nsm_term(struct nsm * sm)
{
    NREQUIRE(NAPI_POINTER, sm != NULL);
    NREQUIRE(NAPI_OBJECT,  sm->signature == NSIGNATURE_SM);

    sm->vf_dispatch = NULL;
    sm->state       = NULL;
    sm->wspace      = NULL;
    NOBLIGATION(sm->signature = ~NSIGNATURE_SM);
}



naction ntop_state(struct nsm * sm, const struct nevent * event)
{
    (void)sm;
    (void)event;

    return (NACTION_IGNORED);
}



const struct nevent * nsm_event(enum nsm_event event_id)
{
    return (&g_smp_events[event_id]);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of smp.c
 ******************************************************************************/
