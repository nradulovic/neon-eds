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
 * @brief       Bias linked lists header
 * @addtogroup  base_intf
 *********************************************************************//** @{ */
/**@defgroup    base_bias_list Bias linked lists
 * @brief       Bias linked lists
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_BIAS_LIST_H_
#define NEON_BASE_BIAS_LIST_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stdint.h>

#include "base/dlist.h"
#include "port/compiler.h"

/*===============================================================  MACRO's  ==*/

#define ndlist_to_bias_list(node)                                               \
    ndlist_entry(node, struct nbias_list, list)

#define NBIAS_LIST_MAX_PRIO             255u


#define NBIAS_LIST_INITIALIZER(name, priority)                                  \
    {                                                                           \
        .list = NDLIST_INITIALIZER(name.list),                                  \
        .bias = priority & 0xffu                                                \
    }

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nbias_list
{
    struct ndlist               list;
    uint_fast8_t                bias;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE
void nbias_list_init(struct nbias_list * node, uint_fast8_t bias)
{
    ndlist_init(&node->list);
    node->bias = bias;
}



PORT_C_INLINE
void nbias_list_reinit(struct nbias_list * node)
{
    ndlist_init(&node->list);
}



PORT_C_INLINE
void nbias_list_term(struct nbias_list * node)
{
    node->bias = 0;
    ndlist_init(&node->list);
}



PORT_C_INLINE
void nbias_list_sort_insert(struct nbias_list * list, struct nbias_list * node)
{
    struct ndlist *             current;

    for (NDLIST_FOR_EACH(current, &list->list)) {

        if (ndlist_to_bias_list(current)->bias >= node->bias) {
            ndlist_add_before(current, &node->list);
            break;
        }
    }
}



PORT_C_INLINE
void nbias_list_fifo_insert(struct nbias_list * list, struct nbias_list * node)
{
    ndlist_add_before(&list->list, &node->list);
}



PORT_C_INLINE
void nbias_list_remove(struct nbias_list * node)
{
    ndlist_remove(&node->list);
}



PORT_C_INLINE
struct nbias_list * nbias_list_tail(const struct nbias_list * list)
{
    return (ndlist_to_bias_list(ndlist_next(&list->list)));
}



PORT_C_INLINE
struct nbias_list * nbias_list_head(struct nbias_list * list)
{
    return (ndlist_to_bias_list(ndlist_prev(&list->list)));
}



PORT_C_INLINE
bool nbias_list_is_empty(const struct nbias_list * list)
{
    return (ndlist_is_empty(&list->list));
}



PORT_C_INLINE
struct nbias_list * nbias_list_next(const struct nbias_list * node)
{
    return (ndlist_to_bias_list(ndlist_next(&node->list)));
}



PORT_C_INLINE
uint_fast8_t nbias_list_get_bias(const struct nbias_list * node)
{
    return (node->bias);
}



PORT_C_INLINE
void nbias_list_set_bias(struct nbias_list * node, uint_fast8_t bias)
{
    node->bias = bias;
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of bias_list.h
 ******************************************************************************/
#endif /* NEON_BASE_BIAS_LIST_H_ */

