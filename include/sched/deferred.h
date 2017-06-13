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
 * @brief       Deferred work
 * @defgroup    sched Scheduler
 * @brief       Deferred work
 *********************************************************************//** @{ */

#ifndef NEON_SCHED_DEFERRED_H_
#define NEON_SCHED_DEFERRED_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "base/config.h"
#include "base/dlist.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Deferred job structure
 * @api
 */
struct nsched_deferred
{
	struct ndlist 				list;                  /* Linked list of jobs */               
	void 					 (* fn)(void * arg);   /* Associated job function */
	void *						arg;                     /* Funciton argument */
#if (CONFIG_API_VALIDATION == 1)
    unsigned int                signature;        /* This structure signature */
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Initialize deferred job structure
 * @details     This function must be invoked before using deferred job. Once 
 *              initialized job can be invoked multiple times.
 * @param       deferred
 *              Pointer to allocated deferred job structure.
 * @param       fn
 *              Pointer to deferred job function which will be executed when 
 *              job is activated. The function has the following signature:
 *              void function(void * argument);
 * @param       arg
 *              This is a pointer that will be passed to deferred job function.
 * @api
 */
void nsched_deferred_init(struct nsched_deferred * deferred, 
        void (* fn)(void *), void * arg);

/**@brief       Activate a deferred job function.
 * @details     This function will activate the associated deferred job 
 *              function as soon as possible.
 * @param       deferred
 *              Pointer to allocated deferred job function.
 * @api
 */
void nsched_deferred_do(struct nsched_deferred * deferred);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of deferred.h
 ******************************************************************************/
#endif /* NEON_SCHED_DEFERRED_H_ */
