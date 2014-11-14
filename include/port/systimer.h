#ifndef NSYSTIMER_H
#define NSYSTIMER_H

#include "arch/systimer.h"

void nsystimer_init(
    nsystimer_tick              tick);


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

#endif /* NSYSTIMER_H */
