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
 * @brief       Interface of System timer module port.
 * @addtogroup  pic32-none-gcc
 *********************************************************************//** @{ */
/**@defgroup    pic32-none-gcc-systmr System timer module
 * @brief       System timer module
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_SYSTIMER_H_
#define ES_SYSTIMER_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "plat/compiler.h"
#include "arch/systimer_config.h"
#include "family/profile.h"
#include "cpu.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       System timer one tick value
 */
#define ES_SYSTIMER_ONE_TICK                                                    \
    (CONFIG_SYSTIMER_CLOCK_FREQ / CONFIG_SYSTIMER_EVENT_FREQ)

/**@brief       Maximum number of ticks without overflowing the system timer
 */
#define ES_SYSTIMER_MAX_TICKS                                                   \
    (ES_PROFILE_MAX_SYSTIMER_VAL / ES_SYSTIMER_ONE_TICK)

/**@} *//*----------------------------------------------------------------*//**
 * @name        System timer management
 * @{ *//*--------------------------------------------------------------------*/

#define ES_MODULE_SYSTIMER_INIT()       portModuleSysTimerInit()

#define ES_MODULE_SYSTIMER_TERM()       portModuleSysTimerTerm()

#define ES_SYSTIMER_INIT(val)           portSysTimerInit_(val)

#define ES_SYSTIMER_TERM()              portSysTimerTerm_()

#define ES_SYSTIMER_GET_RVAL()          portSysTimerGetRVal_()

#define ES_SYSTIMER_GET_CVAL()          portSysTimerGetCVal_()

#define ES_SYSTIMER_RELOAD(val)         portSysTimerReload_(val)

#define ES_SYSTIMER_ENABLE()            portSysTimerEnable_()

#define ES_SYSTIMER_DISABLE()           portSysTimerDisable_()

#define ES_SYSTIMER_ISR_ENABLE()        portSysTimerIsrEnable_()

#define ES_SYSTIMER_ISR_DISABLE()       portSysTimerIsrDisable_()

#define ES_SYSTIMER_SET_HANDLER(handler, level)                                 \
    portSysTimerSetHandler(handler, level)

/**@} *//*-----------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       System timer hardware register type.
 */
typedef unsigned int esSysTimerTick;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

static PORT_C_INLINE void portSysTimerInit_(
    esSysTimerTick      tick) {

    esCpuReg            cause;

    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(tick);
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
}

static PORT_C_INLINE void portSysTimerTerm_(
    void) {

    esCpuReg            cause;

    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
}

static PORT_C_INLINE esSysTimerTick portSysTimerGetRVal_(
    void) {

    return (_CP0_GET_COMPARE());
}

static PORT_C_INLINE esSysTimerTick portSysTimerGetCVal_(
    void) {

    return (_CP0_GET_COMPARE() - _CP0_GET_COUNT());
}

static PORT_C_INLINE void portSysTimerReload_(
    esSysTimerTick      tick) {

    esCpuReg            cause;

    cause  = _CP0_GET_CAUSE();
    _CP0_SET_CAUSE(cause | _CP0_CAUSE_DC_MASK);
    _CP0_SET_COUNT(0u);
    _CP0_SET_COMPARE(tick);
    _CP0_SET_CAUSE(cause & ~_CP0_CAUSE_DC_MASK);
}

static PORT_C_INLINE void portSysTimerEnable_(
    void) {

    esCpuReg            cause;

    cause  = _CP0_GET_CAUSE();
    cause &= ~_CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
}


static PORT_C_INLINE void portSysTimerDisable_(
    void) {

    esCpuReg            cause;

    cause  = _CP0_GET_CAUSE();
    cause |= _CP0_CAUSE_DC_MASK;
    _CP0_SET_CAUSE(cause);
}

static PORT_C_INLINE void portSysTimerIsrEnable_(
    void) {

    IFS0CLR = _IFS0_CTIF_MASK;
    IEC0SET = _IEC0_CTIE_MASK;
}

static PORT_C_INLINE void portSysTimerIsrDisable_(
    void) {

    IEC0CLR = _IEC0_CTIE_MASK;
}

void portModuleSysTimerInit(
    void);

void portModuleSysTimerTerm(
    void);

void portSysTimerSetHandler(
    void             (* handler)(void),
    uint_fast8_t        level);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of systimer.h
 ******************************************************************************/
#endif /* ES_SYSTIMER_H_ */
