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
 * @brief       Main Neon include header
 * @defgroup    intf Neon Interface
 * @brief       Interface
 *********************************************************************//** @{ */
/**@defgroup    port_intf Port
 * @brief       Port layer
 */
/**@defgroup    base_intf Base
 * @brief       Base
 */
/**@defgroup    timer_intf Timer
 * @brief       Timer
 */
/**@defgroup    mm_intf Memory Management
 * @brief       Memory Management
 */
/**@defgroup    sched_intf Cooperative scheduler
 * @brief       Cooperative scheduler
 */
/**@defgroup    ep_intf Event Processing
 * @brief       Event Processing
 */

#ifndef NEON_NEON_EDS_H_
#define NEON_NEON_EDS_H_

/*=========================================================  INCLUDE FILES  ==*/

/* EDS Port */
#include "port/compiler.h"
#include "port/core.h"

/* EDS Base */
#include "base/bias_list.h"
#include "base/bitop.h"
#include "base/component.h"
#include "base/config.h"
#include "base/debug.h"
#include "base/dlist.h"
#include "base/error.h"
#include "base/queue.h"

/* EDS Timer */
#include "timer/timer.h"

/* EDS Memory Management */
#include "mm/heap.h"
#include "mm/mem.h"
#include "mm/pool.h"
#include "mm/static.h"
#include "mm/stdheap.h"

/* EDS Scheduler */
#include "sched/sched.h"

/* EDS Event Procesing */
#include "ep/epa.h"
#include "ep/equeue.h"
#include "ep/etimer.h"
#include "ep/event.h"
#include "ep/smp.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Event Driven System identification
 *//** @{ *//*----------------------------------------------------------------*/

/**@brief       Version string.
 */
#define NEDS_VERSION                    "1.0"

/**@brief       Version major number.
 */
#define NEDS_VERSION_MAJOR              1

/**@brief       Version minor number.
 */
#define NEDS_VERSION_MINOR              0

/** @} */

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of neon_eds.h
 ******************************************************************************/
#endif /* NEON_NEON_EDS_H_ */
