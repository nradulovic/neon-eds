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
 * @brief       Standard Heap Memory management implementation
 * @addtogroup  mm_static
 *********************************************************************//** @{ */
/**@defgroup    mm_stdheap_impl Implementation
 * @brief       Standard Heap Memory management implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stdlib.h>

#include "port/core.h"
#include "base/component.h"
#include "mm/stdheap.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


static void * stdheap_alloc_i(
    struct nmem *               mem_class,
    size_t                      size);



static void stdheap_free_i(
    struct nmem *               mem_class,
    void *                      mem);

/*=======================================================  LOCAL VARIABLES  ==*/

static const NCOMPONENT_DEFINE("Static Memory Management");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static void * stdheap_alloc_i(
    struct nmem *               mem_class,
    size_t                      size)
{
    NREQUIRE(NAPI_POINTER, mem_class != NULL);
    NREQUIRE(NAPI_OBJECT,  mem_class->signature == NSIGNATURE_STDHEAP);
    NREQUIRE(NAPI_USAGE, ncore_is_lock_valid());

    (void)mem_class;

    return (malloc(size));
}

static void stdheap_free_i(
    struct nmem *               mem_class,
    void *                      mem)
{
	NREQUIRE(NAPI_POINTER, mem_class != NULL);
	NREQUIRE(NAPI_OBJECT,  mem_class->signature == NSIGNATURE_STDHEAP);
	NREQUIRE(NAPI_USAGE, ncore_is_lock_valid());

	(void)mem_class;
	free(mem);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nstdheap_init(
    struct nstdheap *           stdheap_obj)
{
    NREQUIRE(NAPI_POINTER, stdheap_obj != NULL);
    NREQUIRE(NAPI_OBJECT,  stdheap_obj->mem_class.signature != NSIGNATURE_STDHEAP);

    stdheap_obj->mem_class.base     = NULL;
    stdheap_obj->mem_class.size     = 0u;
    stdheap_obj->mem_class.free     = 0u;
    stdheap_obj->mem_class.vf_alloc = stdheap_alloc_i;
    stdheap_obj->mem_class.vf_free  = stdheap_free_i;

    NOBLIGATION(stdheap_obj->mem_class.signature = NSIGNATURE_STDHEAP);
}



void * nstdheap_alloc_i(struct nstdheap * stdheap_obj, size_t size)
{
    return (stdheap_alloc_i(&stdheap_obj->mem_class, size));
}



void nstdheap_free_i(
    struct nstdheap *           stdheap_obj,
    void *                      mem)
{
	stdheap_free_i(&stdheap_obj->mem_class, mem);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of stdheap.c
 ******************************************************************************/
