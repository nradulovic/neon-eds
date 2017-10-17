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
 * @brief       Pool Memory management
 * @defgroup    mem_pool Pool Memory management
 * @brief       Pool Memory management
 *********************************************************************//** @{ */

#ifndef NEON_MM_POOL_H_
#define NEON_MM_POOL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "base/bitop.h"
#include "mm/mem.h"

/*===============================================================  MACRO's  ==*/

#define NPOOL_COMPUTE_SIZE(n_of_blocks, block_size)                         \
    ((n_of_blocks) * (NALIGN_UP(block_size, sizeof(NCPU_DATA_ALIGNMENT))))

#define NPOOL_BUNDLE_STRUCT(name, block_size, n_of_blocks)                  \
    NMEM_BUNDLE_STRUCT(name, NPOOL_COMPUTE_SIZE(n_of_blocks, block_size))

#define NPOOL_BUNDLE_STRUCT_INIT(instance, n_of_blocks)                     \
    NMEM_BUNDLE_STRUCT_INIT(instance, n_of_blocks, pool_init_alloc,         \
        NSIGNATURE_POOL)

#define NPOOL_BUNDLE_DEFINE(name, block_size, n_of_blocks)                  \
    NPOOL_BUNDLE_STRUCT(name, block_size, n_of_blocks) name =               \
        NPOOL_BUNDLE_STRUCT_INIT(name, n_of_blocks)

#define NPOOL_FROM_BUNDLE(instance)     NMEM_FROM_BUNDLE(instance)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void * pool_init_alloc(struct nmem * pool_obj, size_t size);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of pool.h
 ******************************************************************************/
#endif /* NEON_MM_POOL_H_ */
