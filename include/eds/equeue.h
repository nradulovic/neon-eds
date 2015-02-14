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
 * @brief       Event queue
 * @defgroup    event_queue Event queue
 * @brief       Event queue
 *********************************************************************//** @{ */

#ifndef NEON_EDS_EQUEUE_H_
#define NEON_EDS_EQUEUE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdbool.h>

#include "port/compiler.h"
#include "port/cpu.h"
#include "shared/config.h"
#include "lib/queue.h"

/*===============================================================  MACRO's  ==*/

#define NEQUEUE_SIZEOF(elements)			(sizeof(struct nevent * [elements]))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nevent;

struct nequeue_define
{
	struct nevent **			storage;
	size_t						size;
};

struct nequeue
{
	struct nqueue				queue;
#if (CONFIG_REGISTRY == 1)
	ncpu_reg                    min;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


void nequeue_init(
	struct nequeue * 			queue,
	const struct nequeue_define * define);



void nequeue_term(
	struct nequeue * 			queue);



PORT_C_INLINE
void nequeue_put_fifo(struct nequeue * queue, const struct nevent * event)
{
#if (CONFIG_REGISTRY == 1)
	ncpu_reg					empty;

	nqueue_put_fifo(&queue->queue, (void *)event);

	empty = nqueue_free(&queue->queue);

	if (queue->min > empty) {
		queue->min = empty;
	}
#else
	nqueue_put_fifo(&queue->queue, (void *)event);
#endif
}



PORT_C_INLINE
void nequeue_put_lifo(
	struct nequeue * 			queue,
	const struct nevent * 		event)
{
#if (CONFIG_REGISTRY == 1)
	ncpu_reg					empty;

	nqueue_put_lifo(&queue->queue, (void *)event);

	empty = nqueue_free(&queue->queue);

	if (queue->min > empty) {
		queue->min = empty;
	}
#else
	nqueue_put_lifo(&queue->queue, (void *)event);
#endif
}



PORT_C_INLINE
const struct nevent * nequeue_get(
	struct nequeue * 			queue)
{
	return (nqueue_get(&queue->queue));
}



PORT_C_INLINE
bool nequeue_is_full(
	const struct nequeue * 		queue)
{
	return (nqueue_is_full(&queue->queue));
}



PORT_C_INLINE
bool nequeue_is_empty(
	const struct nequeue *		queue)
{
	return (nqueue_is_empty(&queue->queue));
}


PORT_C_INLINE
void * nequeue_storage(
	const struct nequeue * 		queue)
{
	return (nqueue_storage(&queue->queue));
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of equeue.h
 ******************************************************************************/
#endif /* NEON_EDS_EQUEUE_H_ */
