/*
 * This file is part of eSolid.
 *
 * Copyright (C) 2010 - 2013 Nenad Radulovic
 *
 * eSolid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with eSolid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Generic queue header
 * @defgroup    base_queue Generic queue
 * @brief       Generic queue
 *********************************************************************//** @{ */

#ifndef ES_QUEUE_H_
#define ES_QUEUE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "plat/compiler.h"

/*===============================================================  MACRO's  ==*/

#define ES_QP_SIZEOF(elements)                                                  \
    (sizeof(void * [1]) * (elements))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Pointer queue
 * @api
 */
struct esQp {
    void **             buff;
    uint32_t            head;
    uint32_t            tail;
    uint32_t            free;
    uint32_t            size;
};

typedef struct esQp esQp;

typedef void * esQpItem;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

static PORT_C_INLINE void esQpInit(
    struct esQp *       qp,
    void **             buff,
    size_t              size) {

    qp->buff = buff;
    qp->head = UINT32_C(0);
    qp->tail = UINT32_C(0);
    qp->free = (uint32_t)size;
    qp->size = (uint32_t)size;
}

static PORT_C_INLINE void esQpTerm(
    struct esQp *       qp) {

    qp->buff = NULL;
    qp->head = UINT32_C(0);
    qp->tail = UINT32_C(0);
    qp->free = UINT32_C(0);
    qp->size = UINT32_C(0);
}

static PORT_C_INLINE void esQpPutItem(
    struct esQp *       qp,
    void *              item) {

    qp->buff[qp->head++] = item;

    if (qp->head == qp->size) {
        qp->head = UINT32_C(0);
    }
    --qp->free;
}

static PORT_C_INLINE void esQpPutTailItem(
    struct esQp *       qp,
    void *              item) {

    if (qp->tail == UINT32_C(0)) {
        qp->tail = qp->size;
    }
    qp->buff[--qp->tail] = item;
    --qp->free;
}

static PORT_C_INLINE void * esQpGetItem(
    struct esQp *       qp) {

    void *              tmp;

    tmp = qp->buff[qp->tail++];

    if (qp->tail == qp->size) {
        qp->tail = UINT32_C(0);
    }
    ++qp->free;

    return (tmp);
}

static PORT_C_INLINE size_t esQpSize(
    const struct esQp * qp) {

    return ((size_t)(qp->size));
}

static PORT_C_INLINE size_t esQpOccupied(
    const struct esQp * qp) {

    return ((size_t)(qp->size - qp->free));
}

static PORT_C_INLINE size_t esQpFreeSpace(
    const struct esQp * qp) {

    return ((size_t)(qp->free));
}

static PORT_C_INLINE bool esQpIsFull(
    const struct esQp * qp) {

    if (qp->free == UINT32_C(0)) {

        return (true);
    } else {

        return (false);
    }
}

static PORT_C_INLINE bool esQpIsEmpty(
    const struct esQp * qp) {

    if (qp->free == qp->size) {

        return (true);
    } else {

        return (false);
    }
}

static PORT_C_INLINE void ** esQpBuff(
    const struct esQp * qp) {

    return (qp->buff);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of queue.h
 ******************************************************************************/
#endif /* ES_QUEUE_H_ */
