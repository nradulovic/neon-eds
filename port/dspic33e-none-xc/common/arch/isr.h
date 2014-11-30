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

#ifndef ARCH_ISR_H
#define ARCH_ISR_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <xc.h>

#include "plat/compiler.h"
#include "arch/port_config.h"
#include "arch/cpu.h"

/*===============================================================  MACRO's  ==*/

#define NISR_PRIO_TO_CODE(prio)                                                 \
    (((prio) << _SR_IPL_POSITION) & _SR_IPL_MASK)

#define NISR_CODE_TO_PRIO(code)                                                 \
    (((code) & _SR_IPL_POSITION) >> _SR_IPL_POSITION)

#define nisr_exit()                         (void)0

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Interrupt context type
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
typedef unsigned int nisr_ctx;

/*======================================================  GLOBAL VARIABLES  ==*/

extern bool g_isr_is_active;

/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void nisr_enable(void)
{
    INTCON2bits.GIE = 1;
}



/**@brief       Disable all interrupts
 * @details     Enabling/disabling interrupts on 16-bit PIC series seems to be
 *              quite problematic. For further discusion see:
 *              http://www.microchip.com/forums/tm.aspx?m=458828&mpage=1&key=&#461267
 */
PORT_C_INLINE
void nisr_disable(void)
{
    while (INTCON2bits.GIE == 1) {
        INTCON2bits.GIE = 0;
    }
}



/**@brief       Set the new interrupt priority state
 * @param       new_mask
 *              New interrupt priority mask or new state of interrupts
 * @note        Depending on @ref CONFIG_ISR_MAX_PRIO setting this function will
 *              either set the new priority of allowed interrupts or just
 *              disable/enable all interrupts.
 */
PORT_C_INLINE
void nisr_set_mask(
    nisr_ctx                    new_mask)
{
#if (CONFIG_ISR_MAX_PRIO == 0)
    
    if (new_mask == 0u) {
        nisr_disable();
    } else {
        nisr_enable();
    }
#else
    SET_CPU_IPL(new_mask);
#endif
}



/**@brief       Get old and set new interrupt priority mask
 * @return      Current interrupt priority mask
 */
PORT_C_INLINE
nisr_ctx nisr_replace_mask(
    nisr_ctx                    new_mask)
{
#if (CONFIG_ISR_MAX_PRIO == 0)
    nisr_ctx                    old_mask;

    old_mask = INTCON2bits.GIE;
    
    if (new_mask == 0u) {
        nisr_disable();
    } else {
        nisr_enable();
    }
    
    return (old_mask);
#else
    nisr_ctx                    old_mask;
    
    SET_AND_SAVE_CPU_IPL(old_mask, new_mask);
    
    return (old_mask);
#endif
}



PORT_C_INLINE
void nisr_enter(void)
{
    g_isr_is_active = true;
}



PORT_C_INLINE
void nisr_pend_kernel(void)
{
    /* TODO: Set the pending kernel ISR here
     */
}



PORT_C_INLINE
bool nisr_is_active(void)
{
    return (g_isr_is_active);
}



/**@brief       Initialize ISR module
 */
void nisr_module_init(void);



/**@brief       Terminate ISR module
 */
void nisr_module_term(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of nport_core.h
 ******************************************************************************/
#endif /* ARCH_ISR_H */
