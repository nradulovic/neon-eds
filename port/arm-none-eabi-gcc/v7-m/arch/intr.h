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
 * @author  	Nenad Radulovic
 * @brief       Interface of ARM Cortex-M3 interrupt port.
 * @addtogroup  arm-none-eabi-gcc-v7-m
 *********************************************************************//** @{ */

#if !defined(INTR_H__)
#define INTR_H__

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/compiler.h"
#include "arch/cpu.h"
#include "arch/sysctrl.h"
#include "arch/intr_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_DEF_MAX_ISR_PRIO                                                   \
    (((PORT_CFG_MAX_ISR_PRIO) << (8u - CPU_DEF_ISR_PRIO_BITS)) & 0xfful)

/**@} *//*----------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_INTR_ENABLE()              portIntrEnable_()

#define PORT_INTR_DISABLE()             portIntrDisable_()

#define PORT_INTR_MASK_SET(mask)        portIntrMaskSet_(mask)

#define PORT_INTR_MASK_GET(mask)        portIntrMaskGet_(mask)

#define PORT_INTR_MASK_REPLACE(oldPrio, newPrio)                                \
    portIntrMaskReplace_(oldPrio, newPrio)

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_INTR_INIT_EARLY()          (void)0                                /**< @brief This port does not need this function call      */

#define PORT_INTR_INIT()                portIntrInit()

#define PORT_INTR_INIT_LATE()           (void)0                                 /**< @brief This port does not need this function call      */

#define PORT_INTR_TERM()                portIntrTerm()

/**@} *//*----------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Interrupt Number Definition
 * @details     Cortex-M3 Processor Exceptions Numbers
 */
enum IntrNum {
    NONMASKABLEINT_IRQN   = -14,                                                /**< @brief 2 Non Maskable Interrupt                        */
    MEMORYMANAGEMENT_IRQN = -12,                                                /**< @brief 4 Cortex-M3 Memory Management Interrupt         */
    BUSFAULT_IRQN         = -11,                                                /**< @brief 5 Cortex-M3 Bus Fault Interrupt                 */
    USAGEFAULT_IRQN       = -10,                                                /**< @brief 6 Cortex-M3 Usage Fault Interrupt               */
    SVCALL_IRQN           = -5,                                                 /**< @brief 11 Cortex-M3 SV Call Interrupt                  */
    PENDSV_IRQN           = -2,                                                 /**< @brief 14 Cortex-M3 Pend SV Interrupt                  */
    SYST_IRQN             = -1                                                  /**< @brief 15 Cortex-M3 System Tick Interrupt              */
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Enable all interrupts
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntrEnable_(
    void) {

    __asm __volatile__ (
        "   cpsie   i                                       \n");
}

/**@brief       Disable all interrupts
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntrDisable_(
    void) {

    __asm __volatile__ (
        "   cpsid   i                                       \n");
}

/**@brief       Set the new interrupt priority state
 * @param       state
 *              New interrupt priority mask or new state of interrupts
 * @note        Depending on @ref PORT_CFG_MAX_ISR_PRIO setting this function will
 *              either set the new priority of allowed interrupts or just
 *              disable/enable all interrupts.
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntrMaskSet_(
    portReg_T           state) {

#if (0 != PORT_CFG_MAX_ISR_PRIO)
    __asm __volatile__ (
        "   msr    basepri, %0                              \n"
        :
        : "r"(state));
#else
    __asm __volatile__ (
        "   msr    primask, %0                              \n"
        :
        : "r"(state));
#endif
}

/**@brief       Get the interrupt priority state
 * @param       state
 *              Pointer to state variable where to store enabled interrupts
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntrMaskGet_(
    portReg_T *         state) {

    portReg_T           tmp;

#if (0 != PORT_CFG_MAX_ISR_PRIO)
    __asm __volatile__ (
        "   mrs     %0, basepri                             \n"
        : "=r"(tmp));
#else
    __asm __volatile__ (
        "   mrs     %0, primask                             \n"
        : "=r"(tmp));
#endif
    *state = tmp;
}

/**@brief       Get current and set new interrupt priority mask
 * @return      Current interrupt priority mask
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntrMaskReplace_(
    portReg_T *         old,
    portReg_T           new) {

    portReg_T           tmp;

#if (0 != PORT_CFG_MAX_ISR_PRIO)
    __asm __volatile__ (
        "   mrs     %0, basepri                             \n"
        "   msr     basepri, %1                             \n"
        : "=r"(tmp)
        : "r"(new));
#else
    __asm __volatile__ (
        "   mrs     %0, primask                             \n"
        "   msr    primask, %1                              \n"
        : "=r"(tmp)
        : "r"(new));
#endif
    *old = tmp;
}

/**@brief       Set Priority for Cortex-M  System Interrupts
 * @param       intrNum
 *              Interrupt number
 * @param       prio
 *              The priority of specified interrupt source
 * @inline
 */
static PORT_C_INLINE_ALWAYS void intrPrioSet_(
    enum IntrNum        intrNum,
    portReg_T           prio) {

    SYS_SCB->shp[((uint32_t)(intrNum) & 0x0ful) - 4u] = prio;
}

/** @} *//*---------------------------------------------------------------*//**
 * @name        Generic port functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize port
 * @details     Function will set up sub-priority bits to zero and handlers
 *              interrupt priority.
 */
void portIntrInit(
    void);

/**@brief       Terminate port
 */
void portIntrTerm(
    void);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of int.h
 ******************************************************************************/
#endif /* INTR_H__ */
