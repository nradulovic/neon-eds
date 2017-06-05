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
 * @brief       Memory class implementation
 * @addtogroup  mm_mem
 *********************************************************************//** @{ */
/**@defgroup    mm_mem_impl Implementation
 * @brief       Memory class Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "port/core.h"
#include "mm/mem.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

struct nmem *                   g_generic_heap_;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


void * nmem_alloc(
    struct nmem *               mem,
    size_t                      size)
{
    ncore_lock                  sys_lock;
    void *                      mem_storage;

    ncore_lock_enter(&sys_lock);
    mem_storage = nmem_alloc_i(mem, size);
    ncore_lock_exit(&sys_lock);

    return (mem_storage);
}



void * nmem_zalloc(
    struct nmem *               mem,
    size_t                      size)
{
    void *                      mem_storage;

    mem_storage = nmem_alloc(mem, size);

    if (mem_storage) {
        memset(mem_storage, 0, size);
    }

    return (mem_storage);
}



void nmem_free(
    struct nmem *               mem,
    void *                      mem_storage)
{
    ncore_lock                  sys_lock;

    ncore_lock_enter(&sys_lock);
    nmem_free_i(mem, mem_storage);
    ncore_lock_exit(&sys_lock);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem_class.c
 ******************************************************************************/
