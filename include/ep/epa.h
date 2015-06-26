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
#include "base/config.h"
#include "sched/sched.h"
#include "ep/smp.h"
#include "ep/equeue.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Validate the pointer to Event Processing Agent object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#define N_IS_EPA_OBJECT(epa_obj)                                                \
    (((epa_obj) != NULL) && ((epa_obj)->signature == NSIGNATURE_EPA))

/**@brief       Returns EPA address from pointer to thread
 */
#define NTHREAD_TO_EPA(thread_ptr)                                              \
    PORT_C_CONTAINER_OF(thread_ptr, struct nepa, thread)

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

/**@brief       Define structure for an EPA object
 * @details     Use this structure to define an EPA object. This structure can
 *              reside in flash memory and once the EPA object is created this
 *              structure is not used.
 * @api
 */
struct nepa_define
{
    struct nsm_define           sm;     /**<@brief State machine define       */
    struct nequeue_define       working_queue;
                                        /**<@brief Working event queue define */
    struct nthread_define       thread;
                                        /**<@brief Thread definition          */
};

/**@brief       Define type for EPA object
 * @api
 */
typedef struct nepa_define nepa_define;

/**@brief       EPA object
 * @api
 */
struct nepa
{
    struct nmem *               mem;    /**<@brief Memory instance pointer    */
    struct nthread              thread; /**<@brief Thread                     */
    struct nsm                  sm;     /**<@brief State machine processor    */
    struct nequeue              working_queue;
                                        /**<@brief Working event queue        */
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    unsigned int                signature;
                                        /**<@brief Debug signature            */
#endif
};

/**@brief       EPA object type
 * @api
 */
typedef struct nepa nepa;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        EPA General functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Start the event processing loop
 */
void neds_run(void);



/**@brief       Set an user implementation of idle routine.
 * @param       idle
 *              Pointer to idle routine. If this pointer is NULL the portable
 *              idle routine will be used instead. Usually the portable idle
 *              routine will put the CPU in sleep state.
 * @details     Idle routine is called when there is no available EPA for 
 *              execution. Idle routine can be used to run user loop code which
 *              is typically used by USB, TCP/IP frameworks.
 * @api
 */
void neds_set_idle(
    void                     (* idle)(void));



/**@brief       Get currently executed EPA object pointer
 * @api
 */
PORT_C_INLINE
struct nepa * nepa_get_current(void)
{
    return (NTHREAD_TO_EPA(nsched_get_current()));
}



/**@brief       Allocate memory from memory instance which was used to create
 *              the EPA object
 * @api
 */
void * nepa_create_storage(size_t size);



/**@brief       Return allocated memory
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

#define nepa_defer_init(queue, define)     nequeue_init((queue), (define))

nerror nepa_defer_event(struct nequeue * queue, const struct nevent * event);

/**@brief       Fetch one deferred event and put it into working queue
 * @api
 */
nerror nepa_defer_fetch_one(
    struct nequeue *            queue);



/**@brief       Fetch all deferred events and put them into working queue
 * @api
 */
nerror nepa_defer_fetch_all(
    struct nequeue *            queue);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA management
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Initialize EPA
 * @api
 */
void nepa_init(
    struct nepa *               epa,
    const struct nepa_define *  define);



/**@brief       Terminate EPA
 * @api
 */
void nepa_term(
    struct nepa *               epa);



/**@brief       Create and initialize EPA
 * @api
 */
struct nepa * nepa_create(
    const struct nepa_define *  define_epa,
    struct nmem *               mem);



/**@brief       Terminate and destroy EPA
 * @api
 */
void nepa_destroy(
    struct nepa *               epa);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Event transport
 * @{ *//*--------------------------------------------------------------------*/


nerror nepa_send_event_i(
    struct nepa *               epa,
    const struct nevent *       event);



nerror nepa_send_event(
    struct nepa *               epa,
    const struct nevent *       event);



nerror nepa_send_event_ahead_i(
    struct nepa *               epa,
    struct nevent *             event);



nerror nepa_send_event_ahead(
    struct nepa *               epa,
    struct nevent *             event);



nerror nepa_send_signal_i(
    struct nepa *               epa,
    uint16_t                    event_id);



nerror nepa_send_signal(
    struct nepa *               epa,
    uint16_t                    event_id);

/**@} *//*------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.h
 ******************************************************************************/
#endif /* NEON_EP_EPA_H_ */
