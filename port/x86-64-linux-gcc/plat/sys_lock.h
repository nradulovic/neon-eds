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
 * @author      Nenad Radulovic
 * @brief       Interface of ARM Cortex critical code section port.
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-critical Critical code section
 * @brief       Critical code section
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_ARCH_CRITICAL_H_
#define ES_ARCH_CRITICAL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <pthread.h>
#include "port/compiler.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nsys_lock
{
    int                         dummy;
};

typedef struct nsys_lock nsys_lock;


/*======================================================  GLOBAL VARIABLES  ==*/

extern pthread_mutex_t g_global_lock;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Enter critical code section
 * @param       resource
 *              Interrupt resource, pointer to portable type variable which will
 *              hold the interrupt context state during the critical code
 *              section.
 */
PORT_C_INLINE
void nsys_lock_enter(
    struct nsys_lock *          lock)
{
    (void)lock;

    pthread_mutex_lock(&g_global_lock);
}



/**@brief       Exit critical code section
 * @param       resource
 *              Interrupt resource, portable type variable which is holding a
 *              previously saved interrupt context state.
 */
PORT_C_INLINE
void nsys_lock_exit(
    struct nsys_lock *          lock)
{
    (void)lock;

    pthread_mutex_unlock(&g_global_lock);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of critical.h
 ******************************************************************************/
#endif /* ES_ARCH_CRITICAL_H_ */
