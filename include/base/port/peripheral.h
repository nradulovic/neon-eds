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
 * @brief       Peripheral management
 * @defgroup    periph_intf Peripheral management
 * @brief       Peripheral management
 *********************************************************************//** @{ */

#ifndef NEON_PORT_PERIPHERAL_H_
#define NEON_PORT_PERIPHERAL_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include "../../../port/arm-none-eabi-gcc/stm32f/family/p_dev.h"


/*===============================================================  MACRO's  ==*/

#define NP_DEV_MAGIC                    ((uint16_t)0xde00u)
#define NP_DEV_RECOGNITION(class, instance)   { (class), (instance) }

#define NP_DEV_ID_TO_MINOR(id)          (((uint32_t)(id) >>  0u) & 0xffu)
#define NP_DEV_ID_TO_MAJOR(id)          (((uint32_t)(id) >>  8u) & 0xffu)
#define NP_DEV_ID_TO_CLASS(id)          (((uint32_t)(id) >> 16u) & 0xffffu)

#define NP_DEV_CREATE_ID(class, major, minor)                                   \
    ((((uint32_t)(class) & 0xffffu) << 16u) |                                   \
     (((uint32_t)(major) &   0xffu) <<  8u) |                                   \
     (((uint32_t)(minor) &   0xffu) <<  0u))

#define NP_EN_MAJOR_1                   NP_EN_MAJOR( 1)
#define NP_EN_MAJOR_2                   NP_EN_MAJOR( 2)
#define NP_EN_MAJOR_3                   NP_EN_MAJOR( 3)
#define NP_EN_MAJOR_4                   NP_EN_MAJOR( 4)
#define NP_EN_MAJOR_5                   NP_EN_MAJOR( 5)
#define NP_EN_MAJOR_6                   NP_EN_MAJOR( 6)
#define NP_EN_MAJOR_7                   NP_EN_MAJOR( 7)
#define NP_EN_MAJOR_8                   NP_EN_MAJOR( 8)
#define NP_EN_MAJOR_9                   NP_EN_MAJOR( 9)
#define NP_EN_MAJOR_10                  NP_EN_MAJOR(10)
#define NP_EN_MAJOR_11                  NP_EN_MAJOR(11)
#define NP_EN_MAJOR_12                  NP_EN_MAJOR(12)
#define NP_EN_MAJOR_13                  NP_EN_MAJOR(13)
#define NP_EN_MAJOR_14                  NP_EN_MAJOR(14)
#define NP_EN_MAJOR_15                  NP_EN_MAJOR(15)
#define NP_EN_MAJOR_16                  NP_EN_MAJOR(16)

#define NP_EN_MAJOR(major)              (0x1u << (major - 1u))

#define NP_DEV_CLASS_GPIO               (NP_DEV_MAGIC | 1)
#define NP_DEV_CLASS_UART               (NP_DEV_MAGIC | 2)
#define NP_DEV_CLASS_I2C                (NP_DEV_MAGIC | 2)
#define NP_DEV_CLASS_SPI                (NP_DEV_MAGIC | 2)

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct np_drv;
struct np_dev_isr;
struct np_dev_rst;
struct np_dev_clock;
struct np_dev_mux;

struct np_drv
{
    const struct np_dev *       p_dev;
    uint32_t                    ref;
    void *                      data;
};

struct np_drv_id
{
    const struct np_dev *       p_dev;
    struct np_drv *             p_drv;
};

