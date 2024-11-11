/**
 * @file orz_oneof.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_ONEOF_H__
#define __ORZ_ONEOF_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_util.h"


#define ORZ_ONEOF_EASY(entry, idx)                          oneof_easy(ORZ_ENTRY_EXPAND3(entry), idx, NULL)
#define ORZ_ONEOF_DFT(entry, idx, dft)                      oneof_easy(ORZ_ENTRY_EXPAND3(entry), idx, dft)
#define ORZ_ONEOF_EX(entry, nmemb, size, idx, dft)          oneof_easy(entry, nmemb, size, idx, NULL)

/**
 * @brief For the freedom!
 *
 * @param[in] entry array's entry
 * @param nmemb array's length
 * @param size size of a element
 * @param idx index of array
 * @param dft default pointer
 * @return const void*
 */
static inline
const void *oneof_easy(const void *entry, uint32_t nmemb, uint32_t size, uint32_t idx, const void *dft)
{
    return idx < nmemb && entry ? tovptr(entry, size, idx) : dft;
}


__ORZ_C_ENTER__


__ORZ_C_LEAVE__



#endif //__ORZ_ONEOF_H__

