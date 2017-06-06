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
 * @brief       Deferred work implementation
 * @addtogroup  sched_deferred
 *********************************************************************//** @{ */
/**@defgroup    sched_deferred_impl Implementation
 * @brief       Deferred work Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "sched/deferred.h"
#include "base/debug.h"
#include "base/component.h"


/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct sched_deferred_ctx
{
    struct ndlist               a;
    struct ndlist               b;
    struct ndlist *             pending;
    struct ndlist *             working;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static struct nsched_deferred *
ndlist_to_deferred(struct ndlist * list);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Deferred work");

/*======================================================  GLOBAL VARIABLES  ==*/

static struct sched_deferred_ctx g_ctx;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static struct nsched_deferred *
ndlist_to_deferred(struct ndlist * list)
{
    return (PORT_C_CONTAINER_OF(list, struct nsched_deferred, list));
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nsched_deferred_init(struct nsched_deferred * deferred, void (* fn)(void *), void * arg)
{
    static bool                 is_initialized;

    NREQUIRE(NAPI_POINTER, deferred != NULL);
    NREQUIRE(NAPI_OBJECT, deferred->signature != NSIGNATURE_DEFER);
    NREQUIRE(NAPI_POINTER, fn != NULL);

    if (!is_initialized) {
        is_initialized = true;

        ncore_deferred_init();
        g_ctx.pending = ndlist_init(&g_ctx.a);
        g_ctx.working = ndlist_init(&g_ctx.b);
    }
    deferred->fn = fn;
    deferred->arg = arg;
    ndlist_init(&deferred->list);

    NOBLIGATION(deferred->signature = NSIGNATURE_DEFER);
}



void nsched_deferred_do(struct nsched_deferred * deferred)
{
    NREQUIRE(NAPI_POINTER, deferred != NULL);
    NREQUIRE(NAPI_OBJECT, deferred->signature = NSIGNATURE_DEFER);

    if (ndlist_is_empty(&deferred->list)) {
    	ndlist_add_after(g_ctx.pending, &deferred->list);
    }
    ncore_deferred_do();
}



void ncore_deferred_work(void)
{
	struct ndlist *			tmp;
    struct ndlist *         current;

    tmp			  = g_ctx.pending;
    g_ctx.pending = g_ctx.working;
    g_ctx.working = tmp;

    /* Execute all deferred functions */

    while (!ndlist_is_empty(g_ctx.working)) {
    	struct nsched_deferred * deferred;

    	current = ndlist_next(g_ctx.working);

		deferred = ndlist_to_deferred(current);
		NREQUIRE(NAPI_OBJECT, deferred->signature = NSIGNATURE_DEFER);
		deferred->fn(deferred->arg);
		ndlist_remove(current);
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of sched.c
 ******************************************************************************/
