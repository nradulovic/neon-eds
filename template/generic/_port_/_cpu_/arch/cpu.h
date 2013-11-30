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
 * @brief       Interface of CPU port - Template
 * @addtogroup  template_cpu_intf
 * @brief       CPU port macros and functions
 * @details     Since this header file is included with the API of the kernel a
 *              few naming conventions are defined in order to avoid name
 *              clashing with the names of objects from libraries included by
 *              application code.
 *
 * @par         1) Macro naming conventions
 *              For macro naming try to follow these rules:
 *              - All standard PORT API macro names are prefixed with:
 *              @b @c PORT_.
 *              - All other macros which are specific to the port used are
 *              prefixed with: @b @c CPU_.
 *
 * @par         2) Type declaration naming conventions
 *              For type declaration naming try to follow these rules:
 *              - All type declaration names are prefixed with: @b @c port.
 *
 * @par         3) Global variable naming conventions
 *              For global variable naming try to follow these rules:
 *              - All global variable names are prefixed with: @b @c Port.
 *
 * @par         4) Function naming conventions
 *              For functions naming try to follow these rules:
 *              - All standard PORT API function names are prefixed with:
 *              <b><code>port</code></b>.
 *              - All other functions which are specific to the port used are
 *              prefixed with: <b><code>cpu</code></b>
 *              - All inline functions are additionally postfixed with: @b @c _
 *              (underscore).
 *              - The @c exception to above two rules are the names of functions
 *              used for Interrupt Service Routines. They can have any name
 *              required by the port.
 *
 *********************************************************************//** @{ */

#if !defined(CPU_H__)
#define CPU_H__

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/compiler.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       This macro specifies the bit width of atomic data type portReg_T
 * @details     To avoid uncertainty about interrupting access to a variable,
 *              you can use a particular data type for which access is always
 *              atomic: @ref portReg_T.
 */
#define PORT_DEF_DATA_WIDTH             8u

/**@brief       Defines required data alignment in bytes
 */
#define PORT_DEF_DATA_ALIGNMENT         1u

/**@} *//*----------------------------------------------------------------*//**
 * @name        Bit operations
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Find last set bit in a word
 * @param       val
 *              Value : portReg_T, value which needs to be evaluated
 * @return      The position of the last set bit in a value
 * @details     This function is used by the scheduler to efficiently determine
 *              the highest priority of thread ready for execution. For similar
 *              algorithm details see:
 *              http://en.wikipedia.org/wiki/Find_first_set.
 */
#define PORT_BIT_FIND_LAST_SET(val)         portCpuFLS(val)

/**@brief       Helper macro: calculate 2^pwr expression
 * @param       pwr
 *              Power : portReg_T, value which will be used in calculation
 * @details     Some ports may want to use look up tables instead of shifting
 *              operation
 */
#define PORT_BIT_PWR2(pwr)                  Pwr2LKP[pwr]

/**@} *//*----------------------------------------------------------------*//**
 * @name        General port macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Early port initialization
 * @details     This macro will be called at early initialization stage. It is
 *              called before any system data initialization. Usually this macro
 *              would be used to setup memory space, fill the memory with debug
 *              value or something similar.
 */
#define PORT_INIT_EARLY()

/**@brief       Port initialization
 * @details     This macro will be called after initialization of system data
 *              structures.
 */
#define PORT_INIT()

/**@brief       Late port initialization
 * @details     This macro will be called just a moment before the multitasking
 *              is started.
 */
#define PORT_INIT_LATE()

/**@brief       Terminate port
 * @details     This macro will be called when there is a need to stop any
 *              further execution (example: an error occured and CPU needs to
 *              stop).
 */
#define PORT_TERM()

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Data type which corresponds to the general purpose register
 * @details     Reading and writing this data type is guaranteed to happen in a
 *              single instruction, so there's no way for a handler to run “in
 *              the middle” of an access.
 *
 *              The type portReg_T is always an integer data type, but which one
 *              it is, and how many bits it contains, may vary from machine to
 *              machine.
 * @note        This data type will always have maximum number of bits which can
 *              be accessed atomically.
 */
typedef uint8_t portReg_T;

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Look up table for: 2^n expression
 * @details     This look up table can be used to accelerate the Logical Shift
 *              Left operations which are needed to set bits inside the priority
 *              bit map. In plain C this operation would be written as:
 *              <code>(1u << n)</code>, but in many 8-bit CPUs this operation
 *              can be lengthy. If there is a need for faster operation then
 *              this table can be used instead of the mentioned C code.
 *
 *              To use the look up table change @ref PORT_BIT_PWR2 macro
 *              implementation from: <code>(1u << (pwr))</code> to
 *              <code>pwr2LKP[pwr]</code>
 */
extern const PORT_C_ROM portReg_T Pwr2LKP [PORT_DEF_DATA_WIDTH];

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Scheduler support
 * @note        These functions are extensively used by the scheduler and
 *              therefore they should be optimized for the architecture being
 *              used.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Find last set bit in a word
 * @param       val
 *              Value which needs to be evaluated
 * @return      The position of the last set bit in a word
 * @details     This function is used by the scheduler to efficiently determine
 *              the highest priority of thread ready for execution. For
 *              algorithm details see:
 *              http://en.wikipedia.org/wiki/Find_first_set.
 */
uint_fast8_t portCpuFLS(
    portReg_T       val);

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of cpu.h
 ******************************************************************************/
#endif /* CPU_H_ */
