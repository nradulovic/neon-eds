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
 * @brief       Event Processing Agent header
 * @defgroup    eds_epa Event Processing Agent
 * @brief       Event Processing Agent
 *********************************************************************//** @{ */
/**@defgroup    eds_epa_intf Interface
 * @brief       Event Processing Agent API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_EPA_H_
#define ES_EPA_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"
#include "base/error.h"
#include "mem/mem_class.h"
#include "eds/smp.h"
#include "eds/epa_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Kernel version
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Kernel version major number
 */
#define ES_EPA_KERNEL_MAJOR             1

/**@brief       Kernel version minor number
 */
#define ES_EPA_KERNEL_MINOT             0

/**@brief       Kernel version patch number
 */
#define ES_EPA_KERNEL_PATCH             0

/**@} *//*--------------------------------------------------------------------*/

#define ES_EPA_DEFINE(name, priority, queueSize)                                \
    {                                                                           \
        name,                                                                   \
        priority,                                                               \
        queueSize                                                               \
    }

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct esEpaDefine {
    const PORT_C_ROM char * name;
    uint8_t             priority;
    uint8_t             queueSize;
};

typedef struct esEpaDefine esEpaDefine;

struct esEpa;

typedef struct esEpa esEpa;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        EPA Kernel functions
 * @{ *//*--------------------------------------------------------------------*/

esError esEdsInit(
    void);

esError esEdsTerm(
    void);

esError esEdsStart(
    void);

esError esEdsStop(
    void);

/**@brief       Set idle routine function
 * @param       idle
 *              Pointer to idle function routine
 * @return      eSolid error:
 *              - @ref ES_ERROR_NONE - call was successful
 * @details     The idle function puts the processor in the lowest power mode
 *              capable to serve interrupts. This function is executed only if
 *              there are no other ready EPAs in the system.
 * @note        Idle function must return. If it is implemented as a loop EPAs
 *              execution will stall.
 */
void esEdsSetIdle(
    void             (* idle)(void));

void esEdsGetIdle(
    void            (** idle)(void));

void esEdsGetCurrent(
    struct esEpa **     epa);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Resource management
 * @{ *//*--------------------------------------------------------------------*/

esError esEpaResourceAdd(
    size_t              size,
    void **             resource);

esError esEpaResourceRemove(
    void *              resource);

void esEpaGetMem(esMem ** mem);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA management
 * @{ *//*--------------------------------------------------------------------*/

esError esEpaCreate(
    const struct esEpaDefine * epaDefine,
    const struct esSmDefine * smDefine,
    struct esMem *      mem,
    struct esEpa **     epa);

esError esEpaDestroy(
    struct esEpa *      epa);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Event transport
 * @{ *//*--------------------------------------------------------------------*/

esError esEpaSendEventI(
    struct esEpa *      epa,
    struct esEvent *    event);

esError esEpaSendEvent(
    struct esEpa *      epa,
    struct esEvent *    event);

esError esEpaSendAheadEventI(
    struct esEpa *      epa,
    struct esEvent *    event);

esError esEpaSendAheadEvent(
    struct esEpa *      epa,
    struct esEvent *    event);

/**@} *//*------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.h
 ******************************************************************************/
#endif /* ES_EPA_H_ */
