/**
 * @file orz_algorithm.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-10-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "orz_algorithm.h"
#include "orz_retval.h"


__ORZ_EXPORT
int orz_common_search(const void *entry, uint32_t nmemb, uint32_t size, fn_comp comp, const void *rhs)
{
    if (NULL == entry || NULL == comp)
        return ORZ_RETVAL_ERROR(MOD_ALGORITHM, RSN_NULLPTR, 0);
    
    for (uint32_t idx = 0; idx < nmemb; ++idx) {
        if (comp(tovptr(entry, size, idx), rhs) == 0) {
            return idx;
        }
    }

    return RETVAL_NOT_FOUND;
}

__ORZ_EXPORT
int orz_binary_search(const void *entry, uint32_t nmemb, uint32_t size, fn_comp comp, const void *rhs)
{
    if (NULL == entry || NULL == comp)
        return ORZ_RETVAL_ERROR(MOD_ALGORITHM, RSN_NULLPTR, 1);

    int left = 0, right = nmemb - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        int ret = comp(tovptr(entry, size, mid), rhs);

        if (ret < 0) left = mid + 1;
        else if (ret > 0) right = mid - 1;
        else return mid;
    }
       
    return RETVAL_NOT_FOUND;
}

