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
 * @brief       Static Memory management implementation
 * @addtogroup  mm_static
 *********************************************************************//** @{ */
/**@defgroup    mm_static_impl Implementation
 * @brief       Static Memory management implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "port/core.h"
#include "base/bitop.h"
#include "mm/static.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void * static_alloc(struct nmem * static_obj, size_t size);

static void static_free(struct nmem * static_obj, void * mem);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void * static_alloc(struct nmem * static_obj, size_t size)
{
    NREQUIRE(NSIGNATURE_IS(static_obj, NSIGNATURE_STATIC));
    NREQUIRE((size != 0u) && (size < INT32_MAX));
    NREQUIRE(ncore_is_lock_valid());

    size = NALIGN_UP(size, NCPU_DATA_ALIGNMENT);

    if (size <= static_obj->free) {
        static_obj->free -= size;

        return ((void *)&((uint8_t *)static_obj->base)[static_obj->free]);
    } else {

        return (NULL);
    }
}

static void static_free(struct nmem * static_obj, void * mem)
{
    (void)mem;
    (void)static_obj;

    NASSERT_ALWAYS("illegal static memory call");
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void * static_init_alloc(struct nmem * static_obj, size_t size)
{
    NREQUIRE(NSIGNATURE_IS(static_obj, NSIGNATURE_STATIC));
    NREQUIRE(static_obj->base);
    NREQUIRE(static_obj->size >= NCPU_DATA_ALIGNMENT);

    static_obj->free     = static_obj->size;
    static_obj->vf_alloc = static_alloc;
    static_obj->vf_free  = static_free;

    return (static_alloc(static_obj, size));
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of static_mem.c
 ******************************************************************************/
