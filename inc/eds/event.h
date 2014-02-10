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
#include "mem/pool.h"
#include "eds/event_config.h"

/*===============================================================  DEFINES  ==*/

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
 *              it. Once the event is marked as constant it never can be deleted.
 * @api
 */
#define ES_EVENT_CONST_Msk              ((uint16_t)(0x01u << 14))

/**@brief       This macro defines limit for event ID value
 * @details     An event must have ID which is below this limit.
 * @api
 */
#define ES_EVENT_REF_LIMIT               (ES_EVENT_CONST_Msk - (uint16_t)1u)

#define ES_EVENT_SIGNATURE                 ((esAtomic)0xdeadfeedul)

/*===============================================================  MACRO's  ==*/

/**@brief       Helper macro to create new event
 * @param       type
 *              Type of event to create
 * @param       id
 *              Event identification
 * @api
 */
#define ES_EVENT_CREATE(type, id)                                               \
    (type)esEventCreate(sizeof(type), (uint16_t)id)

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
CONFIG_EVENT_STRUCT_ATTRIBUTE struct esEvent {
    uint16_t            id;                                                     /**<@brief Event ID number                                  */
    uint16_t            attrib;                                                 /**<@brief Event dynamic attributes                         */
#if (CONFIG_EVENT_STORAGE == 1)        || defined(__DOXYGEN__)
    struct esPoolMem *  poolMem;                                                /**<@brief Event pool storage                               */
#endif
#if (CONFIG_EVENT_PRODUCER == 1)       || defined(__DOXYGEN__)
    struct esEpa *      producer;                                               /**<@brief Event producer                                   */
#endif
#if (CONFIG_EVENT_TIMESTAMP == 1)      || defined(__DOXYGEN__)
    esSysTimerTick      timestamp;                                              /**<@brief Event creation time-stamp                        */
#endif
#if (CONFIG_EVENT_SIZE == 1)           || defined(__DOXYGEN__)
    size_t              size;                                                   /**<@brief Event size in bytes                              */
#endif
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
    esAtomic            signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Event header type
 * @api
 */
typedef struct esEvent esEvent;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Event pools
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Register a pool to be used for event storage
 * @param       esPoolMem
 *              Pool memory handle
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 * @api
 */
esError esEventPoolRegister(
    struct esPoolMem *  esPoolMem);

/**@brief       Unregister a memory pool
 * @param       esPoolMem
 *              Pool memory handle
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 * @api
 */
esError esEventPoolUnregister(
    struct esPoolMem *  esPoolMem);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event creation / deletion
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Create an event
 * @param       size
 *              The size of event
 * @param       id
 *              Event identification
 * @param       Pointer to event holder
 *              This pointer will hold the address of newly created event.
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 *              - @ref ES_ERROR_NO_RESOURCE - No memory handles available
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
 * @param       Pointer to event holder
 *              This pointer will hold the address of newly created event.
 * @return      eSolid standard error:
 *              - @ref ES_ERROR_NONE
 *              - @ref ES_ERROR_NO_RESOURCE - No memory handles available
 *              - @reg ES_ERROR_NO_MEMORY - No available memory storage
 * @iclass
 */
esError esEventCreateI(
    size_t              size,
    uint16_t            id,
    struct esEvent **   event);

/**@brief       Unistava dogadjaj.
 * @param       event
 *              Pokazivac na dogadjaj koji treba da se unisti.
 * @details     Ukoliko dati @c event dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @api
 */
esError esEventDestroy(
    struct esEvent *    evt);

/**@brief       Unistava dogadjaj.
 * @param       event
 *              Pokazivac na dogadjaj koji treba da se unisti.
 * @details     Ukoliko dati @c event dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @iclass
 */
esError esEventDestroyI(
    struct esEvent *    event);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Rad sa staticnim dogadjajima
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Rezervise dogadjaj @c event.
 * @param       event
 *              Pokazivac na dogadjaj koji se rezervise.
 * @pre         Dogadjaj mora da bude kreiran funkcijom esEventCreate().
 * @details     Rezervise @c event dogadjaj cime se onemogucava eSolid-u da
 *              izvrsi recikliranje memorijskog prostora ovog dogadjaja.
 *              Uglavnom se rezervacija dogadjaja vrsi kada je potrebno iz neke
 *              prekidne rutine brzo poslati dogadjaj nekom aktivnom EPA objektu.
 *              Prethodnim kreiranjem dogadjaja i njegovom rezervacijom se vrsi
 *              alociranje memorije, tako da u kriticnim trenucima nije potrebno
 *              ponovo izvrsiti alociranje memorije, vec se spreman dogadjaj
 *              odmah koristi.
 * @api
 */
void esEventReserve(
    esEvent *           event);

/**@brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       event
 *              Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
void esEventUnReserve(
    esEvent *           event);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event reference management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Increments the event reference counter
 * @param       event
 *              Pointer to event structure
 */
static PORT_C_INLINE_ALWAYS void esEventRefUp(
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
 *              Pointer to event structure
 */
static PORT_C_INLINE_ALWAYS void esEventReferenceDown(
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

static PORT_C_INLINE uint_fast16_t esEventRefGet(
    const struct esEvent * event) {

    return (event->attrib & (uint16_t)~ES_EVENT_RESERVED_Msk);
}


extern struct esEpa * appEvtGeneratorGet(void);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of event.h
 ******************************************************************************/
#endif /* ES_EVENT_H_ */
