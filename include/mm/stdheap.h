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
 * @brief       Standard Heap Memory management
 * @defgroup    mem_stdheap Standard Heap Memory management
 * @brief       Standard Heap Memory Management
 *********************************************************************//** @{ */

#ifndef NEON_MM_STDHEAP_H_
#define NEON_MM_STDHEAP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "mm/mem.h"

/*===============================================================  MACRO's  ==*/

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Static memory object structure
 * @details     This structure holds information about static memory instance.
 * @api
 */
struct nstdheap
{
    struct nmem                 mem_class;
};

/**@brief       Static memory instance handle type
 * @api
 */
typedef struct nstdheap nstdheap;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Initializes standard heap memory instance
 * @param       stdheap_obj
 *              Pointer to standard heap object
 * @details     This function shall be called before any other standard heap
 *              memory management function.
 * @api
 */
void nstdheap_init(
    struct nstdheap *           stdheap_obj);



/**@brief       Allocates static memory of get_size @c get_size
 * @param       stdheap_obj
 *              Pointer to static object
 * @param       size
 *              The size of requested memory in bytes.
 * @return      Pointer to free memory of requested get_size.
 * @iclass
 */
void * nstdheap_alloc_i(
    struct nstdheap *           stdheap_obj,
    size_t                      size);



void nstdheap_free_i(
    struct nstdheap *           stdheap_obj,
    void *                      mem);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of stdheap.h
 ******************************************************************************/
#endif /* NEON_MM_STDHEAP_H_ */
