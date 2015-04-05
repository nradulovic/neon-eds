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
 * @brief       Object management implementation
 * @addtogroup  object_obj
 *********************************************************************//** @{ */
/**@defgroup    object_obj_impl Implementation
 * @brief       Object management implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "mm/mem.h"
#include "object/obj.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static struct nobj_object * component_to_object(
    struct nobj_component *     component);



static struct nobj_component * component_create(
    const struct nobj_class *   component_class,
    struct nmem *               mem,
    void *                      arg);



static void component_delete(
    struct nobj_component *     component,
    struct nmem *               mem);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static struct nobj_object * component_to_object(
    struct nobj_component *     component)
{
    return (CONTAINER_OF(component, struct nobj_object, component));
}



static struct nobj_component * component_create(
    const struct nobj_class *   component_class,
    struct nmem *               mem,
    void *                      arg)
{
    struct nobj_component *     component;

    component = nmem_alloc(mem, component_class->size);

    if (component) {
        component->class = component_class;
        ndlist_init(&component->list);

        if (component_class->ctor) {
            component = component_class->ctor(component, mem, arg);
        }
    }

    return (component);
}



static void component_delete(
    struct nobj_component *     component,
    struct nmem *               mem)
{
    const struct nobj_class *   component_class;

    ndlist_term(&component->list);
    component_class = component->class;

    if (component_class->dtor) {
        component = component_class->dtor(component, mem);
    }
    nmem_free(mem, component);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


struct nobj_object * nobj_create(
    const struct nobj_class *   obj_class,
    struct nmem *               mem,
    void *                      arg)
{
    struct nobj_component *     component;
    struct nobj_object *        object;

    component = component_create(obj_class, mem, arg);

    if (component) {
        object = component_to_object(component);
        object->mem = mem;
    }

    return (object);
}



void nobj_delete(struct nobj_object * object)
{
    while (!ndlist_is_empty(&object->component.list)) {
        struct nobj_component * component;

        component = CONTAINER_OF(
            ndlist_prev(&object->component.list), struct nobj_component, list);
        nobj_remove_component(component);
    }
    component_delete(&object->component, object->mem);
}



struct nobj_component * nobj_add_component(
    const struct nobj_class *   component_class,
    struct nobj_object *        object,
    void *                      arg)
{
    struct nobj_component *     component;

    component = component_create(component_class, object->mem, arg);

    if (component) {
        ndlist_add_after(&object->component.list, &component->list);
    }

    return (component);
}



void nobj_remove_component(struct nobj_component * component)
{
    struct nobj_object *        object;

    ndlist_remove(&component->list);
    object = component_to_object(component);
    component_delete(component, object->mem);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of obj.c
 ******************************************************************************/
