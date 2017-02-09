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

#include "../base/dlist.h"
#include "port/core.h"
#include "base/config.h"
#include "base/bias_list.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nsched_deferred
{
	struct ndlist 				list;
	void 					 (* fn)(void * arg);
	void *						arg;
#if (CONFIG_API_VALIDATION == 1)
    unsigned int                signature;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void nsched_deferred_init(struct nsched_deferred * deferred, void (* fn)(void *), void * arg);

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
