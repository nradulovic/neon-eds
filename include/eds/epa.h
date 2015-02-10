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

#include "port/compiler.h"
#include "shared/error.h"
#include "eds/epa_config.h"

/*===============================================================  MACRO's  ==*/

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

struct ndefine_epa {
    const char *        name;
    uint8_t             priority;
    uint8_t             queue_size;
};

typedef struct ndefine_epa nepaDefine;

struct nmem;
struct nevent;
struct nepa;
struct ndefine_sm;

struct nepa
{
    struct nmem *               mem;
    struct nsm                  sm;
    struct nthread              thread;
    struct event_fifo           event_fifo;
    const char *                name;
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    ndebug_magic                signature;
#endif
};

typedef struct nepa nepa;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        EPA Kernel functions
 * @{ *//*--------------------------------------------------------------------*/

void neds_init(void);

void neds_term(void);

void neds_run(void);

struct nepa * neds_get_current(void);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Resource management
 * @{ *//*--------------------------------------------------------------------*/

nerror nepaResourceAdd(
    size_t              size,
    void **             resource);

nerror nepaResourceRemove(
    void *              resource);

void nepaGetMem(struct nmem ** mem);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA management
 * @{ *//*--------------------------------------------------------------------*/

nerror nepaCreate(
    const struct ndefine_epa * epaDefine,
    const struct ndefine_sm * smDefine,
    struct nmem *      mem,
    struct nepa **     epa);

nerror nepaDestroy(
    struct nepa *      epa);

/**@} *//*----------------------------------------------------------------*//**
 * @name        EPA Event transport
 * @{ *//*--------------------------------------------------------------------*/

nerror nepaSendEventI(
    struct nepa *      epa,
    struct nevent *    event);

nerror nepaSendEvent(
    struct nepa *      epa,
    struct nevent *    event);

nerror nepaSendAheadEventI(
    struct nepa *      epa,
    struct nevent *    event);

nerror nepaSendAheadEvent(
    struct nepa *      epa,
    struct nevent *    event);

/**@} *//*------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa.h
 ******************************************************************************/
#endif /* ES_EPA_H_ */
