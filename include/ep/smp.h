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
 * @brief       State Machine Processor header
 * @defgroup    eds_smp State Machine Processor
 * @brief       State Machine Processor
 *********************************************************************//** @{ */

#ifndef NEON_EP_SMP_H_
#define NEON_EP_SMP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "port/compiler.h"
#include "base/config.h"

/*===============================================================  MACRO's  ==*/

#define nsm_wspace(sm)                  ((sm)->wspace)

#define nsm_set_wspace(sm, wspace_ptr)  (sm)->wspace = (wspace_ptr)

#define naction_super(sm, state_ptr)                                            \
    ((sm)->state = (state_ptr), NACTION_SUPER)

#define naction_transit_to(sm, state_ptr)                                       \
    ((sm)->state = (state_ptr), NACTION_TRANSIT_TO)

#define naction_handled()                                                       \
    (NACTION_HANDLED)

#define naction_ignored()                                                       \
    (NACTION_IGNORED)

#define naction_deferred()                                                      \
    (NACTION_DEFERRED)

#define NSM_DEF_INIT(wspace, init_state, type)                                  \
    {wspace, init_state, type}

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nevent;
struct nsm;

/**@brief       State machine event identifications
 * @api
 */
enum nsm_event
{
    NSM_SUPER           = 0u,           /**<@brief Get the state super state  */
    NSM_ENTRY           = 1u,           /**<@brief Process state entry        */
    NSM_EXIT            = 2u,           /**<@brief Process state exit         */
    NSM_INIT            = 3u,           /**<@brief Process state init         */
    NEVENT_USER_ID      = 15u
};

/**@brief       State machine types
 */
enum nsm_type
{
    NSM_TYPE_HSM        = 0,            /**<@brief Hierarchical State Machine */
    NSM_TYPE_FSM        = 1             /**<@brief Finite State Machine       */
};

/**@brief       Returned actions of the state machine
 * @note        Do not use this enumerator directly but use the appropriate
 *              naction_*() function.
 * @napi
 */
enum naction
{
    NACTION_SUPER       = 0,            /**<@brief Returns super state        */
    NACTION_TRANSIT_TO  = 1,            /**<@brief Transit to a state         */
    NACTION_HANDLED     = 2,            /**<@brief Event is handled           */
    NACTION_IGNORED     = 3,            /**<@brief Event is ignored           */
    NACTION_DEFERRED    = 4             /**<@brief Defer this event           */
};

/**@brief       State machine action type
 * @api
 */
typedef enum naction naction;

/**@brief       State function prototype
 * @param       sm
 *              Pointer to state machine being executed
 * @param       event
 *              Pointer to event that has been dispatched to the state machine
 * @return
 *              Return action enumerator to the state machine dispatcher.
 */
typedef naction (nstate) (struct nsm *, const struct nevent *);

struct nsm
{
    naction                  (* vf_dispatch)(struct nsm *, const struct nevent *);
    nstate *                    state;
    void *                      wspace;
#if (CONFIG_API_VALIDATION == 1)
    unsigned int                signature;
#endif
};

typedef struct nsm nsm;

struct nsm_define
{
    void *                      wspace;
    nstate *                    init_state;
    enum nsm_type               type;
};

typedef struct nsm_define nsm_define;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


void nsm_init(
    struct nsm *                sm,
    const struct nsm_define *   sm_define);



void nsm_term(
    struct nsm *                sm);



PORT_C_INLINE
naction nsm_dispatch(
    struct nsm *                sm,
    const struct nevent *       event)
{
    return (sm->vf_dispatch(sm, event));
}



naction ntop_state(
    struct nsm *                sm,
    const struct nevent *       event);



const struct nevent * nsm_event(
    enum nsm_event            event_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp.h
 ******************************************************************************/
#endif /* NEON_EP_SMP_H_ */
