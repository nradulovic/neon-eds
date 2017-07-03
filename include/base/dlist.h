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
 * @brief       Linked lists header
 * @addtogroup  base_intf
 *********************************************************************//** @{ */
/**@defgroup    base_list Doubly linked list
 * @brief       Doubly linked list
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_DLIST_H_
#define NEON_BASE_DLIST_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stddef.h>

#include "port/compiler.h"
#include "base/debug.h"

/*===============================================================  MACRO's  ==*/

/**
 * @brief       Initialize a declared queue declared
 * @param       name
 *              Name of the declared list data-type
 * @mseffect
 * @api
 */
#define NDLIST_INITIALIZER(name)        {&(name), &(name)}

/**
 * @brief       Macro to define and initialize a list
 * @param       name
 *              Name of the declared list data-type
 * @note        The macro can be used for global and local list data type
 *              variables.
 * @mseffect
 * @api
 */
#define NDLIST_DEFINE(name)                                                     \
    struct ndlist name = NDLIST_INITIALIZER(name)
    
/**
 * @brief       Get the pointer to structure which contains struct ndlist
 * @param       ptr
 *              Pointer to structure/data that is containing struct ndlist.
 * @param       type
 *              Type of variable which contains struct ndlist.
 * @param       member
 *              Name of member in variable structure.
 * @api
 */
#define ndlist_entry(ptr, type, member)                                         \
    PORT_C_CONTAINER_OF(ptr, type, member)
    
/**
 * @brief       Get the first element in list pointed by sentinel.
 * @api
 */
#define ndlist_first(sentinel)          ndlist_next(sentinel)

/**
 * @brief       Get the last element in list pointed by sentinel.
 * @api
 */
#define ndlist_last(sentinel)           ndlist_prev(sentinel)
    
/**
 * @brief       Construct for FOR loop to iterate over each element in list
 * @code
 * struct ndlist * current;
 * struct ndlist * sentinel = &g_list_sentinel.list;
 *
 * for (NDLIST_FOR_EACH(current, sentinel)) {
 *     ... do something with current (excluding remove)
 * }
 * @endcode
 * @api
 */
#define NDLIST_FOR_EACH(current, sentinel)                                      \
    current = ndlist_first(sentinel); current != (sentinel);                     \
    current = ndlist_next(current)
   
/**
 * @brief       Construct for FOR loop to iterate over each element in list
 *              backwards
 * @code
 * struct ndlist * current;
 * struct ndlist * sentinel = &g_list_sentinel.list;
 *
 * for (NDLIST_FOR_EACH_BACKWARDS(current, sentinel)) {
 *     ... do something with current (excluding remove)
 * }
 * @endcode
 * @api
 */ 
#define NDLIST_FOR_EACH_BACKWARDS(current, sentinel)                            \
    current = ndlist_last(sentinel); current != (sentinel);                     \
    current = ndlist_prev(current)
    
/**
 * @brief       Construct for FOR loop to iterate over each element in list 
 *              which is safe against element removal
 * @code
 * struct ndlist * current;
 * struct ndlist * iterator;
 * struct ndlist * sentinel = &g_list_sentinel.list;
 *
 * for (NDLIST_FOR_EACH_SAFE(current, iterator, sentinel)) {
 *     ... do something with current (including remove)
 * }
 * @endcode
 * @api
 */
#define NDLIST_FOR_EACH_SAFE(current, iterator, sentinel)                       \
    current = ndlist_first(sentinel), iterator = ndlist_next(current);           \
    current != (sentinel);                                                      \
    current = iterator, iterator = ndlist_next(iterator)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Doubly-linked list structure
 * @api
 */
struct ndlist
{
    struct ndlist *             next;
    struct ndlist *             prev;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/** 
 * @brief       Initialise a list entry/sentinel
 *
 * Before calling this function:
 *
 * +-----+
 * |     |
 * |  N  |
 * |     |
 * +-----+
 *
 * After call to this function:
 *
 * +-----------+
 * |  +-----+  |
 * +->|     |--+
 *    |  N  |
 * +--|     |<-+
 * |  +-----+  |
 * +-----------+
 */
PORT_C_INLINE
struct ndlist * ndlist_init(struct ndlist * node)
{
    node->next = node;
    node->prev = node;

