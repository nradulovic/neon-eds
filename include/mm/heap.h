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
 * @brief       Heap Memory management
 * @defgroup    mem_heap Heap Memory management
 * @brief       Heap Memory management
 *********************************************************************//** @{ */

#ifndef NEON_MM_HEAP_H_
#define NEON_MM_HEAP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "mm/mem.h"
#include "base/debug.h"
#include "base/bitop.h"

/*===============================================================  MACRO's  ==*/

#define NHEAP_BUNDLE_STRUCT(name, size)                                     \
    NMEM_BUNDLE_STRUCT(name, size)

#define NHEAP_BUNDLE_STRUCT_INIT(instance)                                  \
    NMEM_BUNDLE_STRUCT_INIT(instance, 1, heap_init_alloc, NSIGNATURE_HEAP)

#define NHEAP_BUNDLE_DEFINE(name, size)                                     \
    NHEAP_BUNDLE_STRUCT(name, size) name =                                  \
        NHEAP_BUNDLE_STRUCT_INIT(name)

#define NHEAP_FROM_BUNDLE(instance)     NMEM_FROM_BUNDLE(instance)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void * heap_init_alloc(struct nmem * mem_obj, size_t size);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of heap.h
 ******************************************************************************/
#endif /* NEON_MM_HEAP_H_ */
