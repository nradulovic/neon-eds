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
 * @author      Nenad Radulovic
 * @brief       Interface of ARM Cortex CPU module port.
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */

#ifndef ES_CORTEX_M3_H_
#define ES_CORTEX_M3_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

/*===============================================================  MACRO's  ==*/

/*--  On AIRCR register writes, write 0x5FA, otherwise the write is ignored  -*/


#define PORT_SCB_ICSR                       (*(volatile unsigned int *)0xE000ED04)
#define PORT_SCB_ICSR_PENDSTSET             (1ul << 26)
#define PORT_SCB_ICSR_PENDSTCLR             (1ul << 25)

#define PORT_SCB_AIRCR                      (*(volatile unsigned int *)0xE000ED0C)
#define PORT_SCB_AIRCR_VECTKEY_Pos          16
#define PORT_SCB_AIRCR_VECTKEY              (0xfffful << 16)
#define PORT_SCB_AIRCR_VECTKEY_VALUE        (0x5faul << 16)
#define PORT_SCB_AIRCR_PRIGROUP_Pos         8
#define PORT_SCB_AIRCR_PRIGROUP             (0x7ul << 8)
#define PORT_SCB_SHP_SYSTICK                (*(volatile unsigned char *)0xE000ED22)

/*--  SysTick Control / Status Register Definitions  -------------------------*/
#define PORT_SYST_CSR                       (*(volatile unsigned int *)0xE000E010)
#define PORT_SYST_CSR_CLKSOURCE             (1ul << 2)
#define PORT_SYST_CSR_TICKINT               (1ul << 1)
#define PORT_SYST_CSR_ENABLE                (1ul << 0)

#define PORT_SYST_RVR                       (*(volatile unsigned int *)0xE000E014)
#define PORT_SYST_CVR                       (*(volatile unsigned int *)0xE000E018)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*==================================================  CONFIGURATION ERRORS  ==*/
/******************************************************************************
 * END of cortex_m3.h
 ******************************************************************************/
#endif /* ES_CORTEX_M3_H_ */
