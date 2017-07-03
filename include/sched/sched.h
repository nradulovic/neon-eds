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
 * @brief       Scheduler
 * @defgroup    sched Scheduler
 * @brief       Scheduler
 *********************************************************************//** @{ */

#ifndef NEON_SCHED_SCHED_H_
#define NEON_SCHED_SCHED_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "base/debug.h"
#include "base/dlist.h"
#include "base/config.h"
#include "base/bias_list.h"

/*===============================================================  MACRO's  ==*/

/**
 * @brief       Helper macro to validate thread signature
 * @notapi
 */
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
#define N_IS_THREAD_OBJECT(epa_obj)                                             \
    (NSIGNATURE_OF(epa_obj) == NSIGNATURE_EPA)
#else
#define N_IS_EPA_OBJECT(epa_obj)        (epa_obj)
#endif

#if (CONFIG_REGISTRY == 1) || defined(__DOXYGEN__)
#define NP_THREAD_REGISTRY_INIT(thread_name)                                    \
    .name = # thread_name,                                                      \
    .registry_node = NDLIST_INITIALIZER(thread_name.registry_node),
#else
#define NP_THREAD_REGISTRY_INIT(name)
#endif

/**
 * @brief       Maximum level of priority possible for application thread
 * @api
 */
#define NTHREAD_PRIORITY_MAX            (CONFIG_PRIORITY_LEVELS - 1u)

/**
 * @brief       Minimum level of priority possible for application thread
 * @api
 */
#define NTHREAD_PRIORITY_MIN            (1u)

/**
 * @brief       Define thread properties
 * @api
 */
#define NTHREAD_INITIALIZER(name, dispatcher, priority)                         \
    {                                                                           \
        NSIGNATURE_INITIALIZER(NSIGNATURE_THREAD)                               \
        .node = NBIAS_LIST_INITIALIZER(name.node, priority),                    \
        .ref = 0,                                                               \
        .vf_dispatch_i = dispatcher,                                            \
        NP_THREAD_REGISTRY_INIT(name)                                           \
    }

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct ntask;
struct nthread;
struct ncore_lock;

struct nthread
{
    NSIGNATURE_DECLARE                            /**<@brief Thread signature */
    struct nbias_list           node;          /**<@brief Priority queue node */
    uint_fast32_t               ref;               /**<@brief Reference count */
    void                     (* vf_dispatch_i)(struct nthread * thread,
            struct ncore_lock *);
#if (CONFIG_REGISTRY == 1) || defined(__DOXYGEN__)
    const char *                name;
    struct ndlist               registry_node;
#endif
};

struct ntask
{
    struct nthread              thread;
    void                     (* vf_task)(struct ntask * task, void * arg);
    void *                      arg;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

#define nthread_set_dispatch(thread, dispatch)                                  \
    (thread)->vf_dispatch_i = (dispatch)


void nthread_init(struct nthread * thread, const char * name, uint8_t priority,
        void (* vf_dispatch)(struct nthread *, struct ncore_lock *));



void nthread_term(struct nthread * thread);



void nthread_insert_i(struct nthread * thread);



void nthread_remove_i(struct nthread * thread);



void nthread_schedule(void);



struct nthread * nthread_get_current(void);



void ntask_init(struct ntask * task, const char * name, uint8_t priority, 
        void (* vf_task)(struct ntask *, void *), void * arg);



#define ntask_ready_i(task)             nthread_insert_i(&(task)->thread)



#define ntask_ready(task)                                                       \
    do {                                                                        \
        struct ncore_lock       lock;                                           \
                                                                                \
        ncore_lock_enter(&lock);                                                \
        ntask_ready_i(task);                                                    \
        ncore_lock_exit(&lock);                                                 \
    } while (0)



#define ntask_block_i(task)             nthread_remove_i(&(task)->thread)



#define ntask_block(task)                                                       \
    do {                                                                        \
        struct ncore_lock       lock;                                           \
                                                                                \
        ncore_lock_enter(&lock);                                                \
        ntask_block_i(task);                                                    \
        ncore_lock_exit(&lock);                                                 \
    } while (0)

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of sched.h
 ******************************************************************************/
#endif /* NEON_SCHED_SCHED_H_ */
