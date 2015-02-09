/*
 * isr.h
 *
 *  Created on: Oct 19, 2014
 *      Author: nenad
 */

#ifndef ARCH_ISR_H
#define ARCH_ISR_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>

#include "port/compiler.h"
#include "family/profile.h"
#include "arch/port_config.h"
#include "arch/cortex_m3.h"

/*===============================================================  MACRO's  ==*/

#define NISR_PRIO_TO_CODE(prio)                                                 \
    (((prio) << (8u - PORT_ISR_PRIO_BITS)) & 0xfful)

#define NISR_CODE_TO_PRIO(code)                                                 \
    (((code) & 0xfful) >> (8u - PORT_ISR_PRIO_BITS))

/*------------------------------------------------------  C++ extern begin  --*/
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


/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void nisr_global_enable(void)
{
    __asm __volatile__ (
        "@  nisr_enable                                     \n"
        "   cpsie   i                                       \n");
}



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void nisr_global_disable(void)
{
    __asm __volatile__ (
        "@  nisr_disable                                    \n"
        "   cpsid   i                                       \n");
}



/**@brief       Set the new interrupt priority state
 * @param       new_mask
 *              New interrupt priority mask or new state of interrupts
 * @note        Depending on @ref CONFIG_ISR_MAX_PRIO setting this function will
 *              either set the new priority of allowed interrupts or just
 *              disable/enable all interrupts.
 */
PORT_C_INLINE
void nisr_set_mask(
    nisr_ctx                   new_mask)
{
#if (CONFIG_ISR_MAX_PRIO != 0)
    __asm __volatile__ (
        "@  nisr_set_mask                                   \n"
        "   msr    basepri, %0                              \n"
        :
        : "r"(new_mask));
#else
    __asm __volatile__ (
        "@  nisr_set_mask                                   \n"
        "   msr    primask, %0                              \n"
        :
        : "r"(new_mask));
#endif
}



/**@brief       Get old and set new interrupt priority mask
 * @return      Current interrupt priority mask
 */
PORT_C_INLINE
nisr_ctx nisr_replace_mask(
    nisr_ctx                   new_mask)
{
#if (CONFIG_ISR_MAX_PRIO != 0)
	nisr_ctx                   old_mask;

    __asm __volatile__ (
        "@  nisr_replace_mask                               \n"
        "   mrs     %0, basepri                             \n"
        "   msr     basepri, %1                             \n"
        : "=&r"(old_mask)
        : "r"(new_mask));

    return (old_mask);
#else
    nisr_ctx                   old_mask;

    __asm __volatile__ (
        "@  nisr_replace_mask                               \n"
        "   mrs     %0, primask                             \n"
        "   msr    primask, %1                              \n"
        : "=&r"(old_mask)
        : "r"(new_mask));

    return (old_mask);
#endif
}



/**@brief       Initialise port
 */
void nisr_module_init(void);



/**@brief       Terminate port
 */
void nisr_module_term(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of isr.h
 ******************************************************************************/
#endif /* ARCH_ISR_H */
