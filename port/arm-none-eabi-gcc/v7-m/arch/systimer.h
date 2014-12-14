/*
 * systimer.h
 *
 *  Created on: Oct 20, 2014
 *      Author: nenad
 */

#ifndef ARCH_SYSTIMER_H
#define ARCH_SYSTIMER_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>

#include "port/compiler.h"
#include "family/profile.h"
#include "arch/port_config.h"
#include "arch/cortex_m3.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Core timer one tick value
 */
#define NSYSTIMER_ONE_TICK(clock)                                               \
    (((clock) + (CONFIG_SYSTIMER_EVENT_FREQ / 2)) / CONFIG_SYSTIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the core timer
 */
#define NSYSTIMER_MAX_TICKS                                                     \
    (UINT32_MAX / NSYSTIMER_ONE_TICK)

#define NSYSTIMER_TICK_MAX              UINT32_MAX

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Core timer hardware register type.
 */
typedef unsigned int nsystimer_tick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Initialize and start the system timer
 */
PORT_C_INLINE
void nsystimer_init(void)
{
    PORT_SYSTICK->CTRL &= ~PORT_SYSTICK_CTRL_ENABLE_Msk;     /* Disable timer */
    PORT_SYSTICK->VAL   = 0u;
}



/**@brief       Stop and terminate the system timer
 */
PORT_C_INLINE
void nsystimer_term(void)
{
    PORT_SYSTICK->CTRL &= ~PORT_SYSTICK_CTRL_ENABLE_Msk;
}



/**@brief       Get free counter value
 */
PORT_C_INLINE
nsystimer_tick nsystimer_get_current(void)
{
    return (PORT_SYSTICK->VAL);
}



/**@brief       Get reload counter value
 */
PORT_C_INLINE
nsystimer_tick nsystimer_get_reload(void)
{
    return (PORT_SYSTICK->LOAD);
}



/**@brief       Load the system timer Reload value register
 */
PORT_C_INLINE
void nsystimer_load(
    nsystimer_tick            	val)
{
    --val;
    PORT_SYSTICK->CTRL &= ~PORT_SYSTICK_CTRL_ENABLE_Msk;
    PORT_SYSTICK->LOAD  = val;
    PORT_SYSTICK->VAL   = 0u;
    PORT_SYSTICK->CTRL |= PORT_SYSTICK_CTRL_ENABLE_Msk;
}



/**@brief       Enable the system timer
 */
PORT_C_INLINE
void nsystimer_enable(void)
{
    PORT_SYSTICK->CTRL |= PORT_SYSTICK_CTRL_ENABLE_Msk;
}



/**@brief       Disable the system timer
 */
PORT_C_INLINE
void nsystimer_disable(void)
{
    PORT_SYSTICK->CTRL &= ~PORT_SYSTICK_CTRL_ENABLE_Msk;
}



/**@brief       Disable the system timer interrupt
 */
PORT_C_INLINE
void nsystimer_isr_enable(void)
{
    PORT_SYSTICK->CTRL |= PORT_SYSTICK_CTRL_TICKINT_Msk;
}



/**@brief       Enable the system timer interrupt
 */
PORT_C_INLINE
void nsystimer_isr_disable(void)
{
    PORT_SCB->ICSR     |= PORT_SCB_ICSR_PENDSTCLR_Msk;
    PORT_SYSTICK->CTRL &= ~PORT_SYSTICK_CTRL_TICKINT_Msk;
}



/**@brief       Initialise System Timer port
 */
void nsystimer_module_init(void);



/**@brief       Terminate System Timer port
 */
void nsystimer_module_term(void);



extern void nsystimer_isr(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* ARCH_SYSTIMER_H */
