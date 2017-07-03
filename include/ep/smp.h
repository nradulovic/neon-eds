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

/**
 * @brief       Helper macro to validate SM signature
 * @notapi
 */
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
#define N_IS_SM_OBJECT(sm_obj)                                                  \
    (NSIGNATURE_OF(sm_obj) == NSIGNATURE_SM)
#else
#define N_IS_SM_OBJECT(sm_obj)          (sm_obj)
#endif

/**
 * @brief       Helper macro for SM memory pointer
 * @notapi
 */
#if (CONFIG_DYNAMIC_SM == 1) || defined(__DOXYGEN__)
#define N_SM_MEM                        .mem = NULL,
#else
#define N_SM_MEM
#endif

/**@brief       State machine define structure initializator
 * @details     This is a convinience macro for filling up the @ref nsm
 *              structure.
 * @api
 */
#define NSM_INITIALIZER(wspace_ptr, init_state_ptr, type_enum)                  \
    {                                                                           \
        NSIGNATURE_INITIALIZER(NSIGNATURE_SM)                                   \
        N_SM_MEM                                                                \
        .vf_dispatch = type_enum == NSM_TYPE_HSM ? n_sm_hsm_dispatch :          \
                                              n_sm_fsm_dispatch,                \
        .state = init_state_ptr,                                                \
        .wspace = wspace_ptr                                                    \
    }

/**
 * @brief       State machine bundle structure
 * @param       name
 *              C expression : This is a name of new structure that will be 
 *              created.
 * @param       wspace_struct
 *              C expression : This is workspace structure type. Pass this 
 *              argument as 'struct my_wspace' then macro will declare a member 
 *              in bundle structure of this type.
 * @details     Bundle structure ties up together state machine structure and
 *              user definable workspace structure into a single structure.
 * @api
 */
#define NSM_BUNDLE_STRUCT(name, wspace_struct)                                  \
    struct name {                                                               \
        struct nsm              b;                                              \
        wspace_struct           workspace;                                      \
    }

/**
 * @brief       Declare state machine bundle structure
 * @param       name
 *              C expression : This is a name of new structure that will be 
 *              created.
 * @param       wspace_struct
 *              C expression : This is workspace structure type. Pass this 
 *              argument as 'struct my_wspace' then macro will declare a member 
 *              in bundle structure of this type.
 * @details     Bundle structure ties up together state machine structure and
 *              user definable workspace structure into a single structure. This
 *              macro will declare a variable of this structure type.
 * @api
 */
#define NSM_BUNDLE_DECLARE(name, wspace_struct)                                 \
    NSM_BUNDLE_STRUCT(name, wspace_struct) name
   
/**
 * @brief       Initialize state machine bundle structure
 * @param       wspace_struct
 *              C expression : This is workspace structure type. Pass this 
 *              argument as 'struct my_wspace' then macro will declare a member 
 *              in bundle structure of this type.
 * @param       init_state_ptr
 *              Function pointer : pointer to function that represents state 
 *              machine initial state.
 * @param       type_enum
 *              Enumerator : type of state machine, see @ref NSM_TYPE_HSM and
 *              @ref NSM_TYPE_FSM
 * @details     Bundle structure ties up together state machine structure and
 *              user definable workspace structure into a single structure. This
 *              macro will initialize a variable of this structure type.
 * @api
 */ 
#define NSM_BUNDLE_INITIALIZER(name, init_state_ptr, type_enum)           \
    {                                                                           \
        .b = NSM_INITIALIZER(&name.workspace, init_state_ptr, type_enum),            \
    }
    
/**
 * @brief       Define a state machine bundle
 * @param       name
 *              C expression : This is a name of new structure that will be 
 *              created.
 * @param       wspace_struct
 *              C expression : This is workspace structure type. Pass this 
 *              argument as 'struct my_wspace' then macro will declare a member 
 *              in bundle structure of this type.
 * @param       init_state_ptr
 *              Function pointer : pointer to function that represents state 
 *              machine initial state.
 * @param       type_enum
 *              Enumerator : type of state machine, see @ref NSM_TYPE_HSM and
 *              @ref NSM_TYPE_FSM
 * @details     Bundle structure ties up together state machine structure and
 *              user definable workspace structure into a single structure. This
 *              macro will declare and initialize a variable of this structure 
 *              type.
 * @api
 */     
#define NSM_BUNDLE_DEFINE(name, wspace_struct, init_state_ptr, type_enum)       \
    NSM_BUNDLE_DECLARE(name, wspace_struct) =                                   \
        NSM_BUNDLE_INITIALIZER(name, init_state_ptr, type_enum)      

