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
 * @brief       Common Neon include header
 * @defgroup    intf Neon Interface
 * @brief       Interface
 *********************************************************************//** @{ */

#ifndef NEON_NEON_EDS_H_
#define NEON_NEON_EDS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "eds_port.h"
#include "eds_base.h"
#include "eds_timer.h"
#include "eds_mm.h"
#include "eds_sched.h"
#include "eds_ep.h"

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
