/**
 * @file orz_alloc.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-01-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "orz_alloc.h"

#ifndef USE_MACRO_ALLOC

#include <stdlib.h>


static void *default_malloc(uint32_t size)
{
    return malloc(size);
}

static void default_free(void *p)
{
    free(p);
}

static void *(*global_malloc)(uint32_t) = default_malloc; 
static void (*global_free)(void *) = default_free;

__ORZ_EXPORT
int orz_alloc_init(void *(*usr_malloc)(uint32_t), void (*usr_free)(void *))
{
    if (usr_malloc == NULL || usr_free == NULL) 
        return ORZ_RETVAL_ERROR(MOD_ALLOC, RSN_NULLPTR, 0);

    global_malloc = usr_malloc;
    global_free = usr_free;

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_alloc_deinit(void)
{
    global_malloc = default_malloc;
    global_free = default_free;

    return RETVAL_OK;
}

__ORZ_EXPORT
void *orz_malloc(uint32_t size)
{
    return global_malloc(size);
}

__ORZ_EXPORT
void orz_free(void *p)
{
    global_free(p);
}

#endif
