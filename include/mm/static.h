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
 * @brief       Static Memory management
 * @defgroup    mem_static Static Memory management
 * @brief       Static Memory Management
 *********************************************************************//** @{ */

#ifndef NEON_MM_STATIC_H_
#define NEON_MM_STATIC_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "base/debug.h"
#include "mm/mem.h"

/*===============================================================  MACRO's  ==*/

#define NSTATICMEM_BUNDLE_STRUCT(name, size)                                \
    NMEM_BUNDLE_STRUCT(name, size)

#define NSTATICMEM_BUNDLE_STRUCT_INIT(instance)                             \
    NMEM_BUNDLE_STRUCT_INIT(instance, 1, static_init_alloc, NSIGNATURE_STATIC)

#define NSTATICMEM_BUNDLE_DEFINE(name, size)                                \
    NSTATICMEM_BUNDLE_STRUCT(name, size) name =                             \
        NSTATICMEM_BUNDLE_STRUCT_INIT(name)

#define NSTATICMEM_FROM_BUNDLE(instance) NMEM_FROM_BUNDLE(instance)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void * static_init_alloc(struct nmem * static_obj, size_t size);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of static.h
 ******************************************************************************/
#endif /* NEON_MM_STATIC_H_ */
