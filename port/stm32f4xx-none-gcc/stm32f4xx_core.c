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
 * @brief       STM32F4xx port core source
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "port/core.h"
#include "stm32f4xx.h"

/*=========================================================  LOCAL MACRO's  ==*/

/*--  Timer Control / Status Register Definitions  ---------------------------*/
#if (CONFIG_CORE_TIMER_SOURCE == 2)
#define TIMER                           TIM2
#define TIMER_RST                       RCC->APB1RSTR
#define TIMER_RST_BIT                   RCC_APB1RSTR_TIM2RST
#define TIMER_CLK                       RCC->APB1ENR
#define TIMER_CLK_BIT                   RCC_APB1ENR_TIM2EN
#define TIMER_IRQN                      TIM2_IRQn
#define TIMER_HANDLER                   TIM2_IRQHandler
#elif (CONFIG_CORE_TIMER_SOURCE == 3)
#define TIMER                           TIM3
#define TIMER_RST                       RCC->APB1RSTR
#define TIMER_RST_BIT                   RCC_APB1RSTR_TIM3RST
#define TIMER_CLK                       RCC->APB1ENR
#define TIMER_CLK_BIT                   RCC_APB1ENR_TIM3EN
#define TIMER_IRQN                      TIM3_IRQn
#define TIMER_HANDLER                   TIM3_IRQHandler
#elif (CONFIG_CORE_TIMER_SOURCE == 4)
#define TIMER                           TIM4
#define TIMER_RST                       RCC->APB1RSTR
#define TIMER_RST_BIT                   RCC_APB1RSTR_TIM4RST
#define TIMER_CLK                       RCC->APB1ENR
#define TIMER_CLK_BIT                   RCC_APB1ENR_TIM4EN
#define TIMER_IRQN                      TIM4_IRQn
#define TIMER_HANDLER                   TIM4_IRQHandler
#elif (CONFIG_CORE_TIMER_SOURCE == 5)
#define TIMER                           TIM5
#define TIMER_RST                       RCC->APB1RSTR
#define TIMER_RST_BIT                   RCC_APB1RSTR_TIM5RST
#define TIMER_CLK                       RCC->APB1ENR
#define TIMER_CLK_BIT                   RCC_APB1ENR_TIM5EN
#define TIMER_IRQN                      TIM5_IRQn
#define TIMER_HANDLER                   TIM5_IRQHandler
#endif

/*======================================================  LOCAL DATA TYPES  ==*/
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



extern void PendSV_Handler(void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

bool                        g_core_is_locked;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


PORT_C_INLINE
void cpu_init(void)
{
                                              /* Clear the exclusive monitor. */
    __asm__ __volatile__(
        "@  cpu_init                                        \n"
        "   clrex                                           \n");
}



PORT_C_INLINE
void cpu_sleep(void)
{
    __asm__ __volatile__(
        "@  cpu_sleep                                       \n"
        "   wfe                                             \n");
}



PORT_C_INLINE
void cpu_term(void)
{
    for (;;) {
    /*
     * NOTE: Turn off WaitForEvent when in debug mode. Some debuggers have 
     * trouble working when WFE is enabled.
     */
#if !defined(NDEBUG) && (CONFIG_DEBUG == 0)
        cpu_sleep();
#endif
    }
}



PORT_C_INLINE
void isr_global_enable(void)
{
    __asm __volatile__ (
        "@  isr_global_enable                               \n"
        "   cpsie   i                                       \n");
}



PORT_C_INLINE
void isr_global_disable(void)
{
    __asm __volatile__ (
        "@  isr_global_disable                              \n"
        "   cpsid   i                                       \n");
}



static void lock_init(void)
{
    isr_global_disable();
    NVIC_SetPriorityGrouping(NCORE_LOCK_LEVEL_BITS - 1);
    isr_global_enable();
}



static void lock_term(void)
{
    isr_global_disable();
}



static void timer_init(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SysTick->CTRL = 0;
    SysTick->LOAD =
        CONFIG_CORE_TIMER_CLOCK_FREQ / CONFIG_CORE_TIMER_EVENT_FREQ - 1;
    SysTick->VAL  = 0;
    SCB->ICSR    |= SCB_ICSR_PENDSTCLR_Msk;          /* Clear pending ISR bit */
    NVIC_SetPriority(SysTick_IRQn, NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL));
#else
    volatile unsigned int       dummy;

    TIMER_RST |=  TIMER_RST_BIT;              /* Reset and enable timer clock */
    TIMER_RST &= ~TIMER_RST_BIT;
    TIMER_CLK |=  TIMER_CLK_BIT;                              /* Enable clock */
    dummy      =  TIMER_CLK;          /* Dummy read to generate a small delay */
    (void)dummy;

                                       /* Setup timer at clock frequency 10kHz*/
    TIMER->CR1 = 0;                                         /* Turn off timer */
    TIMER->ARR = 10000u / (CONFIG_CORE_TIMER_EVENT_FREQ) - 1u;
    TIMER->PSC = (CONFIG_CORE_TIMER_CLOCK_FREQ) / 5000u;
    TIMER->EGR = TIM_EGR_UG;
                                                           /* Setup interrupt */
    NVIC_SetPriority(TIMER_IRQN, NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL));
    NVIC_ClearPendingIRQ(TIMER_IRQN);
    NVIC_EnableIRQ(TIMER_IRQN);

    TIMER_CLK  &= ~TIMER_CLK_BIT;                      /* Disable timer clock */
