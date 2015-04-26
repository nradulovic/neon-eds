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
 * @defgroup    ep_epa Event Processing Agent
 * @brief       Event Processing Agent
 *********************************************************************//** @{ */

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

struct nepa_define
{
    struct nsm_define           sm;
    struct nequeue_define       working_queue;
    struct nequeue_define       deffered_queue;
    struct nthread_define       thread;
};

typedef struct nepa_define nepa_define;

struct nepa
{
    struct nmem *               mem;
    struct nthread              thread;  /**<@brief Thread                    */
    struct nsm                  sm;
    struct nequeue              working_queue;
    struct nequeue              deferred_queue;
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    unsigned int                signature;
#endif
};

typedef struct nepa nepa;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        EPA General functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Start the event processing loop
 */
void neds_run(void);



void neds_set_idle(
    void                     (* idle)(void));



PORT_C_INLINE
struct nepa * nepa_get_current(void)
{
    return (NTHREAD_TO_EPA(nsched_get_current()));
}



void * nepa_create_storage(size_t size);



void nepa_delete_storage(void * storage);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA management
 * @{ *//*--------------------------------------------------------------------*/


void nepa_init(
    struct nepa *               epa,
    const struct nepa_define *  define);



void nepa_term(
    struct nepa *               epa);



struct nepa * nepa_create(
    const struct nepa_define *  define_epa,
    struct nmem *               mem);



void nepa_destroy(
    struct nepa *               epa);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Event transport
 * @{ *//*--------------------------------------------------------------------*/


nerror nepa_send_event_i(
    struct nepa *               epa,
    struct nevent *             event);



nerror nepa_send_event(
    struct nepa *               epa,
    struct nevent *             event);



nerror nepa_send_event_ahead_i(
    struct nepa *               epa,
    struct nevent *             event);



nerror nepa_send_event_ahead(
    struct nepa *               epa,
    struct nevent *             event);



nerror nepa_signal_i(
    struct nepa *               epa,
    uint16_t                    event_id);



nerror nepa_signal(
    struct nepa *               epa,
    uint16_t                    event_id);

/**@} *//*------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa.h
 ******************************************************************************/
#endif /* NEON_EP_EPA_H_ */
