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
 * @brief       Priority queue implementation
 * @addtogroup  base_prio_queue
 *********************************************************************//** @{ */
/**@defgroup    base_prio_queue_impl Implementation
 * @brief       Priority queue Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "base/base.h"
#include "base/prio_queue.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Priority queue signature
 */
#define PQ_SIGNATURE                    ((esAtomic)0xdeedbeeful)

/**@brief       Is priority list empty?
 */
#define PQLIST_IS_EMPTY(sentinel)                                               \
    ((sentinel)->head == NULL)

/**@brief       Is priority list entry at head of list?
 */
#define PQLIST_IS_ENTRY_AT_HEAD(sentinel, entry)                                \
    ((sentinel)->head == (entry))

/**@brief       Is priority list entry at next entry in the list?
 */
#define PQLIST_IS_ENTRY_AT_NEXT(sentinel, entry)                                \
    ((sentinel)->next == (entry))

/**@brief       Is the entry single in the list?
 */
#define PQLIST_IS_ENTRY_SINGLE(entry)                                           \
    ((entry) == (entry)->next)

#define PQLIST_ROTATE_HEAD(sentinel)                                            \
    do {                                                                        \
        (sentinel)->head = (sentinel)->head->next;                              \
    } while (0u);

#define PQLIST_ROTATE_NEXT(sentinel)                                            \
    do {                                                                        \
        (sentinel)->next = (sentinel)->next->next;                              \
    } while (0u)

/**@brief       DList macro: get the next entry
 */
#define PQLIST_ENTRY_NEXT(entry)                                                \
    ((entry)->next)

#define PQLIST_SENTINEL_INIT(sentinel, entry)                                   \
    do {                                                                        \
        (sentinel)->head = (entry);                                             \
        (sentinel)->next = (entry);                                             \
    } while (0u)

#define PQLIST_SENTINEL_TERM(sentinel)                                          \
    do {                                                                        \
        (sentinel)->head = NULL;                                                \
    } while (0u)

/**@brief       DList macro: initialize entry
 */
#define PQLIST_ENTRY_INIT(entry)                                                \
    do {                                                                        \
        (entry)->next = (entry);                                                \
        (entry)->prev = (entry);                                                \
    } while (0u)

/**@brief       DList macro: add new @c entry after @c current entry
 */
#define PQLIST_ENTRY_ADD_AFTER(current, entry)                                  \
    do {                                                                        \
        (entry)->next = (current);                                              \
        (entry)->prev = (entry)->next->prev;                                    \
        (entry)->next->prev = (entry);                                          \
        (entry)->prev->next = (entry);                                          \
    } while (0u)

/**@brief       DList macro: remove the @c entry from a list
 */
#define PQLIST_ENTRY_RM(entry)                                                  \
    do {                                                                        \
        (entry)->next->prev = (entry)->prev;                                    \
        (entry)->prev->next = (entry)->next;                                    \
    } while (0u)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**@brief       Initialize bitmap
 * @param       pqbm
 *              Pointer to the bit map structure
 */
static PORT_C_INLINE void bitmapInit(
    struct esPqBitmap * bitmap);

/**@brief       Set the bit corresponding to the priority argument
 * @param       pqbm
 *              Pointer to the bit map structure
 * @param       priority
 *              Priority which will be marked as used
 */
static PORT_C_INLINE void bitmapSet(
    struct esPqBitmap * bitmap,
    uint_fast8_t        priority);

/**@brief       Clear the bit corresponding to the priority argument
 * @param       pqbm
 *              Pointer to the bit map structure
 * @param       priority
 *              Priority which will be marked as unused
 */
static PORT_C_INLINE void bitmapClear(
    struct esPqBitmap * bitmap,
    uint_fast8_t        priority);

/**@brief       Get the highest priority set
 * @param       pqbm
 *              Pointer to the bit map structure
 * @return      The number of the highest priority marked as used
 */
static PORT_C_INLINE uint_fast8_t bitmapGetHighest(
    const struct esPqBitmap * bitmap);

/**@brief       Is bit map empty?
 * @param       pqbm
 *              Pointer to the bit map structure
 * @return      The status of the bit map
 *  @retval     true - bit map is empty
 *  @retval     false - there is at least one bit set
 */
static PORT_C_INLINE bool bitmapIsEmpty(
    const struct esPqBitmap * bitmap);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Prio queue", "Priority ordered queue", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static PORT_C_INLINE void bitmapInit(
    struct esPqBitmap * bitmap) {

#if   (CONFIG_PQ_PRIORITY_LEVELS > ES_CPU_DEF_DATA_WIDTH)
    uint_fast8_t        grp;

    bitmap->bitGroup = 0u;
    grp = ES_DIVISION_ROUNDUP(CONFIG_PQ_PRIORITY_LEVELS, ES_CPU_DEF_DATA_WIDTH);

    while (grp != 0u) {
        --grp;
        bitmap->bit[grp] = 0u;
    }
#else
    bitmap->bit[0] = 0u;
#endif
}

