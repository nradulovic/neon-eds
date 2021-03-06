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
 * @brief       Common bit/logic operations
 * @addtogroup  base_intf
 *********************************************************************//** @{ */
/**@defgroup    base_bitop Common bit/logic operations
 * @brief       Common bit/logic operations
 * @{ *//*--------------------------------------------------------------------*/

#ifndef NEON_BASE_BITOP_H_
#define NEON_BASE_BITOP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <stdint.h>

#include "port/compiler.h"
#include "port/core.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Array macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Determines the first dimension of an array
 * @param       array
 *              An array : type unspecified
 * @mseffect
 */
#define NARRAY_DIMENSION(array)             (sizeof(array) / sizeof(array[0]))

/**@} *//*----------------------------------------------------------------*//**
 * @name        Integer division
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Round a division
 * @param       numerator
 * @param       denominator
 * @note        It is desirable that denominator is a constant expression,
 *              otherwise the compiler will generate two division operations.
 * @mseffect
 * @par         Example 1:
 *
 *              numerator   = 28
 *              denominator = 5
 *
 *              Integer division: 28 / 5 = 5
 *              Float division  : 28 / 5 = 5.6
 *              Rounded division: 28 / 5 = 6
 *
 * @par         Example 2:
 *
 *              numerator   = 27
 *              denominator = 5
 *
 *              Integer division: 27 / 5 = 5
 *              Float division  : 27 / 5 = 5.4
 *              Rounded division: 27 / 5 = 5
 */
#define NDIVISION_ROUND(numerator, denominator)                                 \
    (((numerator) + ((denominator) / 2u)) / (denominator))

/**@brief       Round up a division
 * @param       numerator
 * @param       denominator
 * @note        It is desirable that denominator is a constant expression,
 *              otherwise the compiler will generate one subtraction and one
 *              division operation.
 * @mseffect
 * @par         Example 1:
 *
 *              numerator   = 28
 *              denominator = 5
 *
 *              Integer division   : 28 / 5 = 5
 *              Float division     : 28 / 5 = 5.6
 *              Rounded up division: 28 / 5 = 6
 *
 * @par         Example 2:
 *
 *              numerator   = 27
 *              denominator = 5
 *
 *              Integer division   : 27 / 5 = 5
 *              Float division     : 27 / 5 = 5.4
 *              Rounded up division: 27 / 5 = 6
 */
#define NDIVISION_ROUNDUP(numerator, denominator)                               \
    (((numerator) + (denominator) - 1u) / (denominator))

/**@} *//*----------------------------------------------------------------*//**
 * @name        Data alignment
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Vrsi poravnjanje @a num promenjive sa granicama specificarane
 *              u @a align
 * @param       num                     Promenjiva koju treba poravnati,
 * @param       align                   granica poravnanja.
 * @mseffect
 * @par         Example 1:
 *
 *              num  : 10010101 = 149
 *              align: 00000100 = 4
 *              Result is 148.
 */
#define NALIGN(num, align)                                                      \
    ((num) & ~((align) - 1u))

/**@brief       Vrsi poravnjanje @a num promenjive sa granicama specificarane
 *              u @a align
 * @param       num                     Promenjiva koju treba poravnati,
 * @param       align                   granica poravnanja.
 * @mseffect
 * @par         Example 1:
 *
 *              num  : 10010101 = 149
 *              align: 00000100 = 4
 *              Result is 152.
 */
#define NALIGN_UP(num, align)                                                   \
    (((num) + (align) - 1u) & ~((align) - 1u))

/**@} *//*----------------------------------------------------------------*//**
 * @name        Logarithm of base 2
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Calculate log2 for value @c x during the compilation
 * @mseffect
 */
#define NLOG2_8(x)                                                              \
    ((x) <   2u ? 0u :                                                          \
     ((x) <   4u ? 1u :                                                         \
      ((x) <   8u ? 2u :                                                        \
       ((x) <  16u ? 3u :                                                       \
        ((x) <  32u ? 4u :                                                      \
         ((x) <  64u ? 5u :                                                     \
          ((x) < 128u ? 6u : 7u)))))))

#define n_msb_32(x)                     n_xb4(x)

#define n_lsb_32(x)                     n_xb1(x)

#define N_IS_POWEROF_2(num)														\
	(((num) != 0u) && (((num) & ((num) - 1)) == 0u))

/**@} *//*-----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


#define n_xb4(value)                    ncore_exu4(value)



#define n_xb3(value)                    ncore_exu3(value)



#define n_xb2(value)                    ncore_exu2(value)



#define n_xb1(value)                    ((uint8_t)(value))

/**@brief       Sign extend signed 24 bit integer to signed 32 bits integer
 * @param       val - signed 24 bit integer
 * @return
 */
#define n_ext_i24(value)                ncore_ext_i24(value)

PORT_C_INLINE
uint32_t n_float_to_u32(float val)
{
    union float_to_u32
    {
        float                       fvalue;
        uint32_t                    ivalue;
    }                           u;
    u.fvalue = val;

    return (u.ivalue);
}

PORT_C_INLINE
float n_u32_to_float(uint32_t val)
{
    union u32_to_float
    {
        uint32_t                    ivalue;
        float                       fvalue;
    }                           u;
    u.ivalue = val;

    return (u.fvalue);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of bitop.h
 ******************************************************************************/
#endif /* NEON_BASE_BITOP_H_ */
