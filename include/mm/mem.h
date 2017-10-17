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
 *//**********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Memory
 * @defgroup    mem_allocator Memory
 * @brief       Memory
 ********************************************************************//** @{ */

#ifndef NEON_MM_MEM_H_
#define NEON_MM_MEM_H_

/*========================================================  INCLUDE FILES  ==*/

#include <stddef.h>

#include "port/compiler.h"
#include "base/debug.h"

/*==============================================================  MACRO's  ==*/

/**
 * @brief       Validate the pointer to memory object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @mseffect
 * @api
 */
#define N_IS_MEM_OBJECT(mem_obj)                                            \
    (((mem_obj) != NULL) && ((NSIGNATURE_OF(mem_obj) == NSIGNATURE_STATIC)  \
        || (NSIGNATURE_OF(mem_obj) == NSIGNATURE_HEAP)                      \
        || (NSIGNATURE_OF(mem_obj) == NSIGNATURE_POOL)                      \
        || (NSIGNATURE_OF(mem_obj) == NSIGNATURE_STDHEAP)))

#define NMEM_GENERIC_HEAP               nmem_get_generic_heap()

/**
 * @brief       Macro to declare a memory allocator bundle structure
 * @param       name
 *              Name of the declared memory data-type
 * @param       size
 *              Number of bytes to allocate for the memory allocator.
 * @details     This macro can be used to declare a memory region of fixed
 *              size. Then the structure can be instanced as needed.
 * @api
 */
#define NMEM_BUNDLE_STRUCT(name, size)                                      \
    struct name {                                                           \
        struct nmem             b;                                          \
        uint32_t                storage[                                    \
                                NDIVISION_ROUNDUP(size, sizeof(uint32_t))]; \
    }

/**
 * @brief       Initialize a memory allocator bundle declared by
 *              @ref NMEM_BUNDLE_STRUCT
 * @param       instance
 *              Name of the instance of the declared memory allocator data-type.
 * @param       a_nblocks
 *              Number of blocks of memory storage.
 * @param       allocator
 *              Pointer to initilization allocator function.
 * @param       signature
 *              Signature used for the memory allocator.
 * @mseffect
 * @api
 */
#define NMEM_BUNDLE_STRUCT_INIT(instance, a_nblocks, allocator, signature)  \
    {                                                                       \
        .b = {                                                              \
            .vf_alloc = allocator,                                          \
            .base = &instance.storage,                                      \
            .size = sizeof(instance.storage),                               \
            .no_blocks = a_nblocks,                                         \
            NSIGNATURE_INITIALIZER(signature)                               \
        },                                                                  \
        .storage = {0}                                                      \
    }

/**
 * @brief       Get the pointer to bundled structure.
 *              @ref NMEM_BUNDLE_STRUCT
 * @param       instance
 *              Name of the instance of the declared memory allocator data-type.
 * @api
 */
#define NMEM_FROM_BUNDLE(instance)         (&(instance)->b)

/*-----------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================  DATA TYPES  ==*/

/**
 * @brief       Memory object structure
 * @details     The structure holds virtual function pointers and common
 *              information of all memory classes.
 *
 *              If an allocator variant needs more memory for state variables
 *              it will allocate the memory from given storage.
 * 
 *              Cache the function pointers for faster access of alloc and free
 *              functions.
 * @api
 */
struct nmem
{
    /**@brief   Allocate VF pointer */
    void *                   (* vf_alloc)(struct nmem *, size_t);
    /**@brief   Free VF pointer */
    void                     (* vf_free) (struct nmem *, void *);
    void *                      base;   /**<@brief Base address              */
    size_t                      free;   /**<@brief Free bytes                */
    size_t                      size;   /**<@brief Size of memory            */
    /**@brief   Number of blocks */
    uint32_t                    no_blocks;
    NSIGNATURE_DECLARE    				/**<@brief Memory object signature   */
};

/**
 * @brief       Memory object type
 * @api
 */
typedef struct nmem nmem;

/*=====================================================  GLOBAL VARIABLES  ==*/
/*==================================================  FUNCTION PROTOTYPES  ==*/


/**
 * @brief       Allocate memory from specified memory object
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
void * nmem_alloc_i(struct nmem * mem_obj, size_t size)
{
    return (mem_obj->vf_alloc(mem_obj, size));
}



/**
 * @brief       Allocate memory from specified memory object
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
void * nmem_alloc(struct nmem * mem_obj, size_t size);



/**
 * @brief       Allocate memory from specified memory object and zero it out
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
void * nmem_zalloc(struct nmem * mem_obj, size_t size);



/**
 * @brief       Free the allocated memory
 * @param       mem_obj
 *              Pointer to memory object
 * @param       mem_storage
 *              Pointer to previously allocated memory
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @api
 */
PORT_C_INLINE
void nmem_free_i(struct nmem * mem_obj, void * mem_storage)
{
    mem_obj->vf_free(mem_obj, mem_storage);
}



/**
 * @brief       Free the allocated memory
 * @param       mem_obj
 *              Pointer to memory object
 * @param       mem_storage
 *              Pointer to previously allocated memory
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
void nmem_free(struct nmem * mem_obj, void * mem_storage);



/**
 * @brief       Return the number of free bytes in specified memory object
 * @param       mem_obj
 *              Pointer to memory object
 * @return      The number of free bytes in specified memory object
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
PORT_C_INLINE
size_t nmem_get_free(struct nmem * mem_obj)
{
    return (mem_obj->free);
}



/**
 * @brief       Return the size of specified memory object in bytes
 * @param       mem_obj
 *              Pointer to memory object
 * @return      The size of specified memory object in bytes
 * @note        The function does not check for pointer validity. Use
 *              @ref N_IS_MEM_OBJECT() macro before calling this function.
 * @iclass
 */
PORT_C_INLINE
size_t nmem_get_size(struct nmem * mem_obj)
{
    return (mem_obj->size);
}



PORT_C_INLINE
void nmem_set_generic_heap(struct nmem * mem_obj)
{
    extern struct nmem *        g_generic_heap_;

    g_generic_heap_ = mem_obj;
}



PORT_C_INLINE
struct nmem * nmem_get_generic_heap(void)
{
    extern struct nmem *        g_generic_heap_;

    return (g_generic_heap_);
}

/*-------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===============================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//*****************************************************
 * END of mem.h
 *****************************************************************************/
#endif /* NEON_MM_MEM_H_ */