static PORT_C_INLINE void bitmapSet(
    struct esPqBitmap * bitmap,
    uint_fast8_t        priority) {

#if   (CONFIG_PQ_PRIORITY_LEVELS > ES_CPU_DEF_DATA_WIDTH)
    uint_fast8_t        grpIndx;
    uint_fast8_t        bitIndx;

    bitIndx = priority &
        (~((uint_fast8_t)0u) >> (sizeof(priority) * 8u - ES_UINT8_LOG2(ES_CPU_DEF_DATA_WIDTH)));
    grpIndx               = priority >> ES_UINT8_LOG2(ES_CPU_DEF_DATA_WIDTH);
    bitmap->bitGroup     |= ES_CPU_PWR2(grpIndx);
    bitmap->bit[grpIndx] |= ES_CPU_PWR2(bitIndx);
#else
    bitmap->bit[0] |= ES_CPU_PWR2(priority);
#endif
}

static PORT_C_INLINE void bitmapClear(
    struct esPqBitmap * bitmap,
    uint_fast8_t        priority) {

#if   (CONFIG_PQ_PRIORITY_LEVELS > ES_CPU_DEF_DATA_WIDTH)
    uint_fast8_t        grpIndx;
    uint_fast8_t        bitIndx;

    bitIndx = priority &
        (~((uint_fast8_t)0u) >> (sizeof(priority) * 8u - ES_UINT8_LOG2(ES_CPU_DEF_DATA_WIDTH)));
    grpIndx               = priority >> ES_UINT8_LOG2(ES_CPU_DEF_DATA_WIDTH);
    bitmap->bit[grpIndx] &= ~ES_CPU_PWR2(bitIndx);

    if (bitmap->bit[grpIndx] == 0u) {                                             /* Is this the last one bit cleared in this list?          */
        bitmap->bitGroup &= ~ES_CPU_PWR2(grpIndx);                             /* Yes: then clear bit list indicator, too.                */
    }
#else
    bitmap->bit[0] &= ~ES_CPU_PWR2(priority);
#endif
}

static PORT_C_INLINE uint_fast8_t bitmapGetHighest(
    const struct esPqBitmap * bitmap) {

#if   (CONFIG_PQ_PRIORITY_LEVELS > ES_CPU_DEF_DATA_WIDTH)
    uint_fast8_t        grpIndx;
    uint_fast8_t        bitIndx;

    grpIndx = ES_CPU_FLS(bitmap->bitGroup);
    bitIndx = ES_CPU_FLS(bitmap->bit[grpIndx]);

    return ((grpIndx << ES_UINT8_LOG2(ES_CPU_DEF_DATA_WIDTH)) | bitIndx);
#else
    uint_fast8_t        bitIndx;

    bitIndx = ES_CPU_FLS(bitmap->bit[0]);

    return (bitIndx);
#endif
}

static PORT_C_INLINE bool bitmapIsEmpty(
    const struct esPqBitmap * bitmap) {

#if   (CONFIG_PQ_PRIORITY_LEVELS > ES_CPU_DEF_DATA_WIDTH)
    bool              ret;

    if (bitmap->bitGroup == 0u) {
        ret = true;
    } else {
        ret = false;
    }

    return (ret);
#else
    bool              ret;

    if (bitmap->bit[0] == 0u) {
        ret = true;
    } else {
        ret = false;
    }

    return (ret);
#endif
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void esPqInit(
    struct esPq *       queue) {

    uint_fast8_t        cnt;

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature != PQ_SIGNATURE);

    bitmapInit(&queue->bitmap);
    cnt = CONFIG_PQ_PRIORITY_LEVELS;

    while (cnt != 0u) {
        --cnt;
        PQLIST_SENTINEL_INIT(&queue->list[cnt], NULL);
    }
    ES_OBLIGATION(queue->signature = PQ_SIGNATURE);
}

/* 1)       When API validation is not used then this function will become empty.
 */
void esPqTerm(
    struct esPq *       queue) {

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature == PQ_SIGNATURE);

#if (CONFIG_API_VALIDATION == 0)
    (void)queue;
#endif
    ES_OBLIGATION(queue->signature = ~PQ_SIGNATURE);
}



