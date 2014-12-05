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
 * @brief       Event object header
 * @defgroup    eds_event Event object
 * @brief       Event object
 *********************************************************************//** @{ */
/**@defgroup    eds_event_intf Interface
 * @brief       Event object API
 * @{ *//*--------------------------------------------------------------------*/

#ifndef ES_EVENT_H_
#define ES_EVENT_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "plat/compiler.h"
#include "arch/systimer.h"
#include "base/debug.h"
#include "mem/mem_class.h"
#include "eds/event_config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Bit mask which defines a reserved event
 * @details     When the bits defined in this bit mask are set the given event
 *              is marked as reserved. In this case EDS will not try to delete
 *              the event even if there are no EPA units who are using it. When
 *              the event is unmarked as reserved EDS will take it into
 *              consideration to delete it. This bit mask can be used when
 *              defining a static event in ROM address space.
 * @api
 */
#define ES_EVENT_RESERVED_Msk           ((uint16_t)(0x01u << 15))

/**@brief       Bit mask which defines a constant event
 * @details     When the bits defined in this bit mask are set the given event
 *              is marked as constant. In this case EDS will never try to delete
 *              it. Once the event is marked as constant it never can be
deleted.
 * @api
 */
#define ES_EVENT_CONST_Msk              ((uint16_t)(0x01u << 14))

/**@brief       This macro defines limit for event ID value
 * @details     An event must have ID which is below this limit.
 * @api
 */
#define ES_EVENT_REF_LIMIT              (ES_EVENT_CONST_Msk - (uint16_t)1u)

/**@brief       Event object API signature
 */
#define ES_EVENT_SIGNATURE              ((esAtomic)0xdeadfeedul)

/**@brief       Event with identifiers equalt to or higer than this number are
 *              event reserved for local usage.
 */
#define ES_EVENT_LOCAL_ID               32768u

/*------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Event header structure
 * @details     This structure defines mandatory event data. Other data fields
 *              can be enabled/disabled using configuration provided in
 *              @ref event_cfg.h file.
 * @note        You may use @ref CONFIG_EVENT_STRUCT_ATTRIBUTE in
 *              @ref event_cfg.h file to configure additional compiler
 *              directives for this structure.
 * @api
 */
struct CONFIG_EVENT_STRUCT_ATTRIBUTE esEvent {
    uint16_t            id;
/**<@brief Event ID number                                  */
    uint16_t            attrib;
/**<@brief Event dynamic attributes                         */
    struct esMem *      mem;
/**<@brief Event storage                                    */
#if (CONFIG_EVENT_PRODUCER == 1)  || defined(__DOXYGEN__)
    struct esEpa *      producer;
/**<@brief Event producer                                   */
#endif
#if (CONFIG_EVENT_TIMESTAMP == 1) || defined(__DOXYGEN__)
    esSysTimerTick      timestamp;
/**<@brief Event creation time-stamp                        */
#endif
#if (CONFIG_EVENT_SIZE == 1)      || defined(__DOXYGEN__)
    size_t              size;
/**<@brief Event size in bytes                              */
#endif
#if (CONFIG_API_VALIDATION == 1)  || defined(__DOXYGEN__)
    esAtomic            signature;
/**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Event header type
 * @api
 */
typedef struct esEvent esEvent;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Event storage
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Register a memory object to be used for event storage
 * @param       mem
 *              Memory object
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 * @details     Event module can accept up to @ref CONFIG_EVENT_STORAGE_NPOOLS
 *              memory handles for event storage.
 * @api
 */
esError esEventRegisterMem(
    struct esMem *      mem);

/**@brief       Unregister a memory object
 * @param       esPoolMem
 *              Memory object
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 * @api
 */
esError esEventUnregisterMem(
    struct esMem *      mem);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event creation / deletion
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Create an event
 * @param       size
 *              The size of event
 * @param       id
 *              Event identification
 * @param       event
 *              Pointer to event holder. This pointer will hold the address of
 *              newly created event.
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE - No error occurred
 *              - @ref ES_ERROR_NO_REFERENCE - No memory handles available
 *              - @reg ES_ERROR_NO_MEMORY - No available memory storage
 * @api
 */
esError esEventCreate(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event);

/**@brief       Create an event
 * @param       size
 *              The size of event
 * @param       id
 *              Event identification
 * @param       event
 *              Pointer to event holder. This pointer will hold the address of
 *              newly created event.
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE - No error occurred
 *              - @ref ES_ERROR_NO_REFERENCE - No memory handles available
 *              - @reg ES_ERROR_NO_MEMORY - No available memory storage
 * @iclass
 */
esError esEventCreateI(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event);

/**@brief       Destroy an event
 * @param       event
 *              Pointer to the event.
 * @details     Recycles the given event. In case there are no users who are
 *              referencing this event then it will be deleted.
 * @api
 */
esError esEventDestroy(
    struct esEvent *    evt);

/**@brief       Destroy an event
 * @param       event
 *              Pointer to the event.
 * @details     Recycles the given event. In case there are no users who are
 *              referencing this event then it will be deleted.
 * @iclass
 */
esError esEventDestroyI(
    struct esEvent *    event);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event reservation
 * @brief       Event reservation provides methods to freeze events after their
 *              creation. This can be used to speed up the application since
 *              the destroy the event after it has been used.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Reserve the event
 * @param       event
 *              Pointer to the event.
 * @api
 */
void esEventLock(
    esEvent *           event);

/**@brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       event
 *              Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
void esEventUnlock(
    esEvent *           event);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event reference management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Increments the event reference counter
 * @param       event
 *              Pointer to event
 */
static PORT_C_INLINE void esEventRefUp_(
    struct esEvent *    event) {

    if ((event->attrib & ES_EVENT_CONST_Msk) == 0u) {
        uint_fast16_t   count;

        count = event->attrib & (uint16_t)~ES_EVENT_RESERVED_Msk;
        ES_API_REQUIRE_A(ES_API_USAGE, count <= ES_EVENT_REF_LIMIT);
        count++;
        event->attrib &= ES_EVENT_RESERVED_Msk;
        event->attrib |= (uint16_t)count;
    }
}

/**@brief       Decrements the event reference counter
 * @param       event
 *              Pointer to event
 */
static PORT_C_INLINE void esEventReferenceDown_(
    struct esEvent *    event) {

    if ((event->attrib & ES_EVENT_CONST_Msk) == 0u) {
        uint_fast16_t   count;

        count = event->attrib & (uint16_t)~ES_EVENT_RESERVED_Msk;
        ES_API_REQUIRE_A(ES_API_USAGE, count != 0u);
        count--;
        event->attrib &= ES_EVENT_RESERVED_Msk;
        event->attrib |= (uint16_t)count;
    }
}

/**@brief       Returns the event reference counter
 * @param       event
 *              Pointer to event
 */
static PORT_C_INLINE uint_fast16_t esEventRefGet_(
    const struct esEvent * event) {

    return (event->attrib & (uint16_t)~ES_EVENT_RESERVED_Msk);
}

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of event.h
 ******************************************************************************/
#endif /* ES_EVENT_H_ */
