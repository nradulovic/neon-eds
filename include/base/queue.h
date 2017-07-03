/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2017 Nenad Radulovic
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
 * @brief       Generic queue header
 * @addtogroup  base_intf
 *********************************************************************//** @{ */
/**
 * @defgroup    base_queue Generic Queue
 * @brief       Generic Queue
 * @{ *//*--------------------------------------------------------------------*/
 
/** 
@addtogroup     base_queue
@section        queue_alloc Queue allocation
@par            Static queue allocation

The following code will statically allocate a queue of 16 items in size. 

@code
static NQUEUE_BUNDLE_DEFINE(event_queue, 16);
@endcode

Then you can use the 'my_queue' queue. The following example shows two 
application methods using queue to put and get items into 'my_queue' queue.

@code
static NQUEUE_BUNDLE_DEFINE(event_queue, 16);

int put_item(void * item) 
{
    struct nqueue * queue = &event_queue.b;
    
    if (!nqueue_is_full(queue)) {
        nqueue_put_fifo(queue, item);
        return (0);
    } else {
        return (-1);
    }
}

void * get_item(void)
{
    struct nqueue * queue = &event_queue.b;

    if (!nqueue_is_empty(queue)) {
        return (nqueue_get(queue));
    } else {
        return (NULL);
    }
}
@endcode
 
@par            Embedding in other structure

The queue structure can be embedded in other structures. 

@code
struct application_struct
{
    NQUEUE_BUNDLE_DECLARE(event_queue, 16);
    int other_member;
};
@endcode
*/

#ifndef NEON_BASE_QUEUE_H_
#define NEON_BASE_QUEUE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "base/bitop.h"
#include "port/compiler.h"

/*===============================================================  MACRO's  ==*/

/**
 * @brief       Helper macros for queue memory pointer
 * @notapi
 */
#if (CONFIG_DYNAMIC_QUEUE == 1) || defined(__DOXYGEN__)
#define NP_QUEUE_MEM                    .mem = NULL,
#define np_queue_mem_init(queue)		(queue)->mem = NULL;
#else
#define NP_QUEUE_MEM
#define np_queue_mem_init(queue)        (void)0
#endif

/**
 * @brief       Macro to declare a queue bundle structure
 * @param       name
 *              Name of the declared queue data-type
 * @param       elements 
 *              Number of elements in the queue. The number of elements must be
 *              equal to 2 to the power of N, where N is in range 1 to 32
 *              (inclusive).
 * @details     This macro can be used to declare a queue of fixed size. Then
 *              the structure can be instanced as needed.
 * @mseffect
 * @api
 */
#define NQUEUE_BUNDLE_STRUCT(name, elements)                                    \
    struct name {                                                               \
        struct nqueue           b;                                          	\
        void *                  buf 											\
				[((elements < 2) || !N_IS_POWEROF_2(elements)) ? -1 : elements];\
    }

/**
 * @brief       Macro to declare a queue bundle object
 * @param       name
 *              Name of the declared queue data-type
 * @param       elements 
 *              Number of elements in the queue
 * @mseffect
 * @api
 */
#define NQUEUE_BUNDLE_DECLARE(name, elements)                                   \
    NQUEUE_BUNDLE_STRUCT(name, elements) name
    
/**
 * @brief       Initialize a queue bundle declared by @ref NQUEUE_BUNDLE_DECLARE
 * @param       name
 *              Name of the declared queue data-type
 * @mseffect
 * @api
 */
#define NQUEUE_BUNDLE_INITIALIZER(name)                                         \
    {                                                                           \
        .b = {                                                                  \
            NP_QUEUE_MEM                                                        \
            .head = 0,                                                          \
            .tail = 0,                                                          \
            .empty = sizeof(name.buf) / sizeof(name.buf[0]),                    \
            .mask = sizeof(name.buf) / sizeof(name.buf[0]) - 1u,                \
            .buf = name.buf,                                                    \
        },                                                                      \
    }

/**
 * @brief       Macro to define and initialize a queue bundle
 * @param       name
 *              Name of the declared queue data-type
 * @param       elements
 *              Number of elements in the queue
 * @note        The macro can be used for global and local queue data type
 *              variables.
 * @mseffect
 * @api
 */
#define NQUEUE_BUNDLE_DEFINE(name, elements)                                    \
    NQUEUE_BUNDLE_DECLARE(name, elements) = NQUEUE_BUNDLE_INITIALIZER(name)

/**
 * @brief		Macro to initialize bundle structure of dynamically allocated
 * 				queues
 * @mseffect
 * @api
 */
#define NQUEUE_BUNDLE_INIT(queue)												\
	do {																		\
	    nqueue_init(&(queue)->b, sizeof((queue)->buf) / sizeof((queue)->buf[0]),\
	        &(queue)->buf[0]);                                                  \
	} while (0)

