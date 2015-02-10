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
 * @author  	Nenad Radulovic
 * @brief       Interface of ARM Cortex compiler port.
 * @addtogroup  arm-none-eabi-gcc
 *********************************************************************//** @{ */
/**@defgroup    arm-none-eabi-gcc-compiler Compiler support
 * @brief       Compiler support
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_COMPILER_H_
#define ES_COMPILER_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

/**@brief       C extension - make a function inline
 */
#define PORT_C_INLINE                   static __inline__

/**@brief       C extension - make a function inline - always
 */
#define PORT_C_INLINE_ALWAYS            static __inline__ __attribute__((__always_inline__))

/**@brief       Omit function prologue/epilogue sequences
 */
#define PORT_C_NAKED                    __attribute__((naked))

/**@brief       Provides function name for assert macros
 */
#if (__STDC_VERSION__ >= 199901L) || defined(__DOXYGEN__)
# define PORT_C_FUNC                    __func__
#elif (__GNUC__ >= 2)
# define PORT_C_FUNC                    __FUNCTION__
#else
# define PORT_C_FUNC                    "unknown"
#endif

/**@brief       Provides the free file's name which is being compiled
 */
#define PORT_C_FILE                     __FILE__

/**@brief       Provides the free source line
 */
#define PORT_C_LINE                     __LINE__

/**@brief       Declare a weak function
 */
#define PORT_C_WEAK                     __attribute__((weak))

/**@brief       Declare a function that will never return
 */
#define PORT_C_NORETURN                 __attribute__((noreturn))

#define PORT_C_UNUSED                   __attribute__((unused))

/**@brief       Declare a variable that will be stored in ROM address space
 */
#define PORT_C_ROM

/**@brief       Declare a pointer that will be stored in ROM address space
 */
#define PORT_C_ROM_VAR

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
#define CONTAINER_OF(ptr, type, member)                                         \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/*---------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief General purpose registers are 64bit wide.
 */
typedef unsigned int esAtomic;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of compiler.h
 ******************************************************************************/
#endif /* ES_COMPILER_H_ */
