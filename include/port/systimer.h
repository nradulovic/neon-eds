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
 */

#ifndef NEON_BASE_PORT_SYSTIMER_H_
#define NEON_BASE_PORT_SYSTIMER_H_

#include "arch/systimer.h"

void nsystimer_init(
    nsystimer_tick              val);


/**@brief       Stop and terminate the system timer
 */
void nsystimer_term(void);



/**@brief       Get free counter value
 */
nsystimer_tick nsystimer_get_current(void);



/**@brief       Get reload counter value
 */
nsystimer_tick nsystimer_get_reload(void);



/**@brief       Load the system timer Reload value register
 */
void nsystimer_load(
    nsystimer_tick              tick);



/**@brief       Enable the system timer
 */
void nsystimer_enable(void);



/**@brief       Disable the system timer
 */
void nsystimer_disable(void);



/**@brief       Disable the system timer interrupt
 */
void nsystimer_isr_enable(void);



/**@brief       Enable the system timer interrupt
 */
void nsystimer_isr_disable(void);



/**@brief       Initialize port
 */
void nsystimer_module_init(void);



/**@brief       Terminate port
 */
void nsystimer_module_term(void);



/**@brief       User System Timer ISR
 */
extern void nsystimer_isr(void);

#endif /* NEON_BASE_PORT_SYSTIMER_H_ */
