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
 * @brief       Object management header
 * @defgroup    object_obj Object management
 * @brief       Object management
 *********************************************************************//** @{ */

#ifndef NEON_OBJECT_OBJ_H_
#define NEON_OBJECT_OBJ_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "base/list.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nmem;
struct nobj_component;

struct nobj_class
{
    size_t                      size;
    struct nobj_component *  (* ctor)(struct nobj_component * component, struct nmem * mem, void * arg);
    struct nobj_component *  (* dtor)(struct nobj_component * component, struct nmem * mem);
};

struct nobj_component
{
    const struct nobj_class *   class;
    struct ndlist               list;
};

struct nobj_object
{
    struct nobj_component       component;
    struct nmem *               mem;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


struct nobj_object * nobj_create(
    const struct nobj_class *   obj_class,
    struct nmem *               mem,
    void *                      arg);



void nobj_delete(struct nobj_object * object);



struct nobj_component * nobj_add_component(
    const struct nobj_class *   component_class,
    struct nobj_object *        object,
    void *                      arg);



void nobj_remove_component(struct nobj_component * component);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of obj.h
 ******************************************************************************/
#endif /* NEON_OBJECT_OBJ_H_ */
