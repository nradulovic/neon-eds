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
 * @brief       Linked lists header
 * @defgroup    base_list Linked lists
 * @brief       Linked lists
 *********************************************************************//** @{ */

#ifndef ES_LIST_H_
#define ES_LIST_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdbool.h>

#include "plat/compiler.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Singly linked list with Sentinel
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Get the entry node pointer
 * @param       typeOfNode
 *              Type of node entry
 * @param       node
 *              Pointer to the node
 * @param       list
 *              Type of list
 * @api
 */
#define ES_SLS_NODE_ENTRY(typeOfNode, list, node)                               \
    ((typeOfNode *)((char *)(node) - offsetof(typeOfNode, list)))

/** @} *//*---------------------------------------------------------------*//**
 * @name        Doubly Linked list with Sentinel
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Get the entry node pointer
 * @param       typeOfNode
 *              Type of node entry
 * @param       node
 *              Pointer to the node
 * @param       list
 *              Type of list
 * @api
 */
#define ES_DLS_NODE_ENTRY(typeOfNode, list, node)                               \
    ((typeOfNode *)((char *)(node) - offsetof(typeOfNode, list)))

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Singly linked list with Sentinel
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Singly linked list structure
 */
struct esSls {
    struct esSls *      next;                                                   /**< @brief Next member in linked list                      */
};

/**@brief       Singly linked list type
 */
typedef struct esSls esSls;

/** @} *//*---------------------------------------------------------------*//**
 * @name        Doubly Linked list with Sentinel
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Doubly linked list structure
 */
struct esDls {
    struct esDls *      next;                                                   /**< @brief Next member in linked list                      */
    struct esDls *      prev;                                                   /**< @brief Previous member in linked list                  */
};

/**@brief       Doubly linked list type
 */
typedef struct esDls esDls;

/** @} *//*-------------------------------------------------------------------*/

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Singly linked list with Sentinel
 * @{ *//*--------------------------------------------------------------------*/

static PORT_C_INLINE void esSlsNodeInit(
    esSls *             node) {

    node->next = node;
}

static PORT_C_INLINE void esSlsSentinelInit(
    esSls *             sentinel) {

    esSlsNodeInit(
        sentinel);
}

static PORT_C_INLINE void esSlsNodeAdd(
    esSls *             newNode,
    esSls *             prevNode,
    esSls *             nextNode) {

    newNode->next = nextNode;
    prevNode->next = newNode;
}

static PORT_C_INLINE void esSlsNodeAddAfter(
    esSls *             currNode,
    esSls *             node) {

    esSlsNodeAdd(
        node,
        currNode,
        currNode->next);
}

static PORT_C_INLINE void esSlsNodeAddHead(
    esSls *             sentinel,
    esSls *             node) {

    node->next = sentinel->next;
    sentinel->next = node;
}

static PORT_C_INLINE void esSlsNodeRm(
    esSls *             sentinel,
    esSls *             node) {

    struct esSls **     head;
    head = &sentinel;

    while ((*head)->next != node) {
        *head = (*head)->next;
    }
    (*head)->next = node->next;
}

static PORT_C_INLINE void esSlsNodeRmAfter(
    esSls *             node) {

    node->next = node->next->next;
}

static PORT_C_INLINE bool esSlsIsEmpty(
    esSls *             sentinel) {

    if (sentinel->next != sentinel) {

        return (false);
    } else {

        return (true);
    }
}

/** @} *//*---------------------------------------------------------------*//**
 * @name        Doubly Linked list with Sentinel
 * @{ *//*--------------------------------------------------------------------*/

static PORT_C_INLINE void esDlsNodeInit(
    esDls *             node) {

    node->next = node;
    node->prev = node;
}

static PORT_C_INLINE void esDlsSentinelInit(
    esDls *             sentinel) {

    esDlsNodeInit(
        sentinel);
}

static PORT_C_INLINE void esDlsNodeAdd(
    esDls *             newNode,
    esDls *             prevNode,
    esDls *             nextNode) {

    newNode->next = nextNode;
    newNode->prev = prevNode;
    prevNode->next = newNode;
    nextNode->prev = newNode;
}

static PORT_C_INLINE void esDlsNodeAddHead(
    esDls *             sentinel,
    esDls *             newNode) {

    esDlsNodeAdd(
        newNode,
        sentinel,
        sentinel->next);
}

static PORT_C_INLINE void esDlsNodeAddTail(
    esDls *             sentinel,
    esDls *             newNode) {

    esDlsNodeAdd(
        newNode,
        sentinel->prev,
        sentinel);
}

static PORT_C_INLINE void esDlsNodeAddBefore(
    esDls *             currNode,
    esDls *             newNode) {

    esDlsNodeAdd(
        newNode,
        currNode->prev,
        currNode);
}

static PORT_C_INLINE void esDlsNodeAddAfter(
    esDls *             currNode,
    esDls *             newNode) {

    esDlsNodeAdd(
        newNode,
        currNode,
        currNode->next);
}

static PORT_C_INLINE void esDlsNodeRm(
    esDls *             oldNode) {

    oldNode->next->prev = oldNode->prev;
    oldNode->prev->next = oldNode->next;
}

static PORT_C_INLINE bool esDlsIsEmpty(
    esDls *             sentinel) {

    if (sentinel->next != sentinel) {

        return (false);
    } else {

        return (true);
    }
}

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of list.h
 ******************************************************************************/
#endif /* ES_LIST_H_ */
