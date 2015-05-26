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

/**@brief       Get the state machine workspace pointer
 * @api
 */
#define nsm_wspace(sm)                  ((sm)->wspace)

/**@brief       Set the state machine workspace pointer
 * @api
 */
#define nsm_set_wspace(sm, wspace_ptr)  (sm)->wspace = (wspace_ptr)

/**@brief       State machine action, state is returning its super state
 * @param       sm
 *              Pointer to the state machine
 * @param       state_ptr
 *              State function pointer to super state
 * @return      Actions enumerator @ref NACTION_SUPER.
 * @api
 */
#define naction_super(sm, state_ptr)                                            \
    ((sm)->state = (state_ptr), NACTION_SUPER)

/**@brief       State machine action, state machine wants to transit to new
 *              state
 * @param       sm
 *              Pointer to the state machine
 * @param       state_ptr
 *              State function pointer to new state
 * @return      Actions enumerator @ref NACTION_TRANSIT_TO.
 * @api
 */
#define naction_transit_to(sm, state_ptr)                                       \
    ((sm)->state = (state_ptr), NACTION_TRANSIT_TO)

/**@brief       State machine action, given event was handled.
 * @return      Actions enumerator @ref NACTION_HANDLED.
 * @api
 */
#define naction_handled()               (NACTION_HANDLED)

/**@brief       State machine action, given event was ignored.
 * @return      Actions enumerator @ref NACTION_IGNORED.
 * @api
 */
#define naction_ignored()               (NACTION_IGNORED)

/**@brief       State machine action, given event will be deffered.
 * @return      Actions enumerator @ref NACTION_DEFERRED.
 * @api
 */
#define naction_deferred()              (NACTION_DEFERRED)

/**@brief       State machine define structure initializator
 * @details     This is a convinience macro for filling up the @ref nsm_define
 *              structure.
 * @api
 */
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
 * @api
 */
enum nsm_type
{
    NSM_TYPE_HSM        = 0u,           /**<@brief Hierarchical State Machine */
    NSM_TYPE_FSM        = 1u            /**<@brief Finite State Machine       */
};

/**@brief       Returned actions enumerator
 * @details     This enumerator is for internal use only. It defines what
 *              actions should dispatcher execute for the given state machine.
 * @note        Do not use this enumerator directly, but use the appropriate
 *              naction_*() function.
 * @napi
 */
enum naction
{
    NACTION_SUPER       = 0u,           /**<@brief Returns super state        */
    NACTION_TRANSIT_TO  = 1u,           /**<@brief Transit to a state         */
    NACTION_HANDLED     = 2u,           /**<@brief Event is handled           */
    NACTION_IGNORED     = 3u,           /**<@brief Event is ignored           */
    NACTION_DEFERRED    = 4u            /**<@brief Defer this event           */
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
 * @return      Action enumerator of the specified state. Action enumerator can
 *              be one of the following:
 *              - @ref NACTION_SUPER - state is returning its super state
 *              - @ref NACTION_TRANSIT_TO - state machine wants to transit to
 *                  new state
 *              - @ref NACTION_HANDLED - given event was handled
 *              - @ref NACTION_IGNORED - given event was ignored
 *              - @ref NACTION_DEFERRED - given event must be deffered
 * @note        Do not use return enumerator directly but use the appropriate
 *              naction_*() function.
 * @api
 */
typedef naction (nstate) (struct nsm *, const struct nevent *);

/**@brief       State machine structure
 * @api
 */
struct nsm
{
                                        /**<@brief State machine dispatcher   */
    naction                  (* vf_dispatch)(struct nsm *, const struct nevent *);
    nstate *                    state;  /**<@brief Current state              */
    void *                      wspace; /**<@brief Workspace                  */
#if (CONFIG_API_VALIDATION == 1)
                                        /**<@brief State machine signature    */
    unsigned int                signature;
#endif
};

/**@brief       State machine type
 * @api
 */
typedef struct nsm nsm;

/**@brief       State machine definition structure
 * @api
 */
struct nsm_define
{
    void *                      wspace; /**<@brief Workspace pointer          */
                                        /**<@brief Initial state pointer      */
    nstate *                    init_state;
    enum nsm_type               type;   /**<@brief State machine type         */
};

/**@brief       State machine definition type
 * @api
 */
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
    enum nsm_event              event_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp.h
 ******************************************************************************/
#endif /* NEON_EP_SMP_H_ */
