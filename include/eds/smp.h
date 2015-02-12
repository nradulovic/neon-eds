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

#ifndef NEDS_SMP_H_
#define NEDS_SMP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "port/compiler.h"
#include "shared/config.h"

/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       State machine processor event identifications
 * @api
 */
enum nsmp_events {
    NSMP_SUPER          = 0u,           /**<@brief Get the state super state  */
    NSMP_ENTRY          = 1u,           /**<@brief Process state entry        */
    NSMP_EXIT           = 2u,           /**<@brief Process state exit         */
    NSMP_INIT           = 3u,           /**<@brief Process state init         */
    NEVENT_USER_ID      = 15u
};

enum naction {
    NACTION_SUPER       = 0,
    NACTION_TRANSIT_TO  = 1,
    NACTION_HANDLED     = 2,
    NACTION_IGNORED     = 3,
    NACTION_DEFFERED    = 4
};

enum nsm_type {
    NTYPE_FSM           = 0,
    NTYPE_HSM           = 1
};

typedef uint_fast8_t naction;

struct nevent;
struct nsm;

typedef naction (nstate) (struct nsm *, const struct nevent *);

struct nsm {
    naction                  (* dispatch)(struct nsm *, const struct nevent *);
    nstate *                    state;
    void *                      wspace;
#if (CONFIG_API_VALIDATION == 1)
    ndebug_magic                signature;
#endif
};

typedef struct nsm nsm;

struct nsm_define {
    void *                      wspace;
    nstate *                    init_state;
    enum nsm_type               type;
};

typedef struct nsm_define nsm_define;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


void nsm_init(struct nsm * sm, const struct nsm_define * sm_define);



void nsm_term(struct nsm * sm);



naction nsm_dispatch(struct nsm * sm, const struct nevent * event);



naction ntop_state(struct nsm * sm, const struct nevent * event);



PORT_C_INLINE
naction naction_transit_to(struct nsm * sm, nstate * state)
{
    sm->state = state;

    return (NACTION_TRANSIT_TO);
}



PORT_C_INLINE
naction naction_super(struct nsm * sm, nstate * state)
{
    sm->state = state;

    return (NACTION_SUPER);
}



const struct nevent * nsmp_event(enum nsmp_events event_id);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp.h
 ******************************************************************************/
#endif /* NEDS_SMP_H_ */
