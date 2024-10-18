/**
 * @file orz_endian.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-10-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_ENDIAN_H__
#define __ORZ_ENDIAN_H__

#include "orz_cdefs.h"
#include "orz_types.h"


#define __ORZ_LE__  1
#define __ORZ_BE__  2
#define __ORZ_BYTE_ENDIAN__     __ORZ_LE__


__ORZ_C_ENTER__


uint16_t orz_htons(uint16_t s);
uint16_t orz_ntohs(uint16_t s);

uint32_t orz_htonl(uint32_t l);
uint32_t orz_ntohl(uint32_t l);

uint64_t orz_htonx(uint64_t x);
uint64_t orz_ntohx(uint64_t x);


__ORZ_C_LEAVE__



#endif //__ORZ_ENDIAN_H__

