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
#include "arch/config.h"
#include "arch/cpu.h"
#include "device/mcu_profile.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Core timer one tick value
 */
#define NSYSTIMER_ONE_TICK(clock)                                               \
    (((clock) + (CONFIG_SYSTIMER_EVENT_FREQ / 2)) / CONFIG_SYSTIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the core timer
 */
#define NSYSTIMER_MAX_TICKS                                                     \
    (UINT32_MAX / NSYSTIMER_ONE_TICK)

#define NSYSTIMER_TICK_MAX              UINT32_MAX

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Core timer hardware register type.
 */
typedef uint32_t nsystimer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Initialize and start the system timer
 */
PORT_C_INLINE
void nsystimer_init(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    T2CON         = 0;
    T3CON         = 0;
    T2CONbits.T32 = 1;
    TMR2          = 0;
#endif
}



/**@brief       Stop and terminate the system timer
 */
PORT_C_INLINE
void nsystimer_term(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    T2CONbits.TON = 0;
#endif
}



/**@brief       Get free counter value
 */
PORT_C_INLINE
nsystimer_tick nsystimer_get_current(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    return (TMR2);
#endif
}



/**@brief       Get reload counter value
 */
PORT_C_INLINE
nsystimer_tick nsystimer_get_reload(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    return (PR2);
#endif
}



/**@brief       Load the system timer Reload value register
 */
PORT_C_INLINE
void nsystimer_load(
    nsystimer_tick              tick)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    T2CONbits.TON = 0;
    TMR2 = 0;
    PR2  = tick;
    T2CONbits.TON = 1;
#endif
}



/**@brief       Enable the system timer
 */
PORT_C_INLINE
void nsystimer_enable(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    T2CONbits.TON = 1;
#endif
}



/**@brief       Disable the system timer
 */
PORT_C_INLINE
void nsystimer_disable(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    T2CONbits.TON = 0;
#endif
}



/**@brief       Disable the system timer interrupt
 */
PORT_C_INLINE
void nsystimer_isr_enable(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
#endif
}




/**@brief       Enable the system timer interrupt
 */
PORT_C_INLINE
void nsystimer_isr_disable(void)
{
#if (PORT_CONFIG_SYSTIMER_SELECTION == 2)
    IEC0bits.T2IE = 0;
#endif
}



/**@brief       Initialize port
 */
void nsystimer_module_init(void);



/**@brief       Terminate port
 */
void nsystimer_module_term(void);



/**@brief       User System Timer ISR
 */
extern void nsystimer_isr(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* ARCH_SYSTIMER_H */
