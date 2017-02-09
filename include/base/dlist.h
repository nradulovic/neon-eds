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

#define NDLIST_INITIALIZER(list)        {(list), (list)}

#define NDLIST_ENTRY(ptr, type, member)                                         \
    PORT_C_CONTAINER_OF(ptr, type, member)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct ndlist
{
    struct ndlist *     next;
    struct ndlist *     prev;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE
struct ndlist * ndlist_init(struct ndlist * node)
{
    node->next = node;
    node->prev = node;

    return (node);
}



PORT_C_INLINE
void ndlist_add_before(struct ndlist * current, struct ndlist * node)
{
    node->next          = current;
    node->prev          = current->prev;
    current->prev->next = node;
    current->prev       = node;
}



PORT_C_INLINE
void ndlist_add_after(struct ndlist * current, struct ndlist * node)
{
    node->prev          = current;
    node->next          = current->next;
    current->next->prev = node;
    current->next       = node;
}



PORT_C_INLINE
void ndlist_remove(struct ndlist * node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    /** NOTE:
     *  The list is tested if it is empty by checking if next points to itself.
     */
    node->next = node;
    /** NOTE:
     *  During debugging it is easier to catch errors with NULL pointer.
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

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of dlist.h
 ******************************************************************************/
#endif /* NEON_BASE_DLIST_H_ */