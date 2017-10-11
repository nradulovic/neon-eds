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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Debugging support header
 * @addtogroup  base_intf
 *********************************************************************//** @{ */
/**@defgroup    base_debug Debugging support
 * @brief       Debugging support
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_DEBUG_H_
#define NEON_BASE_DEBUG_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "port/compiler.h"
#include "base/config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Error checking
 * @brief       These features are enabled/disabled using the option
 *              @ref CONFIG_DEBUG.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Generic assert macro.
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_DEBUG == 1)
# define NASSERT(expr)                                                     		\
    if (!(expr)) {                                                              \
        hook_at_assert(PORT_C_FILE, PORT_C_FUNC, PORT_C_LINE, #expr);			\
    }
#else
# define NASSERT(expr)               	(void)0
#endif

/**@brief       Assert macro that will always execute (no conditional).
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       text
 *              Text : string : a text which will be printed when this assert
 *              macro is executed.
 * @api
 */
#if (CONFIG_DEBUG == 1)
# define NASSERT_ALWAYS(text)                                                   \
    hook_at_assert(PORT_C_FILE, PORT_C_FUNC, PORT_C_LINE, text)
#else
# define NASSERT_ALWAYS(text)           (void)0
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        API contract validation
 * @brief       These macros are enabled/disabled using the option
 *              @ref CONFIG_DEBUG_API_VALIDATION.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Execute code to fulfill the contract
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_API_VALIDATION == 1)
# define NOBLIGATION(expr)              expr
#else
# define NOBLIGATION(expr)              (void)0
#endif

/**@brief       Make sure the caller has fulfilled all contract preconditions
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_API_VALIDATION == 1)
# define NREQUIRE(expr)               	NASSERT(expr)
#else
# define NREQUIRE(expr)                	(void)0
#endif

/**@brief       Make sure the callee has fulfilled all contract postconditions
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_API_VALIDATION == 1)
# define NENSURE(expr)                  NASSERT(expr)
#else
# define NENSURE(expr)                  (void)0
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Internal checking
 * @brief       These macros are enabled/disabled using the option
 *              @ref CONFIG_DEBUG_INTERNAL_CHECK.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Assert macro used for internal execution checking
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_ASSERT_INTERNAL == 1) && (CONFIG_DEBUG == 1)
# define NASSERT_INTERNAL(expr)        	NASSERT(expr)
#else
# define NASSERT_INTERNAL(expr)        	(void)0
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Object debug signatures
 * @brief       During debugging the objects will use the signatures
 * @{ *//*--------------------------------------------------------------------*/

#define NSIGNATURE_HEAP                     ((unsigned int)0xdeadbee0u)
#define NSIGNATURE_POOL                     ((unsigned int)0xdeadbee1u)
#define NSIGNATURE_STATIC                   ((unsigned int)0xdeadbee2u)
#define NSIGNATURE_STDHEAP                  ((unsigned int)0xdeadbee3u)
#define NSIGNATURE_TIMER                    ((unsigned int)0xdeadcee0u)
#define NSIGNATURE_THREAD                   ((unsigned int)0xdeaddee0u)
#define NSIGNATURE_EPA                      ((unsigned int)0xdeadfeeau)
#define NSIGNATURE_EQUEUE                   ((unsigned int)0xdeadfeebu)
#define NSIGNATURE_ETIMER                   ((unsigned int)0xdeadfeecu)
#define NSIGNATURE_EVENT                    ((unsigned int)0xdeadfeedu)
#define NSIGNATURE_SM                       ((unsigned int)0xdeadfeeeu)
#define NSIGNATURE_DEFER                    ((unsigned int)0xdeadfeefu)

#if (CONFIG_API_VALIDATION == 1)
#define NSIGNATURE_DECLARE                 	int _signature;
#define NSIGNATURE_INITIALIZER(signature)   ._signature = signature,
#else
#define NSIGNATURE_DECLARE
#define NSIGNATURE_INITIALIZER(signature)   
#endif

#define NSIGNATURE_OF(object)				((object) ? (object)->_signature : 0)
#define NSIGNATURE_IS(object, signature)	(object)->_signature = (signature)

/**@} *//*-----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       An assertion has failed
 * @param       file
 *              Filename where the assert fired.
 * @param       fn
 *              Function name where the error occurred.
 * @param       line
 *              Code line of the error.
 * @param       expr
 *              Expression: is pointer to the string containing the expression
 *              that failed to evaluate to `true`.
 * @note        1) This function may be called only if @ref CONFIG_DEBUG is
 *              active.
 * @details     An assertion has failed. This function should inform the user
 *              about failed assertion.
 * @api
 */
PORT_C_NORETURN
void hook_at_assert(const char * file, const char * fn, uint32_t line,
    const char * expr);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of debug.h
 ******************************************************************************/
#endif /* NEON_BASE_DEBUG_H_ */
