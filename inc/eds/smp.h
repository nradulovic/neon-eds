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
 * @author      Nenad Radulovic
 * @brief   	State Machine Processor header
 * @defgroup    eds_smp State Machine Processor
 * @brief       State Machine Processor
 *********************************************************************//** @{ */
/**@defgroup    eds_smp_intf Interface
 * @brief       State Machine Processor API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_SMP_H_
#define ES_SMP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/compiler.h"
#include "mem/mem_class.h"
#include "eds/event.h"
#include "eds/smp_config.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/

#define ES_STATE_ID(state)                                                      \
    ES_ACTION_##state

#define ES_EXPAND_STATE_ID(state, super)                                        \
    ES_STATE_ID(state),

#define ES_EXPAND_STATE_SUPER(state, super)                                     \
    { ES_STATE_ID(super), &state },

#define ES_STATE_ID_INIT(table)                                                 \
    table##_FIRST_STATE,                                                        \
    table(ES_EXPAND_STATE_ID)                                                   \
    table##_LAST_STATE

#define ES_STATE_TABLE_INIT(table)                                              \
    {{0, NULL} ,                                                                \
    table(ES_EXPAND_STATE_SUPER)                                                \
    {0, NULL}}

#define ES_SM_DEFINE(table, wspaceSize, initState)                              \
    {                                                                           \
        table,                                                                  \
        wspaceSize,                                                             \
        ES_STATE_ID(initState)                                                  \
    }

#define ES_STATE_TRANSITION(state)                                              \
    (ES_STATE_ID(state))

#define ES_STATE_HANDLED()                                                      \
    (ES_ACTION_HANDLED)

#define ES_STATE_IGNORED()                                                      \
    (ES_ACTION_IGNORED)

#define ES_SMP_EVENT(event)                                                     \
    (struct esEvent *)&esGlobalSmEvents[(event) - CONFIG_SMP_EVENT_ID_BASE]

#define ES_SMP_DEFINE_INIT(table, workspace, initState)                         \
    {table, workspace, ES_STATE_ID(initState)}

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       State machine processor event identifications
 * @api
 */
enum esSmEvents {
    ES_ENTRY            = CONFIG_SMP_EVENT_ID_BASE,                             /**<@brief Process state entry                              */
    ES_EXIT,                                                                    /**<@brief Process state exit                               */
    ES_INIT,                                                                    /**<@brief Process state initialization                     */
    ES_USER_ID          = CONFIG_SMP_EVENT_ID_BASE + 16u
};

enum esAction {
    ES_ACTION_IGNORED   = -1,
    ES_ACTION_HANDLED   = -2,
    ES_ACTION_DEFFERED  = -3,
    ES_ACTION_TOP       = 0,
    ES_ACTION_BOTTOM    = 32767
};

typedef int_fast16_t esAction;

typedef esAction (* esState) (void *, struct esEvent *);

struct esSm;

typedef struct esSm esSm;

struct esSmTable {
    esAction            super;
    esState             state;
};

typedef struct esSmTable esSmTable;

struct esSmDefine {
    const struct esSmTable * table;
    size_t              wspaceSize;
    esAction            initState;
};

typedef struct esSmDefine esSmDefine;

/*======================================================  GLOBAL VARIABLES  ==*/

extern const PORT_C_ROM struct esEvent esGlobalSmEvents[];

/*===================================================  FUNCTION PROTOTYPES  ==*/

esError esSmCreate(
    const struct esSmDefine * define,
    struct esMem * mem,
    struct esSm **      sm);

esError esSmDestroy(
    struct esSm *       sm);

esError esSmDispatch(
    struct esSm *       sm,
    const struct esEvent * event,
    esAction *          action);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp.h
 ******************************************************************************/
#endif /* ES_SMP_H_ */
