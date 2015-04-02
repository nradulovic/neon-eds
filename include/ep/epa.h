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
 * @defgroup    eds_epa Event Processing Agent
 * @brief       Event Processing Agent
 *********************************************************************//** @{ */

#ifndef NEON_EP_EPA_H_
#define NEON_EP_EPA_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "port/compiler.h"
#include "base/debug.h"
#include "base/error.h"
#include "base/config.h"
#include "sched/sched.h"
#include "ep/smp.h"
#include "ep/equeue.h"

/*===============================================================  MACRO's  ==*/

/**@brief       EPA structure signature.
 * @details     The signature is used to confirm that a structure passed to a
 *              function is indeed a nthread thread structure.
 */
#define EPA_SIGNATURE                       ((unsigned int)0xfeedbeeful)

#define THREAD_TO_EPA(thread_ptr)                                               \
    CONTAINER_OF(thread_ptr, struct nepa, thread)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

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
    struct nthread              thread;         /**<@brief Priority queue     */
    struct nsm                  sm;
    struct nequeue              working_queue;
    struct nequeue              deffered_queue;
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

void neds_run(void);



void neds_set_idle(
    void                     (* idle)(void));



PORT_C_INLINE
struct nepa * nepa_get_current(void)
{
    return (THREAD_TO_EPA(nsched_get_current()));
}

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

/**@} *//*------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa.h
 ******************************************************************************/
#endif /* NEON_EP_EPA_H_ */
