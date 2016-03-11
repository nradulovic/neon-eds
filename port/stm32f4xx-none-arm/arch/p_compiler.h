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
 * @brief       Interface for the STM32F4xx GCC port.
 *********************************************************************//** @{ */

#ifndef NEON_ARCH_P_COMPILER_H_
#define NEON_ARCH_P_COMPILER_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

#define PORT_C_ARMCC

/**@brief       Try to force a function always to be inlined.
 *              Since there is no default language construct to ensure this,
 *              this will always only be an approximation depending on
 *              the compiler.
 */
#define PORT_C_INLINE                   static __inline

/**@brief       Same as @ref PORT_C_INLINE but has greater power over compiler
 */
#define PORT_C_INLINE_ALWAYS            static __forceinline

/**@brief       Omit function prologue/epilogue sequences
 */
#define PORT_C_NAKED                    __attribute__((naked))

#define PORT_C_UNUSED                   __attribute__((unused))

/**@brief       Provides function name for assert macros
 */
#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || defined(__DOXYGEN__)
# define PORT_C_FUNC                    __func__
#elif (__GNUC__ >= 2)
# define PORT_C_FUNC                    __FUNCTION__
#endif

/**@brief       Provides the free file's name which is being compiled
 */
#define PORT_C_FILE                     __FILE__

/**@brief       Provides the free source line
 */
#define PORT_C_LINE                     __LINE__

/**@brief       Declare a function that will never return
 */
#define PORT_C_NORETURN                 __attribute__((noreturn))

#define PORT_C_PACKED                   __attribute__((packed))

/**@brief       This attribute specifies a minimum alignment (in bytes) for
 *              variables of the specified type.
 */
#define PORT_C_ALIGN(align)             __attribute__((aligned (align)))

/**@brief       Cast a member of a structure out to the containing structure
 * @param       ptr
 *              the pointer to the member.
 * @param       type
 *              the type of the container struct this is embedded in.
 * @param       member
 *              the name of the member within the struct.
 */
#define PORT_C_CONTAINER_OF(ptr, type, member)                                  \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/**@brief		Prevent the compiler from merging or refetching accesses.
 * @details		The compiler is also forbidden from reordering successive
 * 				instances of PORT_C_ACCESS_ONCE(), but only when the compiler is
 * 				aware of some particular ordering.  One way to make the compiler
 * 				aware of ordering is to put the two invocations of
 * 				PORT_C_ACCESS_ONCE() in different C statements.
 */
#define PORT_C_ACCESS_ONCE(x) 			(*(volatile typeof(x) *)&(x))

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of p_compiler.h
 ******************************************************************************/
#endif /* NEON_ARCH_P_COMPILER_H_ */
