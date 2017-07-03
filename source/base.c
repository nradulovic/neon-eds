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
 * @brief       Base implementation
 * @addtogroup  base
 *********************************************************************//** @{ */
/**@defgroup    base_impl Implementation
 * @brief       Base implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "base/queue.h"
#include "base/debug.h"
#include "base/error.h"
#include "mm/mem.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define ERROR_TEXT(a, b, c)             c,

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

const char * const              g_error_text[NP_LAST_ERROR_NUMBER] =
{
    NP_ERROR_TABLE(ERROR_TEXT)
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

#if (CONFIG_DYNAMIC_QUEUE == 1) || defined(__DOXYGEN__)
struct nqueue * nqueue_alloc(struct nmem * mem, uint32_t elements)
{
    struct nqueue * 			queue = NULL;
    void **                     buf = NULL;

	NREQUIRE(N_IS_MEM_OBJECT(mem));
	NREQUIRE((elements >= 2) && N_IS_POWEROF_2(elements));

    if ((elements >=2) && N_IS_POWEROF_2(elements)) {
        queue = nmem_alloc(mem, sizeof(struct nqueue));
    		           
        if (queue) {
            buf = nmem_alloc(mem,  elements * sizeof(void * [1]))
            
            if (!buf) {
                nmem_free(mem, queue);
            }
        }
            
        if (queue && buf) {
            queue->mem = mem;
        	queue->head = 0u;
        	queue->tail = 0u;
        	queue->empty = elements;
        	queue->mask = elements - 1u;
        	queue->buf = buf;            
        }
    }
    NENSURE(queue);
    
    return (queue);
}
#endif

#if (CONFIG_DYNAMIC_QUEUE == 1) || defined(__DOXYGEN__)
void nqueue_free(struct nqueue * queue)
{
    nmem_free(queue->mem, buf);
	nmem_free(queue->mem, queue);
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of base.c
 ******************************************************************************/
