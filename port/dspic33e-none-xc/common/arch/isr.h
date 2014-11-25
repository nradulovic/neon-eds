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
    (((prio) << _CP0_STATUS_IPL_POSITION) & _CP0_STATUS_IPL_MASK)

#define NISR_CODE_TO_PRIO(code)                                                 \
    (((code) & _CP0_STATUS_IPL_MASK) >> _CP0_STATUS_IPL_POSITION)

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
void __attribute__ ((nomips16)) nisr_enable(void)
{
    __asm __volatile__(
        "   ei                                              \n");
}



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void __attribute__ ((nomips16)) nisr_disable(void)
{
    __asm __volatile__(
        "   di                                              \n");
}



/**@brief       Set the new interrupt priority state
 * @param       new_mask
 *              New interrupt priority mask or new state of interrupts
 * @note        Depending on @ref CONFIG_ISR_MAX_PRIO setting this function will
 *              either set the new priority of allowed interrupts or just
 *              disable/enable all interrupts.
 */
PORT_C_INLINE
void __attribute__ ((nomips16)) nisr_set_mask(
    nisr_ctx                    new_mask)
{
#if (CONFIG_ISR_MAX_PRIO == 0)
    
    if (new_mask == 0u) {
        nisr_disable();
    } else {
        nisr_enable();
    }
#else
    ncpu_reg                    isr_status;
    
    isr_status  =  _CP0_GET_STATUS();
    isr_status &= ~_CP0_STATUS_IPL_MASK;
    isr_status |= new_mask & _CP0_STATUS_IPL_MASK;
    _CP0_SET_STATUS(isr_status);
#endif
}



/**@brief       Get old and set new interrupt priority mask
 * @return      Current interrupt priority mask
 */
PORT_C_INLINE
nisr_ctx __attribute__ ((nomips16)) nisr_replace_mask(
    nisr_ctx                    new_mask)
{
#if (CONFIG_ISR_MAX_PRIO != 0)
    nisr_ctx                    old_mask;
    
    if (new_mask == 0u) {
        __asm __volatile__(
            "   di      %0                                  \n"
            : "=r"(old_mask));
    } else {
        __asm __volatile__(
            "   ei      %0                                  \n"
            : "=r"(old_mask));
    }
    
    return (old_mask);
#else
    nisr_ctx                    old_mask;
    ncpu_reg                    isr_status;
    
    isr_status  = _CP0_GET_STATUS();
    old_mask    = isr_status;
    isr_status &= ~_CP0_STATUS_IPL_MASK;
    _CP0_SET_STATUS(isr_status | new_mask);
    
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
