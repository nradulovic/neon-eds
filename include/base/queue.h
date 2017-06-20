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
static NQUEUE_DEFINE(my_queue, 16);
@endcode

Then you can use the 'my_queue' queue. The following example shows two 
application methods using queue to put and get items into 'my_queue' queue.

@code
static NQUEUE_DEFINE(my_queue, 16);

int put_item(void * item) 
{
    if (!nqueue_is_full(&my_queue)) {
        nqueue_put_fifo(&my_queue, item);
        return (0);
    } else {
        return (-1);
    }
}

void * get_item(void)
{
    if (!nqueue_is_empty(&my_queue)) {
        return (nqueue_get(&my_queue));
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
    NQUEUE_DECLARE(my_queue, 16);
    int other_member;
};
@endcode
*/

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
 * @brief       Macro to declare a queue structure
 * @param       name
 *              Name of the declared queue data-type
 * @param       elements 
 *              Number of elements in the queue
 * @details     This macro can be used to declare a queue of fixed size. Then
 *              the structure can be instanced as needed.
 * @api
 */
#define NQUEUE_STRUCT(name, elements)                                           \
    struct name {                                                               \
        struct np_queue         p_queue;                                        \
        void *                  buffer[elements];                               \
    }

/**
 * @brief       Macro to declare a queue object
 * @param       name
 *              Name of the declared queue data-type
 * @param       elements 
 *              Number of elements in the queue
 * @api
 */
#define NQUEUE_DECLARE(name, elements)                                          \
    NQUEUE_STRUCT(name, elements) name
    
/**
 * @brief       Macro to define and initialize a queue
 * @param       name
 *              Name of the declared queue data-type
 * @param       elements 
 *              Number of elements in the queue
 * @note        The macro can be used for global and local queue data type 
 *              variables.
 * @api
 */
#define NQUEUE_DEFINE(name, elements)                                           \
    NQUEUE_DECLARE(name, elements) = {                                          \
        {                                                                       \
            0,                                                                  \
            0,                                                                  \
            elements,                                                           \
            elements,                                                           \
            name._buffer                                                        \
        },                                                                      \
        {0}                                                                     \
    }

/**
 * @brief       Initialize a queue declared by @ref NQUEUE_DECLARE
 * @param       queue
 *              Pointer to the declared queue data-type
 * @mfuncw
 * @mseffect
 * @api
 */
#define NQUEUE_INITIALIZE(queue)                                                \
    np_queue_init(&(queue)->p_queue, (queue)->buffer, sizeof((queue)->buffer))

/**
 * @brief       Initialize a queue which was dynamically allocated
 * @param       queue
 *              Pointer to the declared queue data-type
 * @mfuncw
 * @api
 */
#define nqueue_init(queue, storage, size)                                       \
    np_queue_init(&(queue)->p_queue, storage, size)
    
/**
 * @brief       Put an item to queue in FIFO mode
 * @note        Before calling this method ensure that queue can accept new 
 *              item. See @ref nqueue_is_full.
 * @mfuncw
 * @api         
 */
#define nqueue_put_fifo(queue, item)                                            \
    np_queue_put_fifo(&(queue)->p_queue, (item))
    
/**
 * @brief       Put an item to queue in LIFO mode
 * @note        Before calling this function ensure that queue can accept new 
 *              item. See @ref nqueue_is_full.
 * @mfuncw
 * @api
 */
#define nqueue_put_lifo(queue, item)                                            \
    np_queue_put_lifo(&(queue)->p_queue, (item))
    
/**
 * @brief       Get an item from queue
 * @note        Before calling this function ensure that queue has an item. See 
 *              @ref nqueue_is_empty.
 * @mfuncw
 * @api
 */ 
#define nqueue_get(queue)                                                       \
    np_queue_get(&(queue)->p_queue)
    
/**
 * @brief       Peek to queue head
 * @details     Get the pointer to head item in the queue. The item is not 
 *              removed from queue.
 * @mfuncw
 * @api
 */
#define nqueue_head(queue, item)                                                \
    np_queue_head(&(queue)->p_queue, (item))
    
/**
 * @brief       Peek to queue tail
 * @details     Get the pointer to tail item in the queue. The item is not 
 *              removed from queue.
 * @mfuncw
 * @api
 */
#define nqueue_tail(queue, item)                                                \
    np_queue_tail(&(queue)->p_queue, (item))
    
/**
 * @brief       Returns the max number of items in queue
 * @api
 */
#define nqueue_size(queue)              (queue)->p_queue.size

/**
 * @brief       Returns the current number of free slots in queue
 * @api
 */
#define nqueue_free(queue)              (queue)->p_queue.free

/**
 * @brief       Return pointer to queue storage
 * @api         
 */
#define nqueue_storage(queue)           (queue)->p_queue.storage

/**
 * @brief       Return true if queue is full else false
 * @api
 */
#define nqueue_is_full(queue)           (!(queue)->p_queue.free)

/**
 * @brief       Return true if queue is empty else false
 * @api
 */
#define nqueue_is_empty(queue)                                                  \
    (((queue)->p_queue.free != (queue)->p_queue.size))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Private queue structure
 * @notapi
 */
struct np_queue
{
    uint32_t                    head;
    uint32_t                    tail;
    uint32_t                    free;
    uint32_t                    size;
    void **                     storage;
};

/**@brief       Queue structure for dynamic allocation
 * @details     Use this structure only on dynamically allocated objects. For
 *              all other purposes using @ref NQUEUE_DECLARE or 
 *              @ref NQUEUE_DEFINE macros is encouraged. 
 * @api
 */
struct nqueue
{
    struct np_queue             p_queue;
};

typedef struct nqueue nqueue;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**
 * @brief       Initialize queue structure
 * @details     Before using the queue the structure needs to be initialized.
 *              This function should be called only on dynamically allocated
 *              queue.
 * @note        The function does not check the size of storage buffer. It's up
 *              to user to ensure that buffer is big enough to hold 'void *'
 *              pointers for all items in the queue.
 * @api         
 */
PORT_C_INLINE 
void np_queue_init(struct np_queue * queue, void * storage, uint32_t size)
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
void np_queue_term(struct np_queue * queue)
{
    /* Set everything to NULL or 0 since it is easier to catch NULL pointer 
       bugs */
    queue->storage = NULL; 
    queue->free = 0u;
    queue->size = 0u;
}



/**
 * @brief       Put an item to queue in FIFO mode
 * @note        This is a private function, use @ref nqueue_put_fifo.
 * @notapi
 */
PORT_C_INLINE 
void np_queue_put_fifo(struct np_queue * queue, void * item)
{
    queue->storage[queue->head++] = item;

    if (queue->head == queue->size) {
        queue->head = 0u;
    }
    queue->free--;
}



/**
 * @brief       Put an item to queue in LIFO mode
 * @note        This is a private function, use @ref nqueue_put_lifo.
 * @notapi
 */
PORT_C_INLINE 
void np_queue_put_lifo(struct np_queue * queue, void * item)
{
    if (queue->tail == 0u) {
        queue->tail = queue->size;
    }
    queue->storage[--queue->tail] = item;
    queue->free--;
}



/**
 * @brief       Get an item from queue
 * @note        Before calling this function ensure that queue has an item. See 
 *              @ref nqueue_is_empty.
 * @api
 */ 
PORT_C_INLINE 
void * np_queue_get(struct np_queue * queue)
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
void * np_queue_head(const struct np_queue * queue)
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
void * np_queue_tail(const struct np_queue * queue)
{
    return (queue->storage[queue->tail]);
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

