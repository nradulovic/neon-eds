/*
 * This file is part of eSolid.
 *
 * Copyright (C) 2010 - 2013 Nenad Radulovic
 *
 * eSolid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with eSolid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Pool Memory Management module header
 * @defgroup    base_pool_mem Pool Memory management
 * @brief       Pool Memory management
 *********************************************************************//** @{ */
/**@defgroup    base_pool_mem_intf Interface
 * @brief       Pool memory API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_POOL_MEM_H_
#define ES_POOL_MEM_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>

#include "arch/compiler.h"
#include "base/debug.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Pool memory instance
 * @details     This structure holds information about pool memory instance.
 * @p           This structure hold information about pool and block sizes.
 *              Additionally, it holds a guard member which will ensure mutual
 *              exclusion in preemption environments.
 * @see         esPoolMemInit()
 * @api
 */
struct esPoolMem {
    struct poolMemBlock *  sentinel;                                               /**<@brief Pointer to the pool sentinel                     */
    size_t              size;                                                   /**<@brief The size of pool memory                          */
    size_t              blockSize;                                              /**<@brief Size of one block                                */
#if (1U == CONFIG_DEBUG_API_VALIDATION) || defined(__DOXYGEN__)
    portReg             signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Pool memory instance poolMem type
 * @api
 */
typedef struct esPoolMem esPoolMem;

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Default pool memory instance
 * @api
 */
extern esPoolMem esGlobalPoolMem;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Initializes pool memory instance
 * @param       poolMem
 *              Pointer to pool memory instance, see @ref esPoolMem.
 * @param       pool
 *              Reserved memory area for pool allocator.
 * @param       poolSize
 *              The size of reserved memory area expressed in bytes.
 * @param       blockSize
 *              The size of one block expressed in bytes.
 * @details     This function must be called before any call to esPoolMemAllocI()
 *              or esPoolMemAlloc().
 * @warning     Pointers to @c poolMem and @c pool must be aligned to CPU defined
 *              alignment.
 * @api
 */
void esPoolMemInit(
    esPoolMem *         poolMem,
    void *              pool,
    size_t              poolSize,
    size_t              blockSize);

/**@brief       Calculates required reserved memory size for defined number of
 *              blocks.
 * @param       blocks
 *              Number of required blocks.
 * @param       blockSize
 *              The size of one block.
 * @return      Required reserved memory size.
 * @api
 */
size_t esPoolMemComputeSize(
    size_t              blocks,
    size_t              blockSize);

/**@brief       Allocate one block from memory pool
 * @param       poolMem
 *              Pointer to pool memory instance, see @ref esPoolMem.
 * @return      Pointer to requested block.
 * @iclass
 */
void * esPoolMemAllocI(
    esPoolMem *         poolMem);

/**@brief       Allocate one block from memory pool
 * @param       poolMem
 *              Pointer to pool memory instance, see @ref esPoolMem.
 * @return      Pointer to requested block.
 * @api
 */
void * esPoolMemAlloc(
    esPoolMem *         poolMem);

/**
 * @brief       Oslobadja prethodno alocirani blok
 * @param       [in] poolMem             Deskriptor pool alokatora
 * @param       [in] mem                Prethodno alociran blok memorije
 * @iclass
 */
void esPoolMemDeAllocI(
    esPoolMem *         poolMem,
    void *              mem);

/**
 * @brief       Oslobadja prethodno alocirani blok
 * @param       [in] poolMem             Deskriptor pool alokatora
 * @param       [in] mem                Prethodno alociran blok memorije
 * @note        Funkcija koristi makroe @ref ES_CRITICAL_LOCK_ENTER i
 *              @ref ES_CRITICAL_LOCK_EXIT za zastitu memorije od istovremenog
 *              pristupa.
 * @api
 */
void esPoolMemDeAlloc(
    esPoolMem *         poolMem,
    void *              mem);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of pool_mem.h
 ******************************************************************************/
#endif /* ES_POOL_MEM_H_ */
