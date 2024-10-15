/**
 * @file orz_algorithm.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-10-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_ALGORITHM_H__
#define __ORZ_ALGORITHM_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_util.h"

#define ORZ_COMMON_SEARCH(entry, comp, rhs) \
    orz_common_search(ORZ_ENTRY_EXPAND3(entry), comp, rhs)
#define ORZ_BINARY_SEARCH(entry, comp, rhs) \
    orz_binary_search(ORZ_ENTRY_EXPAND3(entry), comp, rhs)

/**
 * @brief Compare order 
 * 
 * @param[in] lhs entry's element
 * @param[in] rhs target's key
 * @retval < 0, lhs < rhs
 * @retval == 0, lhs == rhs
 * @retval > 0, lhs > rhs
 */
typedef int (*fn_comp)(const void *lhs, const void *rhs);

__ORZ_C_ENTER__

/**
 * @brief common search
 * 
 * @param[in] entry array's entry
 * @param nmemb array's length
 * @param size size of a element
 * @param comp compare function
 * @param[in] rhs target's key
 * @retval < 0, error or not found
 * @retval [0,nmemb), found index
 */
int orz_common_search(const void *entry, uint32_t nmemb, uint32_t size, fn_comp comp, const void *rhs);


/**
 * @brief binary search
 * 
 * @param[in] entry array's entry
 * @param nmemb array's length
 * @param size size of a element
 * @param comp compare function
 * @param[in] rhs target's key
 * @retval < 0, error or not found
 * @retval [0,nmemb), found index
 */
int orz_binary_search(const void *entry, uint32_t nmemb, uint32_t size, fn_comp comp, const void *rhs);


__ORZ_C_LEAVE__


#endif //__ORZ_ALGORITHM_H__