struct np_dev
{
    struct np_dev_recognition
    {
        uint16_t                    dev_class;
        uint16_t                    dev_instance;
    }                           recognition;
    uint32_t                    flags;
    struct np_drv *             p_drv;
    const struct np_dev *       host;
#if (NP_ATTR_ADDRESS == 1)
    volatile void *             address;
#endif
#if (NP_ATTR_ISR == 1)
    const struct np_dev_isr *   isr;
#endif
#if (NP_ATTR_RESET == 1)
    const struct np_dev_rst *   rst;
#endif
#if (NP_ATTR_CLOCK == 1)
    const struct np_dev_clock * clock;
#endif
#if (NP_ATTR_MUX == 1)
    const struct np_dev_mux *   mux;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


#define np_dev_id(device)                                                       \
    (((device)->id >> 0u) & 0xffu)

#define np_dev_class_id(device)                                                 \
    ((device)->recognition.dev_class)

#define np_dev_flags(device)                                                    \
    (device)->flags

#define np_dev_driver(device)                                                   \
    (device)->p_drv

#define np_dev_address(device)                                                  \
    (device)->address

#define np_dev_mux(device)                                                      \
    (device)->mux

static inline
const struct np_dev * np_dev_find_by_major(const struct np_dev * dev_class[], uint32_t id)
{
    const struct np_dev *       dev;
    uint32_t                    major;

    major = NP_DEV_ID_TO_MAJOR(id);
    dev   = dev_class[major];

    if (dev) {

        if (dev->recognition.dev_instance != major) {

        }
    }

    return (dev);
}

#define np_drv_dev(drv)                 (drv)->p_dev

struct np_drv * np_drv_get(
    const struct np_dev *       dev);

void np_drv_put(
    struct np_drv *             drv);

#define np_drv_set_data(drv, priv_data)                                         \
    (drv)->data = (priv_data)

#define np_drv_get_data(drv)                                                    \
    (drv)->data

#define np_drv_ref_up(drv)                                                      \
    (++(drv)->ref)

#define np_drv_ref_down(drv)                                                    \
    (--(drv)->ref)

#define np_dev_clock(dev, num)          &((dev)->clock[num])

#define np_drv_clock_enable(drv, num)                                           \
    np_clock_enable(&(drv)->p_dev->clock[num])

#define np_drv_clock_disable(drv, num)                                          \
    np_clock_disable(&(drv)->p_dev->clock[num])

#define np_drv_isr_enable(drv, num)                                             \
    np_isr_enable(&(drv)->p_dev->isr[num])

#define np_drv_isr_disable(drv, num)                                            \
    np_isr_disable(&(drv)->p_dev->isr[num])

#define np_drv_isr_clear_flag(drv, num)                                         \
    np_isr_clear_flag(&(drv)->p_dev->isr[num])

#define np_drv_isr_set_flag(drv, num)                                           \
    np_isr_set_flag(&(drv)->p_dev->isr[num])

#define np_drv_isr_set_prio(drv, num, prio)                                     \
    np_isr_set_prio(&(drv)->p_dev->isr[num], prio)

#define np_drv_isr_get_prio(drv, num)                                           \
    np_isr_get_prio(&(drv)->p_dev->isr[num])

#define np_drv_mux_enable(drv, num, pin_id)                                     \
    np_mux_enable(&(drv)->p_dev->mux[num], pin_id)

#define np_drv_mux_find_by_function(drv, pin_function)                          \
    np_mux_find_by_function

#define np_drv_mux_disable(drv, num, pin_id)                                    \
    np_mux_disable(&(drv)->p_dev->mux[num], pin_id)

void np_clock_enable(const struct np_dev_clock * clock);
void np_clock_disable(const struct np_dev_clock * clock);
void np_isr_enable(const struct np_dev_isr * isr);
void np_isr_disable(const struct np_dev_isr * isr);
void np_isr_clear_flag(const struct np_dev_isr * isr);
void np_isr_set_flag(const struct np_dev_isr * isr);
void np_isr_set_prio(const struct np_dev_isr * isr, uint32_t prio);
uint32_t np_isr_get_prio(const struct np_dev_isr * isr);
void np_mux_enable(const struct np_dev_mux * mux, uint32_t pin_id);
void np_mux_disable(const struct np_dev_mux * mux, uint32_t pin_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of peripheral.h
 ******************************************************************************/
#endif  /* NEON_PORT_PERIPHERAL_H_ */

