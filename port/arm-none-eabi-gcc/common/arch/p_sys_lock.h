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
 * @brief       Port system lock
 *********************************************************************//** @{ */

#ifndef NEON_ARCH_P_SYS_LOCK_H_
#define NEON_ARCH_P_SYS_LOCK_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "port/compiler.h"
#include "port/profile.h"
#include "shared/config.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Interrupt context structure
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
struct nsys_lock
{
    unsigned int                level;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/



PORT_C_INLINE
void nsys_lock_enter(
    struct nsys_lock *          lock)
{
#if (CONFIG_SYS_LOCK_MAX_LEVEL != 0)
    unsigned int                new_mask;

    new_mask = (((CONFIG_SYS_LOCK_MAX_LEVEL) << (8u - PORT_ISR_PRIO_BITS)) & 0xfful);

    __asm __volatile__ (
        "@  nsys_lock_enter                                 \n"
        "   mrs     %0, basepri                             \n"
        "   msr     basepri, %1                             \n"
        : "=&r"(lock->level)
        : "r"(new_mask));
#else
    unsigned int                new_mask;

    new_mask = 1;

    __asm __volatile__ (
        "@  nsys_lock_enter                                 \n"
        "   mrs     %0, primask                             \n"
        "   msr    primask, %1                              \n"
        : "=&r"(lock->level)
        : "r"(new_mask));
#endif
}



PORT_C_INLINE
void nsys_lock_exit(
    struct nsys_lock *          lock)
{
#if (CONFIG_SYS_LOCK_MAX_LEVEL != 0)
    __asm __volatile__ (
        "@  nsys_lock_exit                                  \n"
        "   msr    basepri, %0                              \n"
        :
        : "r"(lock->level));
#else
    __asm __volatile__ (
        "@  nsys_lock_exit                                  \n"
        "   msr    primask, %0                              \n"
        :
        : "r"(lock->level));
#endif
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of p_sys_lock.h
 ******************************************************************************/
#endif /* NEON_ARCH_P_SYS_LOCK_H_ */
