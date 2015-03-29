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
 * @brief       Core portable layer
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "base/port/core.h"

#if (CONFIG_CORE_TIMER_SOURCE != 0)
# include "stm32f4xx.h"
#endif

/*=========================================================  LOCAL MACRO's  ==*/

#define PORT_CONFIG_ISR_SUBPRIORITY     0

/*--  SysTick Control / Status Register Definitions  -------------------------*/
#define SYST                            ((volatile struct syst_regs *)0xE000E010)
#define SYST_CSR_CLKSOURCE              (1ul << 2)
#define SYST_CSR_TICKINT                (1ul << 1)
#define SYST_CSR_ENABLE                 (1ul << 0)

#define SCB_ICSR                        (*(volatile unsigned int *)0xE000ED04)
#define SCB_ICSR_PENDSTSET              (1ul << 26)
#define SCB_ICSR_PENDSTCLR              (1ul << 25)
#define SCB_SHP_SYSTICK                 (*(volatile unsigned char *)0xE000ED23)

/*--  Timer Control / Status Register Definitions  ---------------------------*/
#if (CONFIG_CORE_TIMER_SOURCE == 2)
#define TIMER 							TIM2
#define TIMER_RST						RCC->APB1RSTR
#define TIMER_RST_BIT					RCC_APB1RSTR_TIM2RST
#define TIMER_CLK						RCC->APB1ENR
#define TIMER_CLK_BIT					RCC_APB1ENR_TIM2EN
#define TIMER_IRQN						TIM2_IRQn
#elif (CONFIG_CORE_TIMER_SOURCE == 3)
#define TIMER 							TIM3
#define TIMER_RST						RCC->APB1RSTR
#define TIMER_RST_BIT					RCC_APB1RSTR_TIM3RST
#define TIMER_CLK						RCC->APB1ENR
#define TIMER_CLK_BIT					RCC_APB1ENR_TIM3EN
#define TIMER_IRQN						TIM3_IRQn
#elif (CONFIG_CORE_TIMER_SOURCE == 4)
#define TIMER 							TIM4
#define TIMER_RST						RCC->APB1RSTR
#define TIMER_RST_BIT					RCC_APB1RSTR_TIM4RST
#define TIMER_CLK						RCC->APB1ENR
#define TIMER_CLK_BIT					RCC_APB1ENR_TIM4EN
#define TIMER_IRQN						TIM4_IRQn
#elif (CONFIG_CORE_TIMER_SOURCE == 5)
#define TIMER 							TIM5
#define TIMER_RST						RCC->APB1RSTR
#define TIMER_RST_BIT					RCC_APB1RSTR_TIM5RST
#define TIMER_CLK						RCC->APB1ENR
#define TIMER_CLK_BIT					RCC_APB1ENR_TIM5EN
#define TIMER_IRQN						TIM5_IRQn
#endif

#define TIMER_CR1_CEN					(0x1u << 0)

/*--  On AIRCR register writes, write 0x5FA, otherwise the write is ignored  -*/
#define SCB_AIRCR                       (*(volatile unsigned int *)0xE000ED0C)
#define SCB_AIRCR_VECTKEY_Pos           16
#define SCB_AIRCR_VECTKEY               (0xfffful << 16)
#define SCB_AIRCR_VECTKEY_VALUE         (0x5faul << 16)
#define SCB_AIRCR_PRIGROUP_Pos          8
#define SCB_AIRCR_PRIGROUP              (0x7ul << 8)

/*======================================================  LOCAL DATA TYPES  ==*/

struct syst_regs
{
    volatile unsigned int       csr;
    volatile unsigned int       rvr;
    volatile unsigned int       cvr;
    volatile unsigned int       calib;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/


PORT_C_INLINE
void cpu_init(void);



/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void isr_global_enable(void);



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void isr_global_disable(void);



static void lock_init(void);



static void lock_term(void);



static void timer_init(void);



static void timer_term(void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


PORT_C_INLINE
void cpu_init(void)
{
    __asm__ __volatile__(
        "@  ncpu_init                                       \n"
        "   clrex                                           \n");
                                              /* Clear the exclusive monitor. */
}



PORT_C_INLINE
void cpu_term(void)
{
    for (;;) {
        __asm__ __volatile__(
            "   wfe                                         \n");
    }
}



PORT_C_INLINE
void isr_global_enable(void)
{
    __asm __volatile__ (
        "@  nisr_enable                                     \n"
        "   cpsie   i                                       \n");
}



PORT_C_INLINE
void isr_global_disable(void)
{
    __asm __volatile__ (
        "@  nisr_disable                                    \n"
        "   cpsid   i                                       \n");
}



static void lock_init(void)
{
    unsigned int                reg;

    isr_global_disable();
    reg  = SCB_AIRCR;
    reg &= ~(SCB_AIRCR_VECTKEY | SCB_AIRCR_PRIGROUP);
    reg |=   SCB_AIRCR_VECTKEY_VALUE;
    reg |=  (PORT_CONFIG_ISR_SUBPRIORITY << SCB_AIRCR_PRIGROUP_Pos);
    SCB_AIRCR = reg;
    isr_global_enable();
}



static void lock_term(void)
{
    isr_global_disable();
}



static void timer_init(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SYST->csr &= ~SYST_CSR_ENABLE;
    SYST->csr &= ~SYST_CSR_TICKINT;
    SYST->csr |=  SYST_CSR_CLKSOURCE;
    SYST->rvr  =  CONFIG_CORE_TIMER_CLOCK_FREQ / CONFIG_CORE_TIMER_EVENT_FREQ;
    SYST->csr  =  0;
    SCB_ICSR  |=  SCB_ICSR_PENDSTCLR;
    SCB_SHP_SYSTICK = (uint8_t)NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL);
#else
    volatile unsigned int 		dummy;

    TIMER_RST  |=  TIMER_RST_BIT;							/* Reset and enable timer clock */
    TIMER_RST  &= ~TIMER_RST_BIT;
    TIMER_CLK  |=  TIMER_CLK_BIT;
    dummy       =  TIMER_CLK;
    (void)dummy;
    														/* Setup timer */
    TIMER->ARR  = 1000;
    TIMER->CNT  = 0;
    TIMER->DIER = 0;
    TIMER->CR1  = 0;
    TIMER->PSC  = (CONFIG_CORE_TIMER_CLOCK_FREQ / 1000u) / CONFIG_CORE_TIMER_EVENT_FREQ;
    														/* Disable timer clock */
    TIMER_CLK  &= ~TIMER_CLK_BIT;
															/* Enable interrupt */
	NVIC->IP[TIMER_IRQN] = NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL);
	NVIC->ISER[TIMER_IRQN >> 5] = 0x1u << ((TIMER_IRQN & 0x1fu));
#endif
}



