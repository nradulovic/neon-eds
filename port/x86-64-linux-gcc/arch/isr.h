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
 * @author      Nenad Radulovic
 * @brief       Interrupt module header
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-v7-m-intr ARM Cortex M3/M4 Interrupt module
 * @brief       Interrupt module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_INTR_H_
#define ES_INTR_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

#define NISR_MASK_TO_CODE(prio)             (prio)

#define NISR_MASK_TO_PRIO(code)             (code)

#define nisr_exit()                         (void)0

#define nisr_enter()                        (void)0

#define nisr_global_enable()                (void)0

#define nisr_global_disable()               (void)0

#define nisr_set_mask(new_mask)             (void)0

#define nisr_replace_mask(new_mask)         (void)0

#define nisr_pend_kernel()                  (void)0

#define nisr_is_active()                    false

#define nisr_module_init()                  (void)0

#define nisr_module_term()                  (void)0

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
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of intr.h
 ******************************************************************************/
#endif /* ES_INTR_H_ */
