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

struct sched_deferred_sentinel
{
	struct nsched_deferred *	head;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Deferred work");

/*======================================================  GLOBAL VARIABLES  ==*/

struct sched_deferred_sentinel 	g_ctx;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nsched_deferred_init(struct nsched_deferred * deferred, void (* fn)(void *), void * arg)
{
	static bool 				is_initialized;

	NREQUIRE(NAPI_POINTER, deferred != NULL);
	NREQUIRE(NAPI_POINTER, fn != NULL);

	if (!is_initialized) {
		is_initialized = true;

		ncore_deferred_init();
	}
	deferred->fn = fn;
	deferred->arg = arg;
	deferred->next = NULL;
}



void nsched_deferred_do(struct nsched_deferred * deferred)
{
	g_ctx.head = deferred;

	ncore_deferred_do();
}



void ncore_deferred_work(void)
{
	struct nsched_deferred *	deferred;

	deferred = g_ctx.head;

	if (deferred) {
		deferred->fn(deferred->arg);
	}
	g_ctx.head = NULL;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of sched.c
 ******************************************************************************/
