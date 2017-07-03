/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2017 Nenad Radulovic
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
 * @brief       Event Processing Agent header
 * @addtogroup  ep_intf
 *********************************************************************//** @{ */
/**@defgroup    ep_epa Event Processing Agent
 * @brief       Event Processing Agent
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_EP_EPA_H_
#define NEON_EP_EPA_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "port/compiler.h"
#include "base/error.h"
#include "base/queue.h"
#include "base/config.h"
#include "sched/sched.h"
#include "ep/smp.h"

/*===============================================================  MACRO's  ==*/

/**
 * @brief       Helper macro to validate EPA signature
 * @notapi
 */
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
#define N_IS_EPA_OBJECT(epa_obj)                                                \
    (NSIGNATURE_OF(epa_obj) == NSIGNATURE_EPA)
#else
#define N_IS_EPA_OBJECT(epa_obj)        (epa_obj)
#endif

/**
 * @brief       Helper macro for EPA memory pointer
 * @notapi
 */
#if (CONFIG_DYNAMIC_EPA == 1) || defined(__DOXYGEN__)
#define N_EPA_MEM                       .mem = NULL,
#else
#define N_EPA_MEM
#endif

/**
 * @brief       Get the pointer to EPA from thread structure
 * @notapi
 */
#define NP_THREAD_TO_EPA(thread_ptr)                                            \
    PORT_C_CONTAINER_OF(thread_ptr, struct nepa, thread)


#define NEPA_BUNDLE_STRUCT(name, queue_size, wspace_struct)                     \
    struct name {                                                               \
        struct nepa         b;                                              	\
        NSM_BUNDLE_DECLARE(sm, wspace_struct); 				                    \
        NQUEUE_BUNDLE_DECLARE(equeue, queue_size);                              \
    }
    
#define NEPA_BUNDLE_DECLARE(name, queue_size, wspace_struct)             \
    NEPA_BUNDLE_STRUCT(name, queue_size, wspace_struct) name
    

#define NEPA_INITIALIZER(name, prio, equeue_ptr, sm_ptr)                        \
    {                                                                           \
        N_EPA_MEM                                                               \
        NSIGNATURE_INITIALIZER(NSIGNATURE_EPA)                                  \
        .thread = NTHREAD_INITIALIZER(name.b.thread, NULL, prio),                         \
        .sm = (sm_ptr),                                                           \
        .queue = (equeue_ptr),                                                       \
    }
    
#define NEPA_BUNDLE_DEFINE(name, queue_size, priority, wspace_struct, init_state_ptr, type_enum)        \
	NEPA_BUNDLE_DECLARE(name, queue_size, wspace_struct) = {             \
		.b = NEPA_INITIALIZER(name, priority, &name.equeue.b, &name.sm.b),            	\
		.sm = NSM_BUNDLE_INITIALIZER(name.sm, init_state_ptr, type_enum), \
		.equeue = NQUEUE_BUNDLE_INITIALIZER(name.equeue),                              \
	}

#define NEPA_BUNDLE_EXPORT(name)												\
	&name.b

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/* NOTE:
 * Forward declarations of nmem and nevent structures
 */
struct nmem;
struct nevent;

/**
 * @brief       EPA object
 * @api
 */
struct nepa
{
    NSIGNATURE_DECLARE       			/**<@brief Debug signature            */
#if (CONFIG_DYNAMIC_EPA == 1) || defined(__DOXYGEN__)
	struct nmem *               mem;    /**<@brief Memory instance pointer    */
#endif
	struct nthread              thread; /**<@brief Thread                     */
	struct nsm *                sm;     /**<@brief State machine processor    */
	struct nqueue *             queue;
										/**<@brief Working event queue        */
};

/**
 * @brief       EPA object type
 * @api
 */
typedef struct nepa nepa;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
    
/*------------------------------------------------------------------------*//**
 * @name        EPA management
 * @{ *//*--------------------------------------------------------------------*/
 
 
/**
 * @brief       Create and initialize EPA
 * @api
 */
#if (CONFIG_DYNAMIC_EPA == 1) || defined(__DOXYGEN__)
struct nepa * nepa_alloc(struct nmem * mem, const char * name, size_t q_size, 
    uint8_t prio, size_t wspace_size, nstate * init, enum nsm_type type);
#endif



/**
 * @brief       Terminate and destroy EPA
 * @api
 */
#if (CONFIG_DYNAMIC_EPA == 1) || defined(__DOXYGEN__)
void nepa_free(struct nepa * epa);
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA General functions
 * @{ *//*--------------------------------------------------------------------*/


/**
 * @brief       Register event processing agent to scheduler
 * @api
 */
void nepa_register(struct nepa * epa);



/**
 * @brief       Get currently executed EPA object pointer
 * @api
 */
PORT_C_INLINE
struct nepa * nepa_get_current(void)
{
    return (NP_THREAD_TO_EPA(nthread_get_current()));
}



/**
 * @brief       Allocate memory from memory instance which was used to create
 *              the EPA object
 * @api
 */
void * nepa_create_storage(size_t size);



/**
 * @brief       Return allocated memory
 * @api
 */
void nepa_delete_storage(void * storage);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Deferred event management
 * @details     When a SM returns naction_deffered() action the currently 
 *              processed event will be deferred. This means that the event will
 *              be transferred to deferred event queue. When the SM is ready to
 *              process new events it can fetch one or all events from deferred
 *              queue.
 * @{ *//*--------------------------------------------------------------------*/


/**
 * @brief       Defer one event to specific queue
 * @api
 */
nerror nepa_defer_event(struct nqueue * queue, const struct nevent * event);



/**
 * @brief       Fetch one deferred event and put it into working queue
 * @api
 */
nerror nepa_defer_fetch_one(struct nqueue * queue);



/**
 * @brief       Fetch all deferred events and put them into working queue
 * @api
 */
nerror nepa_defer_fetch_all(struct nqueue * queue);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Event transport
 * @{ *//*--------------------------------------------------------------------*/

nerror nepa_send_event_i(struct nepa * epa, const struct nevent * event);

nerror nepa_send_event(struct nepa * epa, const struct nevent * event);

nerror nepa_send_event_ahead_i(struct nepa * epa, struct nevent * event);

nerror nepa_send_event_ahead(struct nepa * epa, struct nevent * event);

nerror nepa_send_signal_i(struct nepa * epa, uint16_t event_id);

nerror nepa_send_signal(struct nepa * epa, uint16_t event_id);

/**@} *//*------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.h
 ******************************************************************************/
#endif /* NEON_EP_EPA_H_ */
