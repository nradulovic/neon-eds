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
 * @brief       Standard Heap Memory management implementation
 * @addtogroup  mm_static
 *********************************************************************//** @{ */
/**@defgroup    mm_stdheap_impl Implementation
 * @brief       Standard Heap Memory management implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <stdlib.h>

#include "port/core.h"
#include "mm/stdheap.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Validate the pointer to heap memory object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#define N_IS_STDHEAP_OBJECT(mem_obj)                                            \
    (NSIGNATURE_OF(mem_obj) == NSIGNATURE_STDHEAP)
    
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void * stdheap_alloc_i(struct nmem * mem_class, size_t size);

static void stdheap_free_i(struct nmem * mem_class, void * mem);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void * stdheap_alloc_i(struct nmem * mem_class, size_t size)
{
    NREQUIRE(N_IS_STDHEAP_OBJECT(mem_class));
    NREQUIRE(ncore_is_lock_valid());

    (void)mem_class;

    return (malloc(size));
}

static void stdheap_free_i(struct nmem * mem_class, void * mem)
{
    NREQUIRE(N_IS_STDHEAP_OBJECT(mem_class));
    NREQUIRE(ncore_is_lock_valid());

    (void)mem_class;
    free(mem);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void nstdheap_init(struct nstdheap * stdheap_obj)
{
    NREQUIRE(stdheap_obj);
    NREQUIRE(NSIGNATURE_OF(&stdheap_obj->mem_class) != NSIGNATURE_STDHEAP);

    stdheap_obj->mem_class.base     = NULL;
    stdheap_obj->mem_class.size     = 0u;
    stdheap_obj->mem_class.free     = 0u;
    stdheap_obj->mem_class.vf_alloc = stdheap_alloc_i;
    stdheap_obj->mem_class.vf_free  = stdheap_free_i;

    NOBLIGATION(NSIGNATURE_IS(&stdheap_obj->mem_class, NSIGNATURE_STDHEAP));
}



void * nstdheap_alloc_i(struct nstdheap * stdheap_obj, size_t size)
{
    return (stdheap_alloc_i(&stdheap_obj->mem_class, size));
}



void nstdheap_free_i(struct nstdheap * stdheap_obj, void * mem)
{
    stdheap_free_i(&stdheap_obj->mem_class, mem);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of stdheap.c
 ******************************************************************************/
