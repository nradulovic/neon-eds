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
 * @brief       Event object header
 * @defgroup    eds_event Event object
 * @brief       Event object
 *********************************************************************//** @{ */

#ifndef NEON_EP_EVENT_H_
#define NEON_EP_EVENT_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stddef.h>

#include "port/compiler.h"
#include "base/config.h"

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
#define NEVENT_ATTR_RESERVED            ((uint16_t)0xff00u | NEVENT_ATTR_DYNAMIC)

/**@brief       Bit mask which defines a constant event
 * @details     When the bits defined in this bit mask are set the given event
 *              is marked as constant. In this case EDS will never try to delete
 *              it. Once the event is marked as constant it never can be deleted.
 * @api
 */
#define NEVENT_ATTR_DYNAMIC             ((uint16_t)0x00ffu)

/**@brief       This macro defines limit for event ID value
 * @details     An event must have ID which is below this limit.
 * @api
 */
#define NEVENT_REF_LIMIT                (UINT16_MAX)

/**@brief       Event object API signature
 */
#define NEVENT_SIGNATURE                ((unsigned int)0xdeadfeedul)

/**@brief       Event with identifiers equal to to or higher than this number
 *              are event reserved for local usage.
 */
#define NEVENT_LOCAL_ID                 32768u

/*------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nmem;
struct nepa;

/**@brief       Event header structure
 * @details     This structure defines mandatory event data. Other data fields
 *              can be enabled/disabled using configuration provided in
 *              @ref event_cfg.h file.
 * @note        You may use @ref CONFIG_EVENT_STRUCT_ATTRIBUTE in
 *              @ref event_cfg.h file to configure additional compiler
 *              directives for this structure.
 * @api
 */
struct CONFIG_EVENT_STRUCT_ATTRIBUTE nevent {
    uint_fast16_t               id;     /**<@brief Event ID number            */
    uint_fast16_t               ref;
    uint_fast16_t               attrib; /**<@brief Event dynamic attributes   */
    struct nmem *               mem;
/**<@brief Event storage                                    */
#if (CONFIG_EVENT_PRODUCER == 1)  || defined(__DOXYGEN__)
                                        /**<@brief Event producer             */
    struct nepa *               producer;
#endif
#if (CONFIG_EVENT_SIZE == 1)      || defined(__DOXYGEN__)
                                        /**<@brief Event size in bytes        */
    size_t                      size;
#endif
#if (CONFIG_API_VALIDATION == 1)  || defined(__DOXYGEN__)
                                        /**<@brief Structure signature, used
                                         * during development stage only.     */
    unsigned int                signature;
#endif
};

/**@brief       Event header type
 * @api
 */
typedef struct nevent nevent;

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
void nevent_register_mem(
    struct nmem *               mem);

/**@brief       Unregister a memory object
 * @param       esPoolMem
 *              Memory object
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 * @api
 */
void nevent_unregister_mem(
    struct nmem *               mem);

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
struct nevent * nevent_create(
    size_t                      size,
    uint16_t                    id);

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
struct nevent * nevent_create_i(
    size_t                      size,
    uint16_t                    id);

/**@brief       Destroy an event
 * @param       event
 *              Pointer to the event.
 * @details     Recycles the given event. In case there are no users who are
 *              referencing this event then it will be deleted.
 * @api
 */
void nevent_destroy(
    const struct nevent *       event);

/**@brief       Destroy an event
 * @param       event
 *              Pointer to the event.
 * @details     Recycles the given event. In case there are no users who are
 *              referencing this event then it will be deleted.
 * @iclass
 */
void nevent_destroy_i(
    const struct nevent *       event);

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
void nevent_lock(
    const struct nevent *       event);

/**@brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       event
 *              Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
void nevent_unlock(
    const struct nevent *       event);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event reference management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Increments the event reference counter
 * @param       event
 *              Pointer to event
 */
PORT_C_INLINE
void nevent_ref_up(
    const struct nevent *       event)
{
    if (event->attrib) {
        ((struct nevent *)event)->ref++;
    }
}

/**@brief       Decrements the event reference counter
 * @param       event
 *              Pointer to event
 */
PORT_C_INLINE
void nevent_ref_down(
    const struct nevent *       event)
{
    if (event->attrib) {
        ((struct nevent *)event)->ref--;
    }
}



PORT_C_INLINE
uint_fast16_t nevent_ref(
    const struct nevent *       event)
{
    return (event->ref | (event->attrib ^ NEVENT_ATTR_DYNAMIC));
}

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of event.h
 ******************************************************************************/
#endif /* NEON_EP_EVENT_H_ */
