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

#include "common/compiler.h"
#include "family/profile.h"
#include "arch/port_config.h"
#include "arch/cortex_m3.h"

/*===============================================================  MACRO's  ==*/

#define NISR_PRIO_TO_CODE(prio)                                                 \
    (((prio) << (8u - PORT_ISR_PRIO_BITS)) & 0xfful)

#define NISR_CODE_TO_PRIO(code)                                                 \
    (((code) & 0xfful) >> (8u - PORT_ISR_PRIO_BITS))

#define nisr_exit()                         (void)0

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

enum nisr_interrupt_no
{
    NONMASKABLEINT_IRQN   = -14,        /**<@brief Non Maskable Interrupt     */
    MEMORYMANAGEMENT_IRQN = -12,        /**<@brief Memory Management          */
    BUSFAULT_IRQN         = -11,        /**<@brief Bus Fault Interrupt        */
    USAGEFAULT_IRQN       = -10,        /**<@brief Usage Fault Interrupt      */
    SVCALL_IRQN           = -5,         /**<@brief SV Call Interrupt          */
    PENDSV_IRQN           = -2,         /**<@brief Pend SV Interrupt          */
    SYSTIMER_IRQN         = -1          /**<@brief System Tick Interrupt      */
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern bool g_isr_is_active;

/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Enable all interrupts
 */
PORT_C_INLINE
void nisr_enable(void)
{
    __asm __volatile__ (
        "@  nisr_enable                                     \n"
        "   cpsie   i                                       \n");
}



/**@brief       Disable all interrupts
 */
PORT_C_INLINE
void nisr_disable(void)
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



PORT_C_INLINE
void nisr_enter(void)
{
    g_isr_is_active = true;
}



PORT_C_INLINE
void nisr_pend_kernel(void)
{
    PORT_SCB->ICSR |= PORT_SCB_ICSR_PENDSVSET_Msk;
}



PORT_C_INLINE
bool nisr_is_active(void)
{
    return (g_isr_is_active);
}



/**@brief       Set Priority for Cortex-M  System Interrupts
 * @param       intrNum
 *              Interrupt number
 * @param       priority
 *              The priority of specified interrupt source. The parameter
 *              priority must be encoded with @ref ES_INTR_PRIO_TO_CODE.
 */
PORT_C_INLINE
void nisr_set_priority(
    enum nisr_interrupt_no      intr_no,
    uint32_t                    priority)
{
    PORT_SCB->SHP[((uint32_t)(intr_no) & 0x0ful) - 4u] = (uint8_t)priority;
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
