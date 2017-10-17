/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2017 Nenad Radulovic
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
 * @brief       Virtual timer header
 * @defgroup    base_vtimer Virtual timer
 * @brief       Virtual timer
 *********************************************************************//** @{ */

#ifndef NEON_TIMER_TIMER_H_
#define NEON_TIMER_TIMER_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

#include "base/dlist.h"
#include "base/config.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Validate the pointer to Event Processing Agent object
 * @note        This macro may be used only when @ref CONFIG_API_VALIDATION
 *              macro is enabled.
 * @api
 */
#if (CONFIG_API_VALIDATION == 1)
#define N_IS_TIMER_OBJECT(timer_obj)                                            \
    (NSIGNATURE_OF(timer_obj) == NSIGNATURE_TIMER)
#else
#define N_IS_TIMER_OBJECT(timer_obj)    (timer_obj)
#endif

/**@brief       Convert time (given in seconds) into core timer ticks
 * @param       time_sec
 *              Time in seconds
 * @return      The number of core timer ticks.
 * @api
 */
#define NTIMER_SEC(time_sec)                                                    \
    ((uint32_t)((uint32_t)(time_sec) *                                   		\
        (uint32_t)CONFIG_CORE_TIMER_EVENT_FREQ))

/**@brief       Convert time (given in milliseconds) into core timer ticks
 * @param       time_ms
 *              Time in milliseconds
 * @return      The number of core timer ticks.
 * @note        The result is rounded upward to the next core tick boundary.
 * @api
 */
#define NTIMER_MS(time_ms)                                                      \
    ((uint32_t)(((uint32_t)(time_ms) *                                   		\
        (uint32_t)CONFIG_CORE_TIMER_EVENT_FREQ + 999ul) / 1000ul))

/**@brief       Convert time (given in microseconds) into core timer ticks
 * @param       time_us
 *              Time in microseconds
 * @return      The number of core timer ticks.
 * @note        The result is rounded upward to the next core tick boundary.
 * @api
 */
#define NTIMER_US(time_us)                                                      \
    ((uint32_t)(((uint32_t)(time_us) *                                   		\
        (uint32_t)CONFIG_CORE_TIMER_EVENT_FREQ + 999999ul) / 1000000ul))

/**@brief       Timer attribute: one shot
 * @details     When this attribute is specified then the timer will fire up
 *              once and then stop.
 * @api
 */
#define NTIMER_ATTR_ONE_SHOT            (0x1u << 0)

/**@brief       Timer attribute: repeat
 * @details     When this attribute is specified then the timer will
 *              fire up until it is explicitly stopped with ntimer_cancel().
 * @api
 */
#define NTIMER_ATTR_REPEAT              (0x1u << 1)

/**
 * @brief       Helper macro for timer signature
 * @notapi
 */
#if (CONFIG_API_VALIDATION == 1) || defined(__DOXYGEN__)
#define NP_TIMER_SIGNATURE_INIT               NSIGNATURE_TIMER,
#else
#define NP_TIMER_SIGNATURE_INIT
#endif

#define NTIMER_INITIALIZER(name)										     	\
	{																			\
		NSIGNATURE_INITIALIZER(NSIGNATURE_TIMER)								\
		NDLIST_INITIALIZER((name).list),									    \
		0,																		\
		0,																		\
		NULL,																	\
		NULL																	\
	}

#define NTIMER_DEFINE(name)														\
	struct ntimer name = NTIMER_INITIALIZER(name)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Virtual timer structure
 * @details     All elements of this structure are private members. This
 *              implementation detail is only exposed so the structure can be
 *              allocated on stack.
 * @api
 */
struct ntimer
{
	NSIGNATURE_DECLARE								/**<@brief Debug signature*/
    struct ndlist               list;               /**<@brief Linked list    */
    uint32_t                    rtick;              /**<@brief Relative ticks */
    uint32_t                    itick;              /**<@brief Initial ticks  */
    void                     (* fn)(void *);        /**<@brief Callback       */
    void *                      arg;                /**<@brief Argument       */
};

/**@brief       Virtual timer type
 * @api
 */
typedef struct ntimer ntimer;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/


#if (CONFIG_API_VALIDATION == 1)
/**@brief       Initialize the timer structure
 * @details     This function will initialize the timer structure. When
 *              @ref CONFIG_API_VALIDATION is enabled it will make the structure
 *              valid before using other timer functions. Also, the function
 *              will raise assert if you try to initialize already initialized
 *              timer.
 * @param       timer
 *              Pointer to timer structure
 * @api
 */
void ntimer_init(struct ntimer * timer);
#else
#define ntimer_init(timer_obj)              									\
	ndlist_init(&(timer_obj)->list)
#endif



/**@brief       Start a timer
 * @param       timer
 *              Pointer to timer structure
 * @param       tick
 *              Number of ticks to run
 * @param       fn
 *              Pointer to callback function
 * @param       arg
 *              Argument for callback function
 * @iclass
 * @api
 */
void ntimer_start_i(struct ntimer * timer, uint32_t tick, void (* fn)(void *),
		void * arg, uint8_t flags);



/**@brief       Start a timer
 * @param       timer
 *              Pointer to timer structure
 * @param       tick
 *              Number of ticks to run
 * @param       fn
 *              Pointer to callback function
 * @param       arg
 *              Argument for callback function
 * @api
 */
void ntimer_start(struct ntimer * timer, uint32_t tick, void (* fn)(void *),
		void * arg, uint8_t flags);



/**@brief       Terminate a timer
 * @param       timer
 *              Pointer to timer structure
 * @iclass
 */
void ntimer_cancel_i(struct ntimer * timer);



/**@brief       Terminate a timer
 * @param       timer
 *              Pointer to timer structure
 * @api
 */
void ntimer_cancel(struct ntimer * timer);



/**@brief       Is a timer still running?
 * @param       timer
 *              Pointer to timer structure
 * @return      Timer state
 *  @retval     TRUE - the timer is still running
 *  @retval     FALSE - timer has finished running
 * @iclass
 */
bool ntimer_is_running_i(const struct ntimer * timer);


/**@brief       Returns the numbers if ticks that is remaining before the timer
 *              fires up
 * @param       timer
 *              Pointer to timer structure
 * @return      Return the number of ticks remaining. If the is not running the
 *              return value is 0.
 * @api
 */
uint32_t ntimer_remaining(const struct ntimer * timer);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of timer.h
 ******************************************************************************/
#endif /* NEON_TIMER_TIMER_H_ */
