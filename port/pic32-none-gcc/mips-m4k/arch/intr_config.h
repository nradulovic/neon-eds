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
 * @brief   	Configuration of Interrupt module port.
 * @addtogroup  pic32-none-gcc-intr
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-intr_cfg Interrupt module configuration
 * @brief       Interrupt module configuration
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_INTR_CONFIG_H_
#define ES_INTR_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "family/profile.h"

/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/**@brief       Priority of critical sections in kernel
 * @details     Specify the priority in range: <code>0</code>\f$\leq\f$
 *              <code>CONFIG_INTR_MAX_ISR_PRIO</code> \f$\leq\f$ <code>7</code>.
 *              The higher the number the higher the priority.
 * @note        When priority is set to @b 0 then critical code section will not
 *              use priority levels but instead it will just disable interrupts
 *              on entry and enable interrupts on exit.
 */
#if !defined(CONFIG_INTR_MAX_ISR_PRIO)
# define CONFIG_INTR_MAX_ISR_PRIO       6
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of intr_config.h
 ******************************************************************************/
#endif /* ES_INTR_CONFIG_H_ */
