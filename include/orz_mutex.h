/**
 * @file orz_mutex.h
 * @author Orz
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-01-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __ORZ_MUTEX_H__
#define __ORZ_MUTEX_H__

#include "orz_cdefs.h"
#include "orz_types.h"

struct orz_mutex_ops {
    int (*create)(void **);
    int (*destroy)(void *);
    int (*lock)(void *);
    int (*unlock)(void *);
};

__ORZ_C_ENTER__

int orz_mutex_init(const struct orz_mutex_ops *ops);

int orz_mutex_deinit(void);

int orz_mutex_create(void **mutex);

int orz_mutex_destroy(void *mutex);

int orz_mutex_lock(void *mutex);

int orz_mutex_unlock(void *mutex);

__ORZ_C_LEAVE__


#endif //__ORZ_MUTEX_H__
