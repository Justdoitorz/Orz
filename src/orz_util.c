/**
 * @file orz_util.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "orz_util.h"

static 
bool __allsame(const uint8_t *hex, uint32_t len)
{
    for (uint32_t idx = 1; idx < len; ++idx) {
        if (hex[0] != hex[idx]) {
            return false;
        }
    }

    return true;
}

__ORZ_EXPORT
int orz_allsame(const void *hex, uint32_t len)
{
    if (NULL == hex) return -1;
    if (0 == len) return -2;

    return __allsame((const uint8_t *)hex, len);
}

__ORZ_EXPORT
int orz_allsame_value(const void *hex, uint32_t len, uint8_t value)
{
    if (NULL == hex) return -1;
    if (0 == len) return -2;

    if (*(const uint8_t *)hex != value) 
        return false;

    return __allsame((const uint8_t *)hex, len);
}

static 
uint32_t __str2hex(uint8_t *hex, const char *str, uint32_t slen)
{
    uint8_t * const begin = hex;

    for (uint32_t idx = 0; idx < slen; idx += 2) {
        *hex++ = mkbyte(c2h(str[idx]), c2h(str[idx + 1]));
    }

    return hex - begin;
}

__ORZ_EXPORT
int orz_str2hex(uint8_t *hex, uint32_t hlen, const char *str, uint32_t slen)
{
    if (NULL == hex || NULL == str) return -1;
    if ((slen & 1) != 0) return -2;
    if (slen > hlen * 2) return -3;

    return __str2hex(hex, str, slen);
}

