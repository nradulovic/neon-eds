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
 * @brief       Interface of ARM Cortex-M3 System Control port.
 * @addtogroup  arm-none-eabi-gcc-v7-m
 *********************************************************************//** @{ */

#if !defined(SYSCTRL_H__)
#define SYSCTRL_H__

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "arch/compiler.h"
#include "arch/cpu.h"
#include "arch/sysctrl_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       System timer one tick value
 */
#define PORT_DEF_SYSTMR_ONE_TICK                                                \
    (PORT_CFG_SYSTMR_CLOCK_FREQ / CFG_SYSTMR_EVENT_FREQUENCY)

/**@brief       Maximum number of ticks without overflowing the system timer
 */
#define PORT_DEF_SYSTMR_MAX_TICKS                                               \
    (PORT_DEF_SYSTMR_MAX_VAL / PORT_DEF_SYSTMR_ONE_TICK)

/**@brief       On AIRCR register writes, write 0x5FA, otherwise the write is
 *              ignored
 */
#define SYS_SCB_AIRCR_VECTKEY_VALUE     0x5faul

/**@} *//*----------------------------------------------------------------*//**
 * @name        System timer management
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_SYSTMR_INIT(val)           portSysTmrInit_(val)

#define PORT_SYSTMR_TERM()              portSysTmrTerm_()

#define PORT_SYSTMR_GET_RVAL()          portSysTmrGetRVal_()

#define PORT_SYSTMR_GET_CVAL()          portSysTmrGetCVal_()

#define PORT_SYSTMR_RLD(val)            portSysTmrRld_(val)

#define PORT_SYSTMR_ENABLE()            portSysTmrEnable_()

#define PORT_SYSTMR_DISABLE()           portSysTmrDisable_()

#define PORT_SYSTMR_ISR_ENABLE()        portSysTmrIsrEnable_()

#define PORT_SYSTMR_ISR_DISABLE()       portSysTmrIsrDisable_()

/** @} *//*---------------------------------------------------------------*//**
 * @name        System control
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       System Control Space Base Address.
 */
#define SYS_SCS_BASE                    (0xe000e000ul)

/**@brief       System Control Block Base Address.
 */
#define SYS_SCB_BASE                    (SYS_SCS_BASE + 0x0d00ul)

/**@brief       System Control Block
 */
#define SYS_SCB                                                                 \
    ((volatile struct sysSCB_ *)(SYS_SCB_BASE))

/**@brief       SCB icsr: PENDSVSET Position.
 */
#define SYS_SCB_ICSR_PENDSVSET_Pos      (28u)

/**@brief       SCB icsr: PENDSVSET Mask.
 */
#define SYS_SCB_ICSR_PENDSVSET_Msk      (0x01ul << SYS_SCB_ICSR_PENDSVSET_Pos)

/**@brief       SCB icsr: PENDSTCLR Position.
 */
#define SYS_SCB_ICSR_PENDSTCLR_Pos      (25u)

/**@brief       SCB icsr: PENDSTCLR Mask.
 */
#define SYS_SCB_ICSR_PENDSTCLR_Msk      (0x01ul << SYS_SCB_ICSR_PENDSTCLR_Pos)

/**@brief       SCB icsr: RETTOBASE Position.
 */
#define SYS_SCB_ICSR_RETTOBASE_Pos      (11u)

/**@brief       SCB icsr: RETTOBASE Mask.
 */
#define SYS_SCB_ICSR_RETTOBASE_Msk      (0x01ul << SYS_SCB_ICSR_RETTOBASE_Pos)

/**@brief       SCB aircr: PRIGROUP Position
 */
#define SYS_SCB_AIRCR_PRIGROUP_Pos      (8ul)

/**@brief       SCB aircr: PRIGROUP Mask
 */
#define SYS_SCB_AIRCR_PRIGROUP_Msk      (7ul << SYS_SCB_AIRCR_PRIGROUP_Pos)

/**@brief       SCB aircr: VECTKEY Position
 */
#define SYS_SCB_AIRCR_VECTKEY_Pos       (16ul)

/**@brief       SCB aircr: VECTKEY Mask
 */
#define SYS_SCB_AIRCR_VECTKEY_Msk       (0xfffful << SYS_SCB_AIRCR_VECTKEY_Pos)

/**@brief       System Timer Base Address.
 */
#define SYS_SYST_BASE                   (SYS_SCS_BASE + 0x0010ul)

/**@brief       System Timer
 */
#define SYS_SYST                                                                \
    ((volatile struct sysSYST_ *)SYS_SYST_BASE)

/**@brief       SYSTMR csr: CLKSOURCE Position.
 */
#define SYS_SYST_CSR_CLKSOURCE_Pos      (2u)

/**@brief       SYSTMR csr: CLKSOURCE Mask.
 */
#define SYS_SYST_CSR_CLKSOURCE_Msk      (0x01ul << SYS_SYST_CSR_CLKSOURCE_Pos)

/**@brief       SYSTMR csr: TICKINT Position.
 */
#define SYS_SYST_CSR_TICKINT_Pos        (1u)

/**@brief       SYSTMR csr: TICKINT Mask.
 */
#define SYS_SYST_CSR_TICKINT_Msk        (0x01ul << SYS_SYST_CSR_TICKINT_Pos)