/**
 * @brief       Initialize a queue structure
 * @api
 */
#define nqueue_init(queue, size, buf)                                           \
    do {                                                                        \
        struct nqueue * queue_ = queue;                                         \
        size_t size_ = size;                                                    \
        np_queue_mem_init(queue_);												\
		queue_->head = 0u;													    \
		queue_->tail = 0u;													    \
		queue_->empty = size_;		                                            \
		queue_->mask = size_ - 1u;	                                            \
		queue_->buf = buf;                                                      \
    } while (0)

/**
 * @brief       Returns the max number of items in queue
 * @api
 */
#define nqueue_size(queue)           	((queue)->mask + 1u)

/**
 * @brief       Returns the current number of empty slots in queue
 * @api
 */
#define nqueue_empty(queue)           	(queue)->empty

/**
 * @brief       Return true if queue is full else false
 * @api
 */
#define nqueue_is_full(queue)        	(!(queue)->empty)

/**
 * @brief       Return true if queue is empty else false
 * @api
 */
#define nqueue_is_empty(queue)          ((queue)->empty == nqueue_size(queue))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/* NOTE:
 * Forward declarations of nmem structure
 */
struct nmem;

/**
 * @brief       Queue structure
 * @details     Use this structure only on dynamically allocated objects. For
 *              all other purposes using @ref NQUEUE_BUNDLE_DECLARE or
 *              @ref NQUEUE_BUNDLE_DEFINE macros is encouraged.
 * @api
 */
struct nqueue
{
#if (CONFIG_DYNAMIC_QUEUE == 1) || defined(__DOXYGEN__)
    struct nmem *               mem;
#endif
    uint32_t                    head;
    uint32_t                    tail;
    uint32_t                    empty;
    uint32_t                    mask;
    void **                     buf;
};

/**
 * @brief       Queue type
 * @api
 */
typedef struct nqueue nqueue;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Allocate queue structure
 * @param       elements
 *              Number of elements in the queue. The number of elements must be
 *              equal to 2 to the power of N, where N is in range 1 to 32
 *              (inclusive).
 * @note        To use this API call the configuration option 
 *              @ref CONFIG_DYNAMIC_QUEUE must be enabled.
 * @api
 */
#if (CONFIG_DYNAMIC_QUEUE == 1) || defined(__DOXYGEN__)
struct nqueue * nqueue_alloc(struct nmem * mem, uint32_t elements);
#endif

/**
 * @brief       Free queue structure
 * @param       queue
 *              Pointer to queue that was previously allocated using 
 *              @ref nqueue_alloc function.
 * @note        To use this API call the configuration option 
 *              @ref CONFIG_DYNAMIC_QUEUE must be enabled.
 * @api
 */
#if (CONFIG_DYNAMIC_QUEUE == 1) || defined(__DOXYGEN__)
void nqueue_free(struct nqueue * queue);
#endif

/**
 * @brief       Put an item to queue in FIFO mode
 * @note        Before calling this function ensure that queue is not full, see
 * 				@ref nqueue_is_full.
 * @api
 */
PORT_C_INLINE 
void nqueue_put_fifo(struct nqueue * self, void * item)
{
	self->buf[self->head++] = item;
    self->head &= self->mask;
    self->empty--;
}

/**
 * @brief       Put an item to queue in LIFO mode
 * @note        Before calling this function ensure that queue is not full, see
 * 				@ref nqueue_is_full.
 * @api
 */
PORT_C_INLINE 
void nqueue_put_lifo(struct nqueue * self, void * item)
{
	self->tail--;
	self->tail &= self->mask;
    self->buf[self->tail] = item;
    self->empty--;
}

/**
 * @brief       Get an item from queue
 * @note        Before calling this function ensure that queue has an item. See 
 *              @ref nqueue_is_empty.
 * @api
 */ 
PORT_C_INLINE 
void * nqueue_get(struct nqueue * self)
{
    void *                      tmp;

    tmp = self->buf[self->tail++];
    self->tail &= self->mask;
    self->empty++;

    return (tmp);
}

/**
 * @brief       Peek to queue head
 * @details     Get the pointer to head item in the queue. The item is not 
 *              removed from queue.
 * @api
 */
PORT_C_INLINE 
void * nqueue_head(const struct nqueue * self)
{
    uint32_t                    real_head;
    
	real_head = self->head;
    real_head--;
    real_head &= self->mask;
    
    return (self->buf[real_head]);
}

/**
 * @brief       Peek to queue tail
 * @details     Get the pointer to tail item in the queue. The item is not 
 *              removed from queue.
 * @api
 */
PORT_C_INLINE 
void * nqueue_tail(const struct nqueue * self)
{
    return (self->buf[self->tail]);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of queue.h
 ******************************************************************************/
#endif /* NEON_BASE_QUEUE_H_ */

