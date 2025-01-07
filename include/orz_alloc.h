/**
 * @file orz_alloc.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-01-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __ORZ_ALLOC_H__
#define __ORZ_ALLOC_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_retval.h"

//Don't use -DUSE_MACRO_ALLOC, because user can't know
// #define USE_MACRO_ALLOC


#ifdef USE_MACRO_ALLOC
#define orz_alloc_init(...)    (RETVAL_OK)
#define orz_alloc_deinit()      (RETVAL_OK)

//replace 'include header, macro malloc & free' if you want
#include <stdlib.h>
#define orz_malloc(size)        malloc(size)
#define orz_free(p)             free(p)

#else 

__ORZ_C_ENTER__

/**
 * @brief Initialize the memory allocator
 * 
 * @param usr_malloc user-defined malloc function
 * @param usr_free user-defined free function
 * @retval < 0, error
 * @retval 0, success
 */
int orz_alloc_init(void *(*usr_malloc)(uint32_t), void (*usr_free)(void *));

/**
 * @brief Deinitialize the memory allocator
 * 
 * @retval 0, success
 */
int orz_alloc_deinit(void);

/**
 * @brief Allocate memory
 * 
 * @param size memory size
 * @return void* 
 */
void *orz_malloc(uint32_t size);

/**
 * @brief Free allocated memory
 * 
 * @param p pointer to the memory to be freed
 */
void orz_free(void *p);

__ORZ_C_LEAVE__

#endif //USE_MACRO_ALLOC


#endif //__ORZ_ALLOC_H__