    return (node);
}

 
 
/**
 * @brief       Insert node (N) before current node (C)
 * 
 * Before calling this function:
 *                                
 * +-----+    +-----+    +-----+      +-----+ 
 * |     |--->|     |--->|     |      |     |
 * |  1  |    |  C  |    |  2  |      |  N  |
 * |     |<---|     |<---|     |      |     |
 * +-----+    +-----+    +-----+      +-----+
 *                                 
 * After call to this function:
 *
 * +-----+    +-----+    +-----+    +-----+
 * |     |--->|     |--->|     |--->|     |
 * |  1  |    |  N  |    |  C  |    |  2  |
 * |     |<---|     |<---|     |<---|     |
 * +-----+    +-----+    +-----+    +-----+
 */
PORT_C_INLINE
void ndlist_add_before(struct ndlist * current, struct ndlist * node)
{
    node->next          = current;
    node->prev          = current->prev;
    current->prev->next = node;
    current->prev       = node;
}



/**
 * @brief       Insert node (N) after current node (C)
 *
 * Before calling this function:
 *                                
 * +-----+    +-----+    +-----+      +-----+ 
 * |     |--->|     |--->|     |      |     |
 * |  1  |    |  C  |    |  2  |      |  N  |
 * |     |<---|     |<---|     |      |     |
 * +-----+    +-----+    +-----+      +-----+
 *         
 * After call to this function:
 *
 * +-----+    +-----+    +-----+    +-----+
 * |     |--->|     |--->|     |--->|     |
 * |  1  |    |  C  |    |  N  |    |  2  |
 * |     |<---|     |<---|     |<---|     |
 * +-----+    +-----+    +-----+    +-----+
 */
PORT_C_INLINE
void ndlist_add_after(struct ndlist * current, struct ndlist * node)
{
    node->prev          = current;
    node->next          = current->next;
    current->next->prev = node;
    current->next       = node;
}



/**
 * @brief       Remove node (N)
 *
 * Before calling this function:
 *
 * +-----+    +-----+    +-----+
 * |     |--->|     |--->|     |
 * |  1  |    |  N  |    |  2  |
 * |     |<---|     |<---|     |
 * +-----+    +-----+    +-----+
 *
 * After call to this function:
 *
 *                      +-----------+
 * +-----+    +-----+   |  +-----+  |
 * |     |--->|     |   +->|     |--+
 * |  1  |    |  2  |      |  N  |
 * |     |<---|     |      |     |   
 * +-----+    +-----+      +-----+   
 */
PORT_C_INLINE
void ndlist_remove(struct ndlist * node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    
    /* NOTE:
     * Next pointer must point to itself, since it may be checked by 
     * ndlist_is_empty() function.
     */
    node->next = node;
    /* NOTE:
     * During debugging it is easier to catch errors with NULL pointer.
     */
    NOBLIGATION(node->prev = NULL);
}



PORT_C_INLINE
bool ndlist_is_empty(const struct ndlist * node)
{
    return (!!(node->next == node));
}



PORT_C_INLINE
struct ndlist * ndlist_next(const struct ndlist * node)
{
    return (node->next);
}



PORT_C_INLINE
struct ndlist * ndlist_prev(const struct ndlist * node)
{
    return (node->prev);
}



/**
 * @brief       Rotate list (S) to left
 *
 * Before calling this function:
 *                                
 * +-----+    +-----+    +-----+    +-----+ 
 * |     |--->|     |--->|     |<---|     |
 * |  1  |    |  S  |    |  2  |    |  3  |
 * |     |<---|     |<---|     |--->|     |
 * +-----+    +-----+    +-----+    +-----+
 *         
 * After call to this function:
 *
 * +-----+    +-----+    +-----+    +-----+ 
 * |     |--->|     |--->|     |<---|     |
 * |  1  |    |  2  |    |  S  |    |  3  |
 * |     |<---|     |<---|     |--->|     |
 * +-----+    +-----+    +-----+    +-----+
 */
PORT_C_INLINE
void ndlist_rotate_left(struct ndlist * sentinel)
{
    struct ndlist *             first;
    
    if (!ndlist_is_empty(sentinel)) {
        first = sentinel->next;
        ndlist_remove(first);
        ndlist_add_before(sentinel, first);
    }
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of dlist.h
 ******************************************************************************/
#endif /* NEON_BASE_DLIST_H_ */
