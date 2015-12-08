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
 * @brief       Memory class
 * @defgroup    mem_class Memory class
 * @brief       Memory class
 *********************************************************************//** @{ */

#ifndef NEON_MM_MEM_H_
#define NEON_MM_MEM_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "port/compiler.h"
#include "base/debug.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Validate the pointer to memory object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#define N_IS_MEM_OBJECT(mem_obj)                                                \
    (((mem_obj) != NULL) && (((mem_obj)->signature == NSIGNATURE_STATIC) ||     \
                         ((mem_obj)->signature == NSIGNATURE_HEAP) ||           \
                         ((mem_obj)->signature == NSIGNATURE_POOL) ||           \
                         ((mem_obj)->signature == NSIGNATURE_STDHEAP)))

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Memory object structure
 * @details     The structure holds virtual function pointers and common
 *              information of all memory classes.
 *
 *              In order to initialise the structure one of npool_init(),
 *              nheap_init() or nstatic_init() functions must be called.
 * @api
 */
struct nmem
{
                                        /**<@brief Allocate VF pointer        */
    void *                   (* vf_alloc)(struct nmem *, size_t);
                                        /**<@brief Free VF pointer            */
    void                     (* vf_free) (struct nmem *, void *);
    void *                      base;   /**<@brief Base address               */
    size_t                      free;   /**<@brief Free bytes                 */
    size_t                      size;   /**<@brief Size of memory             */
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
                                        /**<@brief Memory object signature    */
    unsigned int                signature;
#endif
};

/**@brief       Memory object type
 * @api
 */
typedef struct nmem nmem;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Allocate memory from specified memory object
 * @param       mem_obj
 *              Pointer to memory object
 * @param       size
 *              The size of requested memory in bytes
 * @return      The pointer to allocated memory
 *              - @retval NULL - No available memory storage
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
PORT_C_INLINE
void * nmem_alloc_i(
    struct nmem *               mem_obj,
    size_t                      size)
{
    return (mem_obj->vf_alloc(mem_obj, size));
}



/**@brief       Allocate memory from specified memory object
 * @param       mem_obj
 *              Pointer to memory object
 * @param       size
 *              The size of requested memory in bytes
 * @return      The pointer to allocated memory
 *              - @retval NULL - No available memory storage
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @api
 */
void * nmem_alloc(
    struct nmem *               mem_obj,
    size_t                      size);



/**@brief       Allocate memory from specified memory object and zero it out
 *              before returning a pointer
 * @param       mem_obj
 *              Pointer to memory object
 * @param       size
 *              The size of requested memory in bytes
 * @return      The pointer to allocated memory
 *              - @retval NULL - No available memory storage
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @api
 */
void * nmem_zalloc(
    struct nmem *               mem_obj,
    size_t                      size);



/**@brief       Free the allocated memory
 * @param       mem_obj
 *              Pointer to memory object
 * @param       mem_storage
 *              Pointer to previously allocated memory
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @api
 */
PORT_C_INLINE
void nmem_free_i(
    struct nmem *               mem_obj,
    void *                      mem_storage)
{
    mem_obj->vf_free(mem_obj, mem_storage);
}



/**@brief       Free the allocated memory
 * @param       mem_obj
 *              Pointer to memory object
 * @param       mem_storage
 *              Pointer to previously allocated memory
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
void nmem_free(
    struct nmem *               mem_obj,
    void *                      mem_storage);



/**@brief       Return the number of free bytes in specified memory object
 * @param       mem_obj
 *              Pointer to memory object
 * @return      The number of free bytes in specified memory object
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
PORT_C_INLINE
size_t nmem_get_free_i(
    struct nmem *               mem_obj)
{
    return (mem_obj->free);
}



/**@brief       Return the size of specified memory object in bytes
 * @param       mem_obj
 *              Pointer to memory object
 * @return      The size of specified memory object in bytes
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
PORT_C_INLINE
size_t nmem_get_size_i(
    struct nmem *               mem_obj)
{
    return (mem_obj->size);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem_obj.h
 ******************************************************************************/
#endif /* NEON_MM_MEM_H_ */
