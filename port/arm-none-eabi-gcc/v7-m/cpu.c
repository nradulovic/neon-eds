/*
 * This file is part of NUB Real-Time Kernel.
 *
 * Copyright (C) 2010 - 2014 Nenad Radulovic
 *
 * NUB Real-Time Kernel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NUB Real-Time Kernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with NUB Real-Time Kernel.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 *//***************************************************************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Implementation of ARM Cortex-M3 CPU port.
 * @addtogroup  arm-none-eabi-gcc-v7-m_impl
 *************************************************************************************************************//** @{ */

/*=================================================================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"
#include "arch/cpu_config.h"
#include "arch/cpu.h"
#include "kernel/nthread.h"
#include "kernel/nsched.h"
#include "kernel/nub.h"

/*=================================================================================================  LOCAL MACRO's  ==*/

/**@brief       Exception return value
 * @details     This value must be set to: exception return gets stack from the process stack, thread mode
 */
#define DEF_EXC_RETURN                0xfffffffdul

/*==============================================================================================  LOCAL DATA TYPES  ==*/
/*=====================================================================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**@brief       Exit thread function
 * @details     This function is called when a thread terminates
 */
static void thread_exit(
    void);

/*===============================================================================================  LOCAL VARIABLES  ==*/
/*==============================================================================================  GLOBAL VARIABLES  ==*/
/*====================================================================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void thread_exit(
    void)
{
    nthread_term();

    while (true);
}

/*===========================================================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*============================================================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*
 * - 1-4    get the beginning of MSP stack from SCB->vtor register
 * - 5      enable interrupts so we can call SVC 0
 * - 8      initiate SVC 0 which switches the context to the first thread
 * - 10     infinite loop: will never reach here
 */
PORT_C_NORETURN void ncpu_dispatch_to_first(
    void)
{
    __asm__ __volatile__ (
        "   ldr     r0, =%0                                 \n"               /* (1)                                                      */
        "   ldr     r0, [r0]                                \n"               /* (2)                                                      */
        "   ldr     r0, [r0]                                \n"               /* (3)                                                      */
        "   msr     msp, r0                                 \n"               /* (4)                                                      */
        "   mov     r0, #0                                  \n"               /* (5)                                                      */
        "   msr     basepri, r0                             \n"               /* (6)                                                      */
        "   cpsie   i                                       \n"               /* (7)                                                      */
        "   svc     0                                       \n"               /* (8)                                                      */
        :
        : "i"(&PORT_SCB->VTOR)
        : "sp", "r0", "memory");

    while (true);
}

void * ncpu_init_ctx(
    void *                      stack,
    size_t                      stack_size,
    void                     (* entry)(void *),
    void *                      arg)
{
    struct nthread_ctx *    sp;

    sp       = (struct nthread_ctx *)((uint8_t *)stack + stack_size);
    sp--;
    sp->xpsr = (ncpu_reg)(PORT_PSR_THUMB_STATE_Msk | PORT_CONFIG_CPU_PSR_DATA);
    sp->pc   = (ncpu_reg)entry;
    sp->lr   = (ncpu_reg)thread_exit;
    sp->r0   = (ncpu_reg)arg;

    return (sp);
}

/*
 * - 2-4    lock the interrupts (by setting higher priority)
 * - 5      get the pointer to new thread ID structure in register r1
 * - 6      get the pointer to process stack
 * - 7      pop the thread stack from PSP stack
 * - 8      save thread top of stack to PSP register
 * - 9      restore previous interrupt priority
 * - 10     Load LR wih @ref DEF_EXC_RETURN value indicating that we want to
 *          return to thread mode and on return use the PSP stack
 */