/**
 * @brief       Get the state machine workspace pointer
 * @api
 */
#define nsm_wspace(sm)                  ((sm)->wspace)

/**
 * @brief       State machine action, given event was handled.
 * @return      Actions enumerator @ref NACTION_HANDLED.
 * @api
 */
#define naction_handled()               (NACTION_HANDLED)

/**
 * @brief       State machine action, given event was ignored.
 * @return      Actions enumerator @ref NACTION_IGNORED.
 * @api
 */
#define naction_ignored()               (NACTION_IGNORED)

/**
 * @brief       State machine action, state is returning its super state
 * @param       sm
 *              Pointer to the state machine
 * @param       state_ptr
 *              State function pointer to super state
 * @return      Actions enumerator @ref NACTION_SUPER.
 * @api
 */
#define naction_super(sm, state_ptr)                                            \
    ((sm)->state = (state_ptr), NACTION_SUPER)

/**
 * @brief       State machine action, state machine wants to transit to new
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

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/* NOTE:
 * Forward declarations of nmem and nevent structures
 */
struct nsm;
struct nevent;

/**
 * @brief       State machine event identifications
 * @api
 */
enum nsm_event
{
    NSM_SUPER           = 0u,           /**<@brief Get the state super state  */
    NSM_ENTRY           = 1u,           /**<@brief Process state entry        */
    NSM_EXIT            = 2u,           /**<@brief Process state exit         */
    NSM_INIT            = 3u,           /**<@brief Process state init         */
    NSM_NULL            = 14u,          /**<@brief NULL event                 */
    NEVENT_USER_ID      = 15u
};

/**
 * @brief       State machine types
 * @api
 */
enum nsm_type
{
    NSM_TYPE_HSM        = 0u,           /**<@brief Hierarchical State Machine */
    NSM_TYPE_FSM        = 1u            /**<@brief Finite State Machine       */
};

/**
 * @brief       Returned actions enumerator
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
    NACTION_IGNORED     = 3u            /**<@brief Event is ignored           */
};

/**
 * @brief       State machine action type
 * @api
 */
typedef enum naction naction;

/**
 * @brief       State function prototype
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
 * @note        Do not use return enumerator directly but use the appropriate
 *              naction_*() function.
 * @api
 */
typedef naction  (nstate) (struct nsm *, const struct nevent *);

/**
 * @brief       State machine structure
 * @api
 */
struct nsm
{
    NSIGNATURE_DECLARE                  /**<@brief State machine signature    */
#if (CONFIG_DYNAMIC_SM == 1) || defined(__DOXYGEN__)
    struct nmem *               mem;
#endif
                                        /**<@brief State machine dispatcher   */
    void                     (* vf_dispatch)(struct nsm *, const struct nevent *);
    nstate *                    state;  /**<@brief Current state              */
    void *                      wspace;
};

/**
 * @brief       State machine type
 * @api
 */
typedef struct nsm nsm;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

#if (CONFIG_DYNAMIC_SM == 1) || defined(__DOXYGEN__)
/**
 * @brief       Dynamically allocate state machine
 * @note        To use this API call the configuration option 
 *              @ref CONFIG_DYNAMIC_SM must be enabled.
 * @api
 */ 
struct nsm * nsm_alloc(struct nmem * mem, size_t wspace_size, 
    nstate * init_state, enum nsm_type type);
#endif

#if (CONFIG_DYNAMIC_SM == 1) || defined(__DOXYGEN__)
/**
 * @brief       Free state machine which was allocated with @ref nsm_alloc
 * @note        To use this API call the configuration option 
 *              @ref CONFIG_DYNAMIC_SM must be enabled.
 * @api
 */
void nsm_free(struct nsm * sm);
#endif

/**
 * @brief       Dispatch a state machine
 * @api
 */ 
PORT_C_INLINE
void nsm_dispatch(struct nsm * sm, const struct nevent * event)
{
    sm->vf_dispatch(sm, event);
}

/**
 * @brief       HSM top state definition
 * @api
 */
naction ntop_state(struct nsm * sm, const struct nevent * event);

/**
 * @brief       Get predefined SM events
 * @api
 */
const struct nevent * nsm_event(enum nsm_event event_id);

/**
 * @brief       HSM dispatch function
 * @notapi
 */
void n_sm_hsm_dispatch(struct nsm * sm, const struct nevent * event);

/**
 * @brief       FSM dispatch function
 * @notapi
 */
void n_sm_fsm_dispatch(struct nsm * sm, const struct nevent * event);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp.h
 ******************************************************************************/
#endif /* NEON_EP_SMP_H_ */
