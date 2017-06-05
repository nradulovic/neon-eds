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
 * @brief       Event queue implementation
 * @addtogroup  event_queue
 *********************************************************************//** @{ */
/**@defgroup    event_queue_impl Implementation
 * @brief       Event queue Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/debug.h"
#include "base/component.h"
#include "ep/equeue.h"
#include "ep/event.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Event Queue");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void nequeue_init(
    struct nequeue *            queue,
    const struct nequeue_define * define)
{
    size_t                      size;

    NREQUIRE(NAPI_POINTER, queue != NULL);
    NREQUIRE(NAPI_OBJECT,  queue->signature != NSIGNATURE_EQUEUE);

    size = define->size / sizeof(struct nevent * [1]);

    nqueue_init(&queue->queue, define->storage, size);
#if (CONFIG_REGISTRY == 1)
    queue->min = size;
#endif
    NOBLIGATION(queue->signature = NSIGNATURE_EQUEUE);
}



void nequeue_term(
    struct nequeue *            queue)
{
    NREQUIRE(NAPI_POINTER, queue != NULL);
    NREQUIRE(NAPI_OBJECT,  queue->signature == NSIGNATURE_EQUEUE);

    while (!nequeue_is_empty(queue)) {
        const struct nevent *   event;

        event = nequeue_get(queue);
        nevent_ref_down(event);
        nevent_destroy(event);
    }
    nqueue_term(&queue->queue);
#if (CONFIG_REGISTRY == 1)
    queue->min = 0;
#endif
    NOBLIGATION(queue->signature = ~NSIGNATURE_EQUEUE);
}


/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of equeue.c
 ******************************************************************************/
