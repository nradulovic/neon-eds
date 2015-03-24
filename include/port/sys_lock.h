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
 * @brief       System lock
 * @defgroup    syslock_intf System lock
 * @brief       System lock
 *********************************************************************//** @{ */

#ifndef NEON_PORT_SYS_LOCK_H_
#define NEON_PORT_SYS_LOCK_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/p_sys_lock.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       System lock type
 * @note        Structure 'nsys_lock' is defined in port sys_lock.h header.
 */
typedef struct nsys_lock nsys_lock;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Initialise port
 */
void nsys_lock_module_init(void);



/**@brief       Terminate port
 */
void nsys_lock_module_term(void);



/**@brief       Enter critical code section
 * @param       lock
 *              Interrupt resource lock, pointer to portable type variable which
 *              will hold the interrupt context state during the critical code
 *              section.
 */
void nsys_lock_enter(
    struct nsys_lock *          lock);



/**@brief       Exit critical code section
 * @param       lock
 *              Interrupt resource lock, portable type variable which is holding
 *              a previously saved interrupt context state.
 */
void nsys_lock_exit(
    struct nsys_lock *          lock);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of sys_lock.h
 ******************************************************************************/
#endif /* NEON_PORT_SYS_LOCK_H_ */
