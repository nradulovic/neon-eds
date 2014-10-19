/*
 * This file is part of Neon RT Kernel.
 *
 * Copyright (C) 2010 - 2014 Nenad Radulovic
 *
 * Neon RT Kernel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon RT Kernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon RT Kernel.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Port core module header
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-v7-m-core ARM Cortex M3/M4 Core module
 * @brief       Port core module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ARCH_SYSTIMER_H
#define ARCH_SYSTIMER_H

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>

#include "plat/compiler.h"
#include "arch/port_config.h"
#include "arch/cpu.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Core timer macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Core timer one tick value
 */
#define NSYSTIMER_ONE_TICK                                                    \
    (CONFIG_SYSTIMER_CLOCK_FREQ / CONFIG_SYSTIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the core timer
 */
#define NSYSTIMER_MAX_TICKS                                                   \
    (NPROFILE_MAX_SYSTIMER_VAL / NSYSTIMER_ONE_TICK)

/**@} *//*----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Core timer hardware register type.
 */
typedef unsigned int nsystimer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Core timer management
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Initialize and start the system timer
 */
PORT_C_INLINE
void nsystimer_init(
    nsystimer_tick              tick)
{
    ncpu_reg                    cause;
    
    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(tick);
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
}



/**@brief       Stop and terminate the system timer
 */
PORT_C_INLINE
void nsystimer_term(void)
{
    ncpu_reg                    cause;
    
    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
}



/**@brief       Get free counter value
 */
PORT_C_INLINE
nsystimer_tick nsystimer_get_current(void)
{
    return (_CP0_GET_COMPARE() - _CP0_GET_COUNT());
}



/**@brief       Get reload counter value
 */
PORT_C_INLINE
nsystimer_tick nsystimer_get_reload(void)
{
    return (_CP0_GET_COMPARE());
}



/**@brief       Load the system timer Reload value register
 */
PORT_C_INLINE
void nsystimer_load(
    nsystimer_tick              val)
{
    ncpu_reg                    cause;

    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(tick);
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
}



/**@brief       Enable the system timer
 */
PORT_C_INLINE
void nsystimer_enable(void)
{
    ncpu_reg                    cause;

    cause  = _CP0_GET_CAUSE();
    cause &= ~_CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
}



/**@brief       Disable the system timer
 */
PORT_C_INLINE
void nsystimer_disable(void)
{
    ncpu_reg                    cause;

    cause  = _CP0_GET_CAUSE();
    cause |= _CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
}



/**@brief       Disable the system timer interrupt
 */
PORT_C_INLINE
void nsystimer_isr_enable(void)
{
    IFS0CLR = _IFS0_CTIF_MASK;
    IEC0SET = _IEC0_CTIE_MASK;
}




/**@brief       Enable the system timer interrupt
 */
PORT_C_INLINE
void nsystimer_isr_disable(void)
{
    IEC0CLR = _IEC0_CTIE_MASK;
}


/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port functions
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Initialize port
 */
void nsystimer_module_init(void);



/**@brief       Terminate port
 */
void nsystimer_module_term(void);



/**@brief       User System Timer ISR
 */
extern void nsystimer_isr(void);


/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* ARCH_SYSTIMER_H */
