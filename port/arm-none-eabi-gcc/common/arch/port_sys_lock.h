/*
 * isr.h
 *
 *  Created on: Oct 19, 2014
 *      Author: nenad
 */

#ifndef ARCH_ISR_H
#define ARCH_ISR_H

/*=========================================================  INCLUDE FILES  ==*/

#include "port/compiler.h"
#include "shared/config.h"
#include "family/profile.h"

/*===============================================================  MACRO's  ==*/

#define NSYS_LOCK_LEVEL_TO_CODE(prio)                                           \
    (((prio) << (8u - PORT_ISR_PRIO_BITS)) & 0xfful)

#define NSYS_LOCK_CODE_TO_LEVEL(code)                                           \
    (((code) & 0xfful) >> (8u - PORT_ISR_PRIO_BITS))

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Interrupt context structure
 * @details     This type is used to declare variable type which will hold
 *              interrupt context data.
 */
struct nsys_lock
{
    unsigned int                level;
};


typedef struct nsys_lock nsys_lock;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       Set the new interrupt priority state
 * @param       new_mask
 *              New interrupt priority mask or new state of interrupts
 * @note        Depending on @ref CONFIG_ISR_MAX_PRIO setting this function will
 *              either set the new priority of allowed interrupts or just
 *              disable/enable all interrupts.
 */
PORT_C_INLINE
void nsys_lock_restore_level(
    struct nsys_lock *          lock)
{
#if (CONFIG_SYS_LOCK_MAX_LEVEL != 0)
    __asm __volatile__ (
        "@  nsys_restore_level                              \n"
        "   msr    basepri, %0                              \n"
        :
        : "r"(lock->level));
#else
    __asm __volatile__ (
        "@  nsys_restore_level                              \n"
        "   msr    primask, %0                              \n"
        :
        : "r"(lock->level));
#endif
}



/**@brief       Get old and set new interrupt priority mask
 * @return      Current interrupt priority mask
 */
PORT_C_INLINE
void nsys_lock_set_level(
    struct nsys_lock *          ctx,
    unsigned int                new_mask)
{
#if (CONFIG_SYS_LOCK_MAX_LEVEL != 0)
    __asm __volatile__ (
        "@  nsys_set_level                                  \n"
        "   mrs     %0, basepri                             \n"
        "   msr     basepri, %1                             \n"
        : "=&r"(ctx->level)
        : "r"(new_mask));
#else
    __asm __volatile__ (
        "@  nsys_set_level                                  \n"
        "   mrs     %0, primask                             \n"
        "   msr    primask, %1                              \n"
        : "=&r"(ctx->level)
        : "r"(new_mask));
#endif
}


/**@brief       Enter critical code section
 * @param       resource
 *              Interrupt resource, pointer to portable type variable which will
 *              hold the interrupt context state during the critical code
 *              section.
 */
PORT_C_INLINE
void nsys_lock_enter(
    struct nsys_lock *          lock)
{
    nsys_lock_set_level(lock, NSYS_LOCK_LEVEL_TO_CODE(CONFIG_SYS_LOCK_MAX_LEVEL));
}



/**@brief       Exit critical code section
 * @param       resource
 *              Interrupt resource, portable type variable which is holding a
 *              previously saved interrupt context state.
 */
PORT_C_INLINE
void nsys_lock_exit(
    struct nsys_lock *          lock)
{
    nsys_lock_restore_level(lock);
}

/**@brief       Initialise port
 */
void nsys_lock_module_init(void);



/**@brief       Terminate port
 */
void nsys_lock_module_term(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of isr.h
 ******************************************************************************/
#endif /* ARCH_ISR_H */
