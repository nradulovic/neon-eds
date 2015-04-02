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

#include "ep/equeue.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void nequeue_init(
    struct nequeue *            queue,
    const struct nequeue_define * define)
{
    ncpu_reg                    size;

    size = (ncpu_reg)(define->size / sizeof(struct nevent * [1]));

#if (CONFIG_REGISTRY == 1)
    nqueue_init(&queue->queue, define->storage, size);
    queue->min = size;
#else
    nqueue_init(&queue->queue, define->storage, size);
#endif
}



void nequeue_term(
    struct nequeue *            queue)
{
#if (CONFIG_REGISTRY == 1)
    nqueue_term(&queue->queue);
    queue->min = 0;
#else
    nqueue_term(&queue->queue);
#endif
}


/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of equeue.c
 ******************************************************************************/