#endif
}



static void timer_term(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SysTick->CTRL = 0;
#else

    NVIC_DisableIRQ(TIMER_IRQN);                         /* Disable interrupt */
    TIMER->CR1  &= ~TIM_CR1_CEN;                            /* Turn off timer */
    TIMER->DIER &= ~TIM_DIER_UIE;                  /* Disable timer interrupt */
    TIMER_CLK   &= ~TIMER_CLK_BIT;                     /* Disable timer clock */
#endif
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void ncore_init(void)
{
    cpu_init();
    lock_init();
    timer_init();
}



void ncore_term(void)
{
    timer_term();
    lock_term();
    cpu_term();
}



void ncore_idle(void)
{
    /* NOTE:
     * When in debug mode do not use sleep instruction.
     */
#if !defined(NDEBUG) && (CONFIG_DEBUG == 0)
    cpu_sleep();
#endif
}



void ncore_timer_enable(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
                                                /* Enable interrupt and Timer */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
#else
    volatile unsigned int       dummy;
                                                        /* Enable timer clock */
    TIMER_CLK   |= TIMER_CLK_BIT;
    dummy        = TIMER_CLK;         /* Dummy read to generate a small delay */
    (void)dummy;

    TIMER->DIER |= TIM_DIER_UIE;                    /* Enable timer interrupt */
    TIMER->CR1  |= TIM_CR1_CEN;                              /* Turn on timer */
#endif
}



void ncore_timer_disable(void)
{
#if (CONFIG_CORE_TIMER_SOURCE == 0)
    SysTick->CTRL = 0;
#else
    TIMER->CR1  &= ~TIM_CR1_CEN;                            /* Turn off timer */
    TIMER->DIER &= ~TIM_DIER_UIE;                  /* Disable timer interrupt */
    TIMER_CLK   &= ~TIMER_CLK_BIT;                     /* Disable timer clock */
#endif
}



#if (CONFIG_CORE_TIMER_SOURCE == 0)
/* NOTE 1:
 * This is the STM32F4xx SysTick ISR
 * The ISR is used only when the SysTick timer is configured as core timer.
 *
 * NOTE 2:
 * When the STM32F4xx HAL is enabled it will use the SysTick Handler by default.
 * The HAL_Init() will setup the SysTick ISR at highest priority. Since Neon
 * must use the interrupt with lower priority (as defined by macro
 * CONFIG_CORE_LOCK_MAX_LEVEL) then another timer must be selected as core timer.
 */
void SysTick_Handler(void);

void SysTick_Handler(void)
{
    ncore_timer_isr();
}
#else
/* NOTE:
 * This is the STM32F4xx timer ISR
 * The ISR is changed by macro TIMER_HANDLER depending on the configured timer.
 */
void TIMER_HANDLER(void);

void TIMER_HANDLER(void)
{
    if (TIMER->SR &   TIM_SR_UIF) {
        TIMER->SR &= ~TIM_SR_UIF;
        ncore_timer_isr();
    }
}
#endif


void ncore_deferred_init(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;

    NVIC_SetPriority(PendSV_IRQn, NCORE_LOCK_TO_CODE(CONFIG_CORE_LOCK_MAX_LEVEL));
    NVIC_ClearPendingIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(PendSV_IRQn);
}



void PendSV_Handler(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;

    ncore_deferred_work();
}

void ncore_dummy_rd(volatile uint32_t value)
{
    (void)value;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if ((CONFIG_CORE_TIMER_SOURCE != 0) && (CONFIG_CORE_TIMER_SOURCE != 2) && \
     (CONFIG_CORE_TIMER_SOURCE != 3) && (CONFIG_CORE_TIMER_SOURCE != 4) && \
     (CONFIG_CORE_TIMER_SOURCE != 5))
# error "NEON::base::port::stm32f4xx-none-gcc: The selected timer in CONFIG_CORE_TIMER_SOURCE is not supported!"
#endif

/** @endcond *//** @} *//******************************************************
 * END of stm32f4xx_core.c
 ******************************************************************************/