void esPqAdd(
    struct esPq *       queue,
    struct esPqElem *   element) {

    struct esPqList *   sentinel;

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature == PQ_SIGNATURE);
    ES_REQUIRE(ES_API_POINTER, element != NULL);
    ES_REQUIRE(ES_API_OBJECT,  element->queue == NULL);

    sentinel = &queue->list[element->priority];                                 /* Get the sentinel for element priority level.             */

    if (PQLIST_IS_EMPTY(sentinel)) {                                            /* Is PQ list empty?                                        */
        PQLIST_SENTINEL_INIT(sentinel, element);                                /* This element becomes first in the list.                  */
        bitmapSet(&queue->bitmap, element->priority);                           /* Mark the priority list as used.                         */
    } else {
        PQLIST_ENTRY_ADD_AFTER(sentinel->head, element);                        /* Element is added at the next of the list.                */
    }
    element->queue = queue;                                                     /* Save the queue into element                              */
}

void esPqRm(
    struct esPqElem *   element) {

    struct esPqList *   sentinel;

    ES_REQUIRE(ES_API_POINTER, element != NULL);
    ES_REQUIRE(ES_API_OBJECT,  element->queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  element->queue->signature == PQ_SIGNATURE);

    sentinel = &element->queue->list[element->priority];                        /* Get the sentinel for element priority level.             */

    if (PQLIST_IS_ENTRY_SINGLE(element)) {
        PQLIST_SENTINEL_TERM(sentinel);                                         /* Make the list sentinel empty.                            */
        bitmapClear(&element->queue->bitmap, element->priority);                /* Remove the mark since this list is not used.            */
    } else {
        if (PQLIST_IS_ENTRY_AT_HEAD(sentinel, element)) {                       /* In case we are removing first element in linked list then*/
            PQLIST_ROTATE_HEAD(sentinel);                                       /* advance the head to point to the next one in the list.   */
        }

        if (PQLIST_IS_ENTRY_AT_NEXT(sentinel, element)) {                       /* In case we are removing next element in the linked list  */
            PQLIST_ROTATE_NEXT(sentinel);                                       /* then move next to point to a next one in the list.       */
        }
        PQLIST_ENTRY_RM(element);
        PQLIST_ENTRY_INIT(element);
    }
    element->queue = NULL;
}

struct esPqElem * esPqGetHighest(
    const struct esPq * queue) {

    const struct esPqList * sentinel;
    uint_fast8_t        prio;

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature == PQ_SIGNATURE);
    ES_REQUIRE(ES_API_USAGE,   bitmapIsEmpty(&queue->bitmap) == false);

    prio = bitmapGetHighest(&queue->bitmap);
    sentinel = &queue->list[prio];

    return (PQLIST_ENTRY_NEXT(sentinel));
}

struct esPqElem * esPqGetNext(
    const struct esPq * queue,
    uint_fast8_t        priority) {

    const struct esPqList * sentinel;

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature == PQ_SIGNATURE);
    ES_REQUIRE(ES_API_USAGE,   bitmapIsEmpty(&queue->bitmap) == false);
    ES_REQUIRE(ES_API_RANGE,   priority < CONFIG_PQ_PRIORITY_LEVELS);

    sentinel = &queue->list[priority];

    return (PQLIST_ENTRY_NEXT(sentinel));
}

struct esPqElem * esPqRotate(
    struct esPq *       queue,
    uint_fast8_t        prio) {

    struct esPqList *     sentinel;

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature == PQ_SIGNATURE);
    ES_REQUIRE(ES_API_USAGE,   bitmapIsEmpty(&queue->bitmap) == false);
    ES_REQUIRE(ES_API_RANGE,   prio < CONFIG_PQ_PRIORITY_LEVELS);

    sentinel = &queue->list[prio];
    PQLIST_ROTATE_NEXT(sentinel);

    return (PQLIST_ENTRY_NEXT(sentinel));
}

bool esPqIsEmpty(
    const struct esPq * queue) {

    bool              ret;

    ES_REQUIRE(ES_API_POINTER, queue != NULL);
    ES_REQUIRE(ES_API_OBJECT,  queue->signature == PQ_SIGNATURE);

    ret = bitmapIsEmpty(&queue->bitmap);

    return (ret);
}

void esPqElementInit(
    struct esPqElem *   element,
    uint_fast8_t        priority) {

    ES_REQUIRE(ES_API_POINTER, element != NULL);
    ES_REQUIRE(ES_API_RANGE,   priority < CONFIG_PQ_PRIORITY_LEVELS);

    element->queue = NULL;
    PQLIST_ENTRY_INIT(element);
    element->priority = priority;
}

void esPqElementTerm(
    struct esPqElem *   element) {

    ES_REQUIRE(ES_API_POINTER, element != NULL);
    ES_REQUIRE(ES_API_POINTER, element->queue == NULL);

#if (CONFIG_API_VALIDATION == 0)
    (void)element;
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of error.c
 ******************************************************************************/
