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
 * @brief       Template for System Control Configuration.
 * @addtogroup  template_sysctrl_cfg
 * @brief       System Control settings
 * @details     Each configuration option or setting has its own default value
 *              when not defined by the application. When application needs to
 *              change a setting it just needs to define a configuration macro
 *              with another value and the default configuration macro will be
 *              overridden.
 *********************************************************************//** @{ */

#if !defined(SYSCTRL_CFG_H__)
#define SYSCTRL_CFG_H__

/*=========================================================  INCLUDE FILES  ==*/

#include "family/profile.h"

/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/**@brief       The frequency of clock which is used for the system timer
 * @details     Specify here the clock value so the kernel can properly manage
 *              system tick event generation. Usually system timer will use the
 *              clock of the processor. A hardware timer is configured to
 *              generate an interrupt at a rate between 10 and 1000 Hz which
 *              provides the system tick. The rate of interrupt is application
 *              specific and depends on the desired resolution system tick time
 *              source. However, the faster the tick rate, the higher the
 *              overhead will be imposed on the system.
 */
#if !defined(PORT_CFG_SYSTMR_CLOCK_FREQ)
# define PORT_CFG_SYSTMR_CLOCK_FREQ     1000000ul
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of sysctrl_cfg.h
 ******************************************************************************/
#endif /* SYSCTRL_CFG_H__ */
