/**
 * @file orz_endian.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-10-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "orz_endian.h"


#if !defined(__ORZ_BYTE_ENDIAN__)
#   define __HTON(type, suffix) \
type orz_hton ## suffix(type suffix) \
{ \
    type val = 0; \
    const uint8_t *hex = (const uint8_t *)&suffix; \
    for (int idx = 0; idx < sizeof(type); ++idx) { val <<= 8; val |= hex[idx]; } \
    return val; \
}

__HTON(uint16_t, s)
__HTON(uint32_t, l)
__HTON(uint64_t, x)

#elif (__ORZ_BYTE_ENDIAN__ == __ORZ_LE__)
uint16_t orz_htons(uint16_t s)
{
    return (s << 8) | (s >> 8);
}

uint32_t orz_htonl(uint32_t l)
{
    return (orz_htons(l) << 16) | orz_htons(l >> 16);
}

uint64_t orz_htonx(uint64_t x)
{
    return ((uint64_t)orz_htonl(x) << 32) | orz_htonl(x >> 32);
}

#elif (__ORZ_BYTE_ENDIAN__ == __ORZ_BE__)
uint16_t orz_htons(uint16_t s)
{
    return s;
}

uint32_t orz_htonl(uint32_t l)
{
    return l;
}

uint64_t orz_htonx(uint64_t x)
{
    return x;
}

#else
#   error "__ORZ_BYTE_ENDIAN__ define Error!"
#endif


uint16_t orz_ntohs(uint16_t s)
{
    return orz_htons(s);
}

uint32_t orz_ntohl(uint32_t l)
{
    return orz_htonl(l);
}

uint64_t orz_ntohx(uint64_t x)
{
    return orz_htonx(x);
}

