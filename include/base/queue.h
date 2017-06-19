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
/**@defgroup    base_queue Generic Queue
 * @brief       Generic Queue
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_QUEUE_H_
#define NEON_BASE_QUEUE_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "port/compiler.h"

/*===============================================================  MACRO's  ==*/

/**
 * @brief       Calculates the number of bytes needed to store N element 
 *              pointers
 * @api
 */
#define NQUEUE_SIZEOF(elements)             (sizeof(void * [1]) * (elements))

/**
 * @brief       Macro to declare a queue object
 * @param       name
 *              Name of the declared queue datatype
 * @param       elements 
 *              Number of elements in the queue
 * @api
 */
#define NQUEUE_DECLARE(name, elements)                                          \
    struct nqueue_ ## name {                                                    \
        struct nqueue           queue;                                          \
        uint8_t                 storage_buffer[NQUEUE_SIZEOF(elements)];        \
    }

/**
 * @brief       Macro to define and initialize a queue
 * @param       name
 *              Name of the declared queue datatype
 * @param       elements 
 *              Number of elements in the queue
 * @note        The macro can be used for global and local queue data type 
 *              variables.
 * @api
 */
#define NQUEUE_DEFINE(name, elements)                                           \
    NQUEUE_DECLARE(name, elements) name = {                                     \
        {0},                                                                    \
        {                                                                       \
            name.storage_buffer,                                                \
            0,                                                                  \
            0,                                                                  \
            elements,                                                           \
            elements                                                            \
        }                                                                       \
    }

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Queue structure
 * @api
 */
struct nqueue
{
    void **                     storage;
    uint32_t                    head;
    uint32_t                    tail;
    uint32_t                    free;
    uint32_t                    size;
};

typedef struct nqueue nqueue;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**
 * @brief       Initialize queue structure
 * @details     Before using the queue the structure needs to be initialized
 * @api         
 */
PORT_C_INLINE 
void nqueue_init(struct nqueue * queue, void * storage, uint32_t size)
{
    queue->storage = storage;
    queue->head    = 0u;
    queue->tail    = 0u;
    queue->free    = size;
    queue->size    = size;
}



/**
 * @brief       Terminate queue structure
 * @api
 */
PORT_C_INLINE 
void nqueue_term(struct nqueue * queue)
{
    /* Set everything to NULL or 0 since it is easier to catch NULL pointer 
       bugs */
    queue->storage = NULL; 
    queue->head = 0u;
    queue->tail = 0u;
    queue->free = 0u;
    queue->size = 0u;
}



/**
 * @brief       Put an item to queue in FIFO mode
 * @api         
 */
PORT_C_INLINE 
void nqueue_put_fifo(struct nqueue * queue, void * item)
{
    queue->storage[queue->head++] = item;

    if (queue->head == queue->size) {
        queue->head = 0u;
    }
    queue->free--;
}



/**
 * @brief       Put an item to queue in LIFO mode
 * @api
 */
PORT_C_INLINE 
void nqueue_put_lifo(struct nqueue * queue, void * item)
{
    if (queue->tail == 0u) {
        queue->tail = queue->size;
    }
    queue->storage[--queue->tail] = item;
    queue->free--;
}



/**
 * @brief       Get an item from queue
 * @api
 */ 
PORT_C_INLINE 
void * nqueue_get(struct nqueue * queue)
{
    void *                      tmp;

    tmp = queue->storage[queue->tail++];

    if (queue->tail == queue->size) {
        queue->tail = 0u;
    }
    queue->free++;

    return (tmp);
}



/**
 * @brief       Peek to queue head
 * @details     Get the pointer to head item in the queue. The item is not 
 *              removed from queue.
 * @api
 */
PORT_C_INLINE 
void * nqueue_head(const struct nqueue * queue)
{
    uint32_t                    real_head;
    
    if (queue->head == 0) {
        real_head = queue->size;
    } else {
        real_head = queue->head;
    }
    real_head--;
    
    return (queue->storage[real_head]);
}



/**
 * @brief       Peek to queue tail
 * @details     Get the pointer to tail item in the queue. The item is not 
 *              removed from queue.
 * @api
 */
PORT_C_INLINE 
void * nqueue_tail(const struct nqueue * queue)
{
    return (queue->storage[queue->tail]);
}



/**
 * @brief       Returns the max number of items in queue
 * @api
 */
PORT_C_INLINE 
uint32_t nqueue_size(const struct nqueue * queue)
{
    return (queue->size);
}



/**
 * @brief       Returns the current number of free slots in queue
 * @api
 */
PORT_C_INLINE 
uint32_t nqueue_free(const struct nqueue * queue)
{
    return (queue->free);
}



/**
 * @brief       Return pointer to queue storage
 * @api         
 */
PORT_C_INLINE 
void * nqueue_storage(const struct nqueue * queue)
{
    return (queue->storage);
}



/**
 * @brief       Return true if queue is full else false
 * @api
 */
PORT_C_INLINE 
bool nqueue_is_full(const struct nqueue * queue)
{
    return (queue->free == 0u);
}



/**
 * @brief       Return true if queue is empty else false
 * @api
 */
PORT_C_INLINE 
bool nqueue_is_empty(const struct nqueue * queue)
{
    return (queue->free == queue->size);
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

