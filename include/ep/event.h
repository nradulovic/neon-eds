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
 *              is marked as reserved. In this case EDS will not try to recycle
 *              the event even if there are no EPA units who are referencing it.
 *              When the event is unmarked as reserved EDS will take it into
 *              consideration for recycle process.
 * @api
 */
#define NEVENT_ATTR_RESERVED            ((uint16_t)0xff00u | NEVENT_ATTR_DYNAMIC)

/**@brief       Bit mask which defines a dynamic event
 * @details     When the bits defined in this bit mask are set the given event
 *              is marked as dynamic. In this case EDS will may try to recycle
 *              the event if it is not referenced by any EPA object.
 * @api
 */
#define NEVENT_ATTR_DYNAMIC             ((uint16_t)0x00ffu)

/**@brief       This macro defines limit for event reference counter
 * @api
 */
#define NEVENT_REF_LIMIT                (UINT16_MAX)

/**@brief       Event with identifiers equal to to or higher than this number
 *              are event reserved for local usage.
 * @api
 */
#define NEVENT_LOCAL_ID                 32768u

/**@brief       Validate the pointer to event object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#define N_IS_EVENT_OBJECT(event)                                                \
    (((event) != NULL) && (event->signature == NSIGNATURE_EVENT))

/*------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct nmem;
struct nepa;

/**@brief       Event header structure
 * @details     This structure defines mandatory event data. Other data fields
 *              can be enabled/disabled using configuration options provided in
 *              @ref neon_eds_app_config.h file.
 * @note        You may use @ref CONFIG_EVENT_STRUCT_ATTRIBUTE in
 *              @ref neon_eds_app_config.h file to configure additional compiler
 *              directives for this structure.
 * @api
 */
struct CONFIG_EVENT_STRUCT_ATTRIBUTE nevent {
    uint_fast16_t               id;     /**<@brief Event ID number            */
    uint_fast16_t               ref;    /**<@brief Reference counter          */
    uint_fast16_t               attrib; /**<@brief Dynamic attributes         */
    struct nmem *               mem;    /**<@brief Memory object              */
#if (CONFIG_EVENT_PRODUCER == 1) || defined(__DOXYGEN__)
                                        /**<@brief Event producer             */
    struct nepa *               producer;
#endif
#if (CONFIG_EVENT_SIZE == 1)     || defined(__DOXYGEN__)
                                        /**<@brief Size of event in bytes     */
    size_t                      size;
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
                                        /**<@brief Event signature            */
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
 * @details     Event module can accept up to @ref CONFIG_EVENT_STORAGE_NPOOLS
 *              memory object for event storage.
 * @api
 */
void nevent_register_mem(
    struct nmem *               mem);



/**@brief       Unregister a memory object
 * @param       mem
 *              Memory object
 * @api
 */
void nevent_unregister_mem(
    struct nmem *               mem);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event creation / deletion
 * @{ *//*--------------------------------------------------------------------*/
#define NEVENT_CREATE(type, id)         (type *)nevent_create(sizeof(type), (id))

/**@brief       Create an event
 * @param       size
 *              The size of event in bytes
 * @param       id
 *              Event identification
 * @return      Pointer to new event
 *              - @retval NULL - No available memory storage
 * @api
 */
struct nevent * nevent_create(
    size_t                      size,
    uint16_t                    id);



/**@brief       Create an event
 * @param       size
 *              The size of event in bytes
 * @param       id
 *              Event identification
 * @return      Pointer to new event
 * @retval      - NULL - No available memory storage
 * @iclass
 */
struct nevent * nevent_create_i(
    size_t                      size,
    uint16_t                    id);



/**@brief       Create an event from specified memory instance
 * @param       mem
 *              Pointer to memory instance
 * @param       size
 *              Size of event in bytes
 * @param       id
 *              Event id.
 * @return      Pointer to new event
 * @retval      - NULL - No available memory storage
 * @iclass
 */
struct nevent * nevent_create_from_i(
    struct nmem *               mem,
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



struct nevent * nevent_forward(
    const struct nevent *       event,
    uint16_t                    id);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event reservation
 * @brief       Event reservation provides methods to freeze events after their
 *              creation. This can be used to speed up the application since
 *              the destroy the event after it has been used.
 * @{ *//*--------------------------------------------------------------------*/


/**@brief       Lock the event from recycling process
 * @param       event
 *              Pointer to the event.
 * @api
 */
void nevent_lock(
    const struct nevent *       event);



/**@brief       Release the event lock and possibly recycle the event
 * @param       event
 *              Pointer to the event.
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



/**@brief       Returns the reference counter value of a dynamic event
 * @note        If a constant event is given then the returned value will be a
 *              non-zero value in order to prevent event deletion.
 * @note        If a reserved dynamic event is given the the returned value will
 *              be a non-zero value in order to prevent event deletion.
 * @api
 */
PORT_C_INLINE
uint_fast16_t nevent_ref(
    const struct nevent *       event)
{
    return (event->ref | (event->attrib ^ NEVENT_ATTR_DYNAMIC));
}



/**@brief       Cast event pointer to void pointer
 * @details     This is only a convenience function which will shorted the user
 *              code a bit.
 * @api
 */
PORT_C_INLINE
const void * nevent_data(const struct nevent * event)
{
	return ((const void *)event);
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
