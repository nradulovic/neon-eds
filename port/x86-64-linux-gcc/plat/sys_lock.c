/*
 * sys_lock.c
 *
 *  Created on: 14 Dec 2014
 *      Author: nenad
 */


#include "port/sys_lock.h"

pthread_mutex_t g_global_lock = PTHREAD_MUTEX_INITIALIZER;