/**@brief       SYSTMR csr: ENABLE Position.
 */
#define SYS_SYST_CSR_ENABLE_Pos         (0u)

/**@brief       SYSTMR csr: ENABLE Mask.
 */
#define SYS_SYST_CSR_ENABLE_Msk         (0x01ul << SYS_SYST_CSR_ENABLE_Pos)

/** @} *//*---------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       System timer hardware register type.
 */
typedef uint32_t portSysTmrReg_T;

/**@brief       Structure type to access the System Control Block (SCB).
 */
struct sysSCB_ {
    uint32_t            cpuid;                                                  /**<@brief  CPUID Base Register                             */
    uint32_t            icsr;                                                   /**<@brief  Interrupt Control and State Register            */
    uint32_t            vtor;                                                   /**<@brief  Vector Table Offset Register                    */
    uint32_t            aircr;                                                  /**<@brief  Application Interrupt and Reset Control Register*/
    uint32_t            scr;                                                    /**<@brief  System Control Register                         */
    uint32_t            ccr;                                                    /**<@brief  Configuration Control Register                  */
    uint8_t             shp[12];                                                /**<@brief  System Handlers Priority Registers              */
    uint32_t            shcsr;                                                  /**<@brief  System Handler Control and State Register       */
    uint32_t            cfsr;                                                   /**<@brief  Configurable Fault Status Register              */
    uint32_t            hfsr;                                                   /**<@brief  HardFault Status Register                       */
    uint32_t            dfsr;                                                   /**<@brief  Debug Fault Status Register                     */
    uint32_t            mmfar;                                                  /**<@brief  MemManage Fault Address Register                */
    uint32_t            bfar;                                                   /**<@brief  BusFault Address Register                       */
    uint32_t            afsr;                                                   /**<@brief  Auxiliary Fault Status Register                 */
    uint32_t            pfr[2];                                                 /**<@brief  Processor Feature Register                      */
    uint32_t            dfr;                                                    /**<@brief  Debug Feature Register                          */
    uint32_t            adr;                                                    /**<@brief  Auxiliary Feature Register                      */
    uint32_t            mmfr[4];                                                /**<@brief  Memory Model Feature Register                   */
    uint32_t            isar[5];                                                /**<@brief  Instruction Set Attributes Register             */
    uint32_t            RESERVED0[5];                                           /**<@brief  Reserved                                        */
    uint32_t            cpacr;                                                  /**<@brief  Coprocessor Access Control Register             */
};

struct sysSYST_ {
    uint32_t            csr;                                                    /**<@brief  SysTick Control and Status Register             */
    uint32_t            rvr;                                                    /**<@brief  SysTick Reload Value Register                   */
    uint32_t            cvr;                                                    /**<@brief  SysTick Current Value Register                  */
    uint32_t            calib;                                                  /**<@brief  SysTick Calibration value Register              */
    uint32_t            RESERVED0[55];                                          /**<@brief  Reserved                                        */
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        System timer management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize and start the system timer
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrInit_(
    portSysTmrReg_T     val) {

    SYS_SYST->csr &= ~SYS_SYST_CSR_ENABLE_Msk;                                  /* Disable SysTick Timer                                    */
    SYS_SYST->rvr = val - 1u;                                                   /* Set SysTick reload register                              */
    SYS_SYST->cvr = 0u;
    SYS_SYST->csr = SYS_SYST_CSR_CLKSOURCE_Msk;                                 /* SysTick uses the processor clock.                        */
}

/**@brief       Get free counter value
 * @inline
 */
static PORT_C_INLINE_ALWAYS portSysTmrReg_T portSysTmrGetCVal_(
    void) {

    return (SYS_SYST->cvr);
}

/**@brief       Get reload counter value
 * @inline
 */
static PORT_C_INLINE_ALWAYS portSysTmrReg_T portSysTmrGetRVal_(
    void) {

    return (SYS_SYST->rvr);
}

/**@brief       Load the system timer Reload value register
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrRld_(
    portSysTmrReg_T val) {

    --val;
    SYS_SYST->csr &= ~SYS_SYST_CSR_ENABLE_Msk;
    SYS_SYST->rvr = val;
    SYS_SYST->cvr = 0u;
    SYS_SYST->csr |= SYS_SYST_CSR_ENABLE_Msk;
}

/**@brief       Enable the system timer
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrEnable_(
    void) {

    SYS_SYST->csr |= SYS_SYST_CSR_ENABLE_Msk;
}

/**@brief       Disable the system timer
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrDisable_(
    void) {

    SYS_SYST->csr &= ~SYS_SYST_CSR_ENABLE_Msk;
}

/**@brief       Disable the system timer interrupt
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrIsrEnable_(
    void) {

    SYS_SYST->csr |= SYS_SYST_CSR_TICKINT_Msk;
}

/**@brief       Enable the system timer interrupt
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrIsrDisable_(
    void) {

    SYS_SCB->icsr |= SYS_SCB_ICSR_PENDSTCLR_Msk;
    SYS_SYST->csr &= ~SYS_SYST_CSR_TICKINT_Msk;
}

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of systmr.h
 ******************************************************************************/
#endif /* SYSCTRL_H__ */
