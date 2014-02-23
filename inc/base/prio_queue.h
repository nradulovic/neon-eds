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
 * @author  	Nenad Radulovic
 * @brief       Priority queue header
 * @defgroup    base_prio_queue Priority queue management
 * @brief       Priority queue management
 *********************************************************************//** @{ */
/**@defgroup    base_prio_queue_intf Interface
 * @brief       Priority queue API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_PRIO_QUEUE_H_
#define ES_PRIO_QUEUE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stdint.h>

#include "plat/compiler.h"
#include "base/prio_queue_config.h"
#include "base/debug.h"
#include "base/bitop.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Priority queue element structure
 * @api
 */
struct esPqElem {
    struct esPq *       queue;                                                  /**@brief Container queue.                                  */
    struct esPqElem *   prev;                                                   /**@brief Previous element in linked list.                  */
    struct esPqElem *   next;                                                   /**@brief Next element in linked list.                      */
    uint_fast8_t        priority;                                               /**@brief Priority level.                                   */
};

/**@brief       Priority queue element type
 * @api
 */
typedef struct esPqElem esPqElem;

/**@brief       Priority Queue structure
 * @api
 */
struct esPq {

/**@brief       Priority Bit Map structure
 * @notapi
 */
    struct esPqBitmap {
#if   (CONFIG_PQ_PRIORITY_LEVELS > ES_CPU_DEF_DATA_WIDTH) || defined(__DOXYGEN__)
        esAtomic        bitGroup;                                               /**<@brief Bit list indicator                               */
#endif
        esAtomic        bit[ES_DIVISION_ROUNDUP(CONFIG_PQ_PRIORITY_LEVELS, ES_CPU_DEF_DATA_WIDTH)]; /**< @brief Bit priority indicator     */
    }                   bitmap;                                                 /**<@brief Priority bitmap                                  */

/**@brief       Priority linked list sentinel structure
 * @notapi
 */
    struct esPqList {
        struct esPqElem * head;                                                 /**<@brief Points to the first element in linked list.      */
        struct esPqElem * next;                                                 /**<@brief Points to the next element in linked list.       */
    }                   list[CONFIG_PQ_PRIORITY_LEVELS];                        /**<@brief Array of linked list sentinel structures.        */
#if   (1u == CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    esAtomic            signature;                                              /**<@brief Priority queue structure signature.              */
#endif
};

/**@brief       Priority queue type
 * @api
 */
typedef struct esPq esPq;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void esPqInit(
    struct esPq *       queue);

void esPqTerm(
    struct esPq *       queue);

void esPqAdd(
    struct esPq *       queue,
    struct esPqElem *   element);

void esPqRm(
    struct esPqElem *   element);

static PORT_C_INLINE struct esPq * esPqGetContainer_(
    const struct esPqElem * element) {

    return (element->queue);
}

static PORT_C_INLINE uint_fast8_t esPqGetPriority_(
    const struct esPqElem * element) {

    return (element->priority);
}

struct esPqElem * esPqGetHighest(
    const struct esPq * queue);

struct esPqElem * esPqGetNext(
    const struct esPq * queue,
    uint_fast8_t        priority);

struct esPqElem * esPqRotate(
    struct esPq *       queue,
    uint_fast8_t        priority);

bool esPqIsEmpty(
    const struct esPq * queue);

void esPqElementInit(
    struct esPqElem *   element,
    uint_fast8_t        priority);

void esPqElementTerm(
    struct esPqElem *   element);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of prio_queue.h
 ******************************************************************************/
#endif /* ES_PRIO_QUEUE_H_ */