/**@brief       Stop and terminate the system timer
 */
static void timer_term(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SYST->csr &= ~SYST_CSR_ENABLE;
#else
															/* Turn off timer */
	TIMER->CR1  &= ~TIM_CR1_CEN;
															/* Disable timer interrupt */
	TIMER->DIER &= ~TIM_DIER_UIE;
															/* Disable timer clock */
	TIMER_CLK   &= ~TIMER_CLK_BIT;
    														/* Disable interrupt */
    NVIC->ICER[TIMER_IRQN >> 5] = 0x1u << ((TIMER_IRQN & 0x1fu));
#endif
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


/**@brief       Enable the system timer
 */
void ncore_timer_enable(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SYST->csr |=  (SYST_CSR_ENABLE | SYST_CSR_TICKINT);
#else
    volatile unsigned int 		dummy;
    														/* Enable timer clock */
    TIMER_CLK   |= TIMER_CLK_BIT;
	dummy        = TIMER_CLK;
	(void)dummy;
    														/* Enable timer interrupt */
    TIMER->DIER |= TIM_DIER_UIE;
															/* Turn on timer */
    TIMER->CR1  |= TIM_CR1_CEN;
#endif
}



/**@brief       Disable the system timer
 */
void ncore_timer_disable(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SYST->csr &= ~(SYST_CSR_ENABLE | SYST_CSR_TICKINT);
#else
    														/* Turn off timer */
    TIMER->CR1  &= ~TIM_CR1_CEN;
															/* Disable timer interrupt */
    TIMER->DIER &= ~TIM_DIER_UIE;
    														/* Disable timer clock */
    TIMER_CLK   &= ~TIMER_CLK_BIT;
#endif
}



void nmodule_core_init(void)
{
    cpu_init();
    lock_init();
    timer_init();
}



void nmodule_core_term(void)
{
    timer_term();
    lock_term();
    cpu_term();
}


#if (CONFIG_CORE_TIMER_SOURCE == 0)
void SysTick_Handler(void);

void SysTick_Handler(void)
{
	ncore_timer_isr();
}
#elif (CONFIG_CORE_TIMER_SOURCE == 2)
void TIM2_IRQHandler(void);

void TIM2_IRQHandler(void)
{
	if (TIMER->SR &   TIM_SR_UIF) {
		TIMER->SR &= ~TIM_SR_UIF;
		ncore_timer_isr();
	}
}
#elif (CONFIG_CORE_TIMER_SOURCE == 3)
void TIM3_IRQHandler(void);

void TIM3_IRQHandler(void)
{
	if (TIMER->SR &   TIM_SR_UIF) {
		TIMER->SR &= ~TIM_SR_UIF;
		ncore_timer_isr();
	}
}
#elif (CONFIG_CORE_TIMER_SOURCE == 4)
void TIM4_IRQHandler(void);

void TIM4_IRQHandler(void)
{
	if (TIMER->SR &   TIM_SR_UIF) {
		TIMER->SR &= ~TIM_SR_UIF;
		ncore_timer_isr();
	}
}
#elif (CONFIG_CORE_TIMER_SOURCE == 5)
void TIM5_IRQHandler(void);

void TIM5_IRQHandler(void)
{
	if (TIMER->SR &   TIM_SR_UIF) {
		TIMER->SR &= ~TIM_SR_UIF;
		ncore_timer_isr();
	}
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if ((CONFIG_CORE_TIMER_SOURCE != 0) && (CONFIG_CORE_TIMER_SOURCE != 2) && \
	 (CONFIG_CORE_TIMER_SOURCE != 3) && (CONFIG_CORE_TIMER_SOURCE != 4) && \
	 (CONFIG_CORE_TIMER_SOURCE != 5))
# error "NEON::base::port::stm32f4xx-none-gcc: The selected timer in CONFIG_CORE_TIMER_SOURCE is not supported!"
#endif

/** @endcond *//** @} *//******************************************************
 * END of p_sys_lock.c
 ******************************************************************************/
