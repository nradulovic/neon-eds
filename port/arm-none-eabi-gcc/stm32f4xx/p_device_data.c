/*
 * This file is part of neon-test.
 *
 * Copyright (C) 2010 - 2015 nenad
 *
 * neon-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * neon-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with neon-test.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    
 * e-mail  :    
 *//***********************************************************************//**
 * @file
 * @author      nenad
 * @brief       Brief description
 *********************************************************************//** @{ */
/**@defgroup    def_impl Implementation
 * @brief       Default Implementation
 * @{ *//*--------------------------------------------------------------------*/

/*=========================================================  INCLUDE FILES  ==*/

#include "port/peripheral.h"
#include "port/profile.h"
#include "port/uart.h"
#include "port/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/*-- GPIOA  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(1)))
static struct ngpio_driver              g_gpioa_driver;

static const struct np_dev_clock        g_gpioa_clock =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOAEN
};

const struct np_dev                     g_gpioa =
{
    .recognition        = NP_DEV_RECOGNITION(NP_DEV_CLASS_GPIO, 1),
    .p_drv              = &g_gpioa_driver.p_drv,
    .address            = (volatile unsigned int *)GPIOA_BASE,
    .clock              = &g_gpioa_clock
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_1) */

/*-- GPIOB  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(2))
static struct ngpio_driver              g_gpiob_driver;
static const struct np_dev_clock        g_gpiob_clock =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOBEN
};

const struct np_dev                     g_gpiob =
{
    .recognition        = NP_DEV_RECOGNITION(NP_DEV_CLASS_GPIO, 2),
    .p_drv              = &g_gpiob_driver.p_drv,
    .address            = (volatile unsigned int *)GPIOB_BASE,
    .clock              = &g_gpiob_clock
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_2) */

/*-- GPIOC  ------------------------------------------------------------------*/
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(3))
static struct ngpio_driver              g_gpioc_driver;

static const struct np_dev_clock        g_gpioc_clock =
{
    .reg                = &RCC->AHB1ENR,
    .mask               = RCC_AHB1ENR_GPIOCEN
};

const struct np_dev                     g_gpioc =
{
    .recognition        = NP_DEV_RECOGNITION(NP_DEV_CLASS_GPIO, 3),
    .p_drv              = &g_gpioc_driver.p_drv,
    .address            = (volatile unsigned int *)GPIOC_BASE,
    .clock              = &g_gpioc_clock
};
#endif /* (NPROFILE_EN_GPIO & NP_MASK_MAJOR_3) */

/*-- UART1  ------------------------------------------------------------------*/
#if (NPROFILE_EN_UART & NP_EN_MAJOR(1))
static struct nuart_drv                 g_uart1_driver;

static const struct np_dev_mux          g_uart1_mux =
{
    .af                 = GPIO_AF7_USART1,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};

static const struct np_dev_clock        g_uart1_clock =
{
    .reg                = &RCC->APB2ENR,
    .mask               = RCC_APB2ENR_USART1EN
};

static const struct np_dev_isr          g_uart1_isr =
{
    .irqn               = USART1_IRQn
};

const struct np_dev                     g_uart1 =
{
    .recognition        = NP_DEV_RECOGNITION(NP_DEV_CLASS_UART, 1),
    .flags              = NUART_MODE_RX    | NUART_MODE_TX      | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE  | NUART_STOPBITS_1  |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 |
                          NUART_WORDLENGTH_9,
    .p_drv              = &g_uart1_driver.p_drv,
    .address            = (volatile unsigned int *)USART1_BASE,
    .isr                = &g_uart1_isr,
    .clock              = &g_uart1_clock,
    .mux                = &g_uart1_mux
};

#endif /* (NPROFILE_EN_UART & NP_EN_MAJOR(1)) */

/*-- UART2  ------------------------------------------------------------------*/
#if (NPROFILE_EN_UART & NP_EN_MAJOR(2))
static struct nuart_drv                 g_uart2_driver;

static const struct np_dev_mux          g_uart2_mux =
{
    .af                 = GPIO_AF7_USART2,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};

static const struct np_dev_clock        g_uart2_clock =
{
    .reg                = &RCC->APB1ENR,
    .mask               = RCC_APB1ENR_USART2EN
};

static const struct np_dev_isr          g_uart2_isr =
{
    .irqn               = USART2_IRQn
};

const struct np_dev                     g_uart2 =
{
    .recognition        = NP_DEV_RECOGNITION(NP_DEV_CLASS_UART, 2),
    .flags              = NUART_MODE_RX    | NUART_MODE_TX      | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE  | NUART_STOPBITS_1  |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 |
                          NUART_WORDLENGTH_9,
    .p_drv              = &g_uart2_driver.p_drv,
    .address            = (volatile unsigned int *)USART2_BASE,
    .isr                = &g_uart2_isr,
    .clock              = &g_uart2_clock,
    .mux                = &g_uart2_mux
};
#endif /* (NPROFILE_EN_UART & NP_EN_MAJOR(2)) */

/*-- UART6  ------------------------------------------------------------------*/
#if (NPROFILE_EN_UART & NP_EN_MAJOR(6))
static struct nuart_drv                 g_uart6_driver;

static const struct np_dev_mux          g_uart6_mux =
{
    .af                 = GPIO_AF8_USART6,
    .mode               = GPIO_MODE_AF_PP,
    .pull               = GPIO_NOPULL
};

static const struct np_dev_clock        g_uart6_clock =
{
    .reg                = &RCC->APB2ENR,
    .mask               = RCC_APB2ENR_USART6EN
};

static const struct np_dev_isr          g_uart6_isr =
{
    .irqn               = USART6_IRQn
};

const struct np_dev                     g_uart6 =
{
    .recognition        = NP_DEV_RECOGNITION(NP_DEV_CLASS_UART, 6),
    .flags              = NUART_MODE_RX    | NUART_MODE_TX      | NUART_PARITY_EVEN |
                          NUART_PARITY_ODD | NUART_PARITY_NONE  | NUART_STOPBITS_1  |
                          NUART_STOPBITS_2 | NUART_WORDLENGTH_8 |
                          NUART_WORDLENGTH_9,
    .p_drv              = &g_uart6_driver.p_drv,
    .address            = (volatile unsigned int *)USART6_BASE,
    .isr                = &g_uart6_isr,
    .clock              = &g_uart6_clock,
    .mux                = &g_uart6_mux
};
#endif /* (NPROFILE_EN_UART & NP_EN_MAJOR(6)) */

/*======================================================  GLOBAL VARIABLES  ==*/

#if (NPROFILE_EN_GPIO)
const struct np_dev *                   g_gpios[] =
{
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(1))
    &g_gpioa,
#endif
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(2))
    &g_gpiob,
#endif
#if (NPROFILE_EN_GPIO & NP_EN_MAJOR(3))
    &g_gpioc,
#endif
    NULL
};
#endif /* NPROFILE_EN_GPIO */

#if (NPROFILE_EN_UART)
const struct np_dev *                   g_uarts[] =
{
#if (NPROFILE_EN_UART & NP_EN_MAJOR(1))
    &g_uart1,
#endif
#if (NPROFILE_EN_UART & NP_EN_MAJOR(2))
    &g_uart2,
#endif
#if (NPROFILE_EN_UART & NP_EN_MAJOR(6))
    &g_uart6,
#endif
    NULL
};
#endif /* NPROFILE_EN_UART */

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of profile.c
 ******************************************************************************/