PORT_C_NAKED void port_svc_isr(
    void)
{
#if (0 != CONFIG_INTR_MAX_PRIO)
    __asm__ __volatile__ (
        "   ldr     r0, =%0                                 \n"               /* (1) Load KernCtrl.cthd address                          */
        "   mov     r1, %2                                  \n"               /* (2)                                                      */
        "   mrs     r3, basepri                             \n"               /* (3)                                                      */
        "   msr     basepri, r1                             \n"               /* (4)                                                      */
        "   ldr     r1, [r0]                                \n"               /* (5)                                                      */
        "   ldr     r1, [r1]                                \n"               /* (6)                                                      */
        "   ldmia   r1!, {r4-r11}                           \n"               /* (7)                                                      */
        "   msr     psp, r1                                 \n"               /* (8)                                                      */
        "   msr     basepri, r3                             \n"               /* (9)                                                      */
        "   mov     lr, %1                                  \n"               /* (10)                                                     */
        "   bx      lr                                      \n"               /* Return to first thread                                   */
        :
        :   "i"(&g_nsched.current),
            "i"(DEF_EXC_RETURN),
            "i"(NINTR_PRIO_TO_CODE(CONFIG_INTR_MAX_PRIO)));
#else
    __asm__ __volatile__ (
        "   ldr     r0, =%0                                 \n"               /* (1) Load KernCtrl.cthd address                          */
        "   cpsid   i                                       \n"               /* (2)                                                      */
        "   ldr     r1, [r0]                                \n"               /* (5)                                                      */
        "   ldr     r1, [r1]                                \n"               /* (6)                                                      */
        "   ldmia   r1!, {r4-r11}                           \n"               /* (7)                                                      */
        "   msr     psp, r1                                 \n"               /* (8)                                                      */
        "   cpsie   i                                       \n"               /* (9)                                                      */
        "   mov     lr, %1                                  \n"               /* (10)                                                     */
        "   bx      lr                                      \n"               /* Return to first thread                                   */
        :
        :   "i"(&g_nsched.current),
            "i"(DEF_EXC_RETURN));
#endif
}

/*
 * - 2-4    lock the interrupts (by setting higher priority)
 * - 5,6    save the current context on PSP stack
 * - 1,7,8  save the thread top of stack into the first member of the thread ID
 *          structure
 * - 9-10   Make g_nsched.cthd == g_nsched.pthd
 * - 11-13  restore new context
 * - 14     restore previous interrupt priority from main stack
 * Note:    LR was already loaded with valid DEF_EXC_RETURN value
 */
PORT_C_NAKED void port_pend_sv(
    void)
{
#if (0 != CONFIG_INTR_MAX_PRIO)
    __asm__ __volatile__ (
        "   ldr     r0, =%0                                 \n"                 /* (1) Get the address of g_nsched                          */
        "   mov     r1, %3                                  \n"                 /* (2)                                                      */
        "   mrs     r3, basepri                             \n"                 /* (3)                                                      */
        "   msr     basepri, r1                             \n"                 /* (4)                                                      */
        "   mrs     r1, psp                                 \n"                 /* (5)                                                      */
        "   stmdb   r1!, {r4-r11}                           \n"                 /* (6)                                                      */
        "   ldr     r2, [r0, %1]                            \n"                 /* (7)                                                      */
        "   str     r1, [r2]                                \n"                 /* (8)                                                      */
        "   ldr     r2, [r0, %2]                            \n"                 /* (9)                                                      */
        "   str     r2, [r0]                                \n"                 /* (10)                                                     */
        "   ldr     r1, [r2]                                \n"                 /* (11)                                                     */
        "   ldmia   r1!, {r4-r11}                           \n"                 /* (12)                                                     */
        "   msr     psp, r1                                 \n"                 /* (13)                                                     */
        "   msr     basepri, r3                             \n"                 /* (14)                                                     */
        "   bx      lr                                      \n"                 /* Return to new thread                                     */
        :
        :   "i"(&g_nsched),
            "J"(offsetof(struct nsched_ctx, current)),
            "J"(offsetof(struct nsched_ctx, pending)),
            "i"(NINTR_PRIO_TO_CODE(CONFIG_INTR_MAX_PRIO)));
#else
    __asm__ __volatile__ (
        "   ldr     r0, =%0                                 \n"               /* (1) Get the address of gCurrentThd                       */
        "   cpsid   i                                       \n"               /* (2)                                                      */
        "   mrs     r1, psp                                 \n"               /* (5)                                                      */
        "   stmdb   r1!, {r4-r11}                           \n"               /* (6)                                                      */
        "   ldr     r2, [r0, %1]                            \n"               /* (7)                                                      */
        "   str     r1, [r2]                                \n"               /* (8)                                                      */
        "   ldr     r2, [r0, %2]                            \n"               /* (9)                                                      */
        "   str     r2, [r0]                                \n"               /* (10)                                                     */
        "   ldr     r1, [r2]                                \n"               /* (11)                                                     */
        "   ldmia   r1!, {r4-r11}                           \n"               /* (12)                                                     */
        "   msr     psp, r1                                 \n"               /* (13)                                                     */
        "   cpsie   i                                       \n"               /* (14)                                                     */
        "   bx      lr                                      \n"               /* Return to new thread                                     */
        :
        :   "i"(&global_nsched),
            "J"(offsetof(esKernCtrl_T, current)),
            "J"(offsetof(esKernCtrl_T, pending)));
#endif
}

void port_sys_tmr(
    void)
{
    nsys_isr_enter_i();
    nsys_timer_isr();
    nsys_isr_exit_i();
}

/*========================================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//**********************************************************************************************
 * END of cpu.c
 **********************************************************************************************************************/
