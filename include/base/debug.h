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
 * @brief       Debug module header
 * @defgroup    common_debug Debug
 * @brief       Basic debugging support
 *********************************************************************//** @{ */
/**@defgroup    common_debug_intf Interface
 * @brief       Basic debugging support
 * @details     For more details see @ref errors_intro.
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_DEBUG_H_
#define NEON_BASE_DEBUG_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "port/compiler.h"
#include "base/config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Error checking
 * @brief       These features are enabled/disabled using the option
 *              @ref CONFIG_DEBUG.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Static assert macro
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @details     This macro will evaluate the given expression at compile time.
 *              If the expression is not true the compilation process will stop
 *              with an error message about negative size of array.
 * @api
 */
#if defined(PORT_C_STATIC_ASSERT)
# define NASSERT_STATIC(msg, expr)                                              \
    PORT_C_STATIC_ASSERT(expr)
#else
# define NASSERT_STATIC(msg, expr)                                              \
    extern char static_assert_has_failed_##msg[(expr) ? 1 : -1]
#endif

/**@brief       Generic assert macro.
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_DEBUG == 1)
# define NASSERT(msg, expr)                                                     \
    if (!(expr)) {                                                              \
        hook_at_assert(&g_component_info, PORT_C_FUNC, PORT_C_LINE, #expr, msg);\
    }
#else
# define NASSERT(msg, expr)               (void)0
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
    hook_at_assert(&g_component_info, PORT_C_FUNC, PORT_C_LINE, text, NULL);
#else
# define NASSERT_ALWAYS(text)               (void)0
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Internal checking
 * @brief       These macros are enabled/disabled using the option
 *              @ref CONFIG_DEBUG_INTERNAL_CHECK.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Assert macro used for internal execution checking
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_DEBUG_INTERNAL == 1)
# define NASSERT_INTERNAL(msg, expr)        NASSERT(msg, expr)
#else
# define NASSERT_INTERNAL(msg, expr)        (void)0
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
# define NOBLIGATION(expr)                  expr
#else
# define NOBLIGATION(expr)                  (void)0
#endif

/**@brief       Make sure the caller has fulfilled all contract preconditions
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if (CONFIG_API_VALIDATION == 1)
# define NREQUIRE(msg, expr)                NASSERT(msg, expr)
#else
# define NREQUIRE(msg, expr)                (void)0
#endif

/**@brief       Make sure the callee has fulfilled all contract postconditions
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if    (CONFIG_DEBUG == 1)
# define NENSURE(expr)                                                          \
    if ((expr) != NERROR_NONE) {                                                \
        hook_at_assert(&g_component_info, PORT_C_FUNC, PORT_C_LINE, #expr,      \
            NAPI_CALL);                                                         \
    }
#else
# define NENSURE(expr)                      expr
#endif

/**@brief       Make sure the caller has fulfilled all contract preconditions
 * @param       msg
 *              Message : a standard error message, see
 *              @ref Standard error messages.
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if   (CONFIG_API_VALIDATION == 1) && (CONFIG_DEBUG_API == 1)
# define NREQUIRE_INTERNAL(msg, expr)       NASSERT(msg, expr)
#else
# define NREQUIRE_INTERNAL(msg, expr)       (void)0
#endif

/**@brief       Make sure the callee has fulfilled all contract postconditions
 * @param       expr
 *              Expression : C expression : condition which must be 'true'.
 * @api
 */
#if   (CONFIG_DEBUG_INTERNAL == 1) && (CONFIG_API_VALIDATION == 1)
# define NENSURE_INTERNAL(expr)             NENSURE(expr)
#else
# define NENSURE_INTERNAL(expr)             expr
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Standard error messages
 * @brief       These are used throughout Neon software collection in a concise
 *              way to inform about error details.
 * @{ *//*--------------------------------------------------------------------*/

#define NAPI_RANGE                          "Value is out of valid range."
#define NAPI_OBJECT                         "Object is not valid."
#define NAPI_POINTER                        "Pointer has null value."
#define NAPI_USAGE                          "Object/method usage failure."
#define NAPI_CALL                           "An API method call has failed."
#define NASSERT_FAILED                      "Assert failed"

/**@} *//*----------------------------------------------------------------*//**
 * @name        Object debug signatures
 * @brief       During debugging the objects will use the signatures
 * @{ *//*--------------------------------------------------------------------*/

#define NSIGNATURE_HEAP                     ((unsigned int)0xdeadbee0u)
#define NSIGNATURE_POOL                     ((unsigned int)0xdeadbee1u)
#define NSIGNATURE_STATIC                   ((unsigned int)0xdeadbee2u)
#define NSIGNATURE_TIMER                    ((unsigned int)0xdeadcee0u)
#define NSIGNATURE_THREAD                   ((unsigned int)0xdeaddee0u)
#define NSIGNATURE_EPA                      ((unsigned int)0xdeadfeeau)
#define NSIGNATURE_EQUEUE                   ((unsigned int)0xdeadfeebu)
#define NSIGNATURE_ETIMER                   ((unsigned int)0xdeadfeecu)
#define NSIGNATURE_EVENT                    ((unsigned int)0xdeadfeedu)
#define NSIGNATURE_SM                       ((unsigned int)0xdeadfeeeu)

/**@} *//*-----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct ncomponent_info;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


/**@brief       An assertion has failed
 * @param       component_info
 *              Software component information.
 * @param       fn
 *              Function name where the error occurred.
 * @param       line
 *              Code line of the error.
 * @param       expr
 *              Expression: is pointer to the string containing the expression
 *              that failed to evaluate to `true`.
 * @param       msg
 *              String containing some information about the error.
 * @note        1) This function may be called only if @ref CONFIG_DEBUG is
 *              active.
 * @details     An assertion has failed. This function should inform the user
 *              about failed assertion.
 * @api
 */
PORT_C_NORETURN
void hook_at_assert(
    const struct ncomponent_info * component_info,
    const char *     			fn,
    uint32_t                    line,
    const char *     			expr,
    const char *     			msg);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of debug.h
 ******************************************************************************/
#endif /* NEON_BASE_DEBUG_H_ */
