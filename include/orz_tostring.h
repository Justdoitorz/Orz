/**
 * @file orz_tostring.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_TOSTRING_H__
#define __ORZ_TOSTRING_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_oneof.h"

#define TOSTRING_DFT    "Invalid"

#define ORZ_TOSTRING_EASY(entry, idx)                   tostring_easy(ORZ_ENTRY_EXPAND3(entry), idx, TOSTRING_DFT) 
#define ORZ_TOSTRING_DFT(entry, idx, dft)               tostring_easy(ORZ_ENTRY_EXPAND3(entry), idx, dft) 
#define ORZ_TOSTRING_EX(entry, nmemb, size, idx, dft)   tostring_easy(entry, nmemb, size, idx, dft)

/**
 * @brief For the freedom!
 *
 * @param[in] entry array's entry
 * @param nmemb array's length
 * @param size size of a element
 * @param idx index of array
 * @param dft default string
 * @return const char*
 */
static inline 
const char *tostring_easy(const char * const *entry, uint32_t nmemb, uint32_t size, uint32_t idx, const char *dft)
{
    return *(const char **)oneof_easy(entry, nmemb, size, idx, &dft);
}



__ORZ_C_ENTER__

/**
 * @brief get boolean string from integer
 *
 * @param xbool boolean value, not 0 is true
 * @retval == 0, "False"
 * @retval != 0, "True"
 */
const char *orz_tostring_bool(uint32_t xbool);

__ORZ_C_LEAVE__



#endif //__ORZ_TOSTRING_H__

