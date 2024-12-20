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
#include "orz_retval.h"

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
    if (NULL == hex) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 0);
    if (0 == len) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_ZERO_LENGTH, 0);

    return __allsame((const uint8_t *)hex, len);
}

__ORZ_EXPORT
int orz_allsame_value(const void *hex, uint32_t len, uint8_t value)
{
    if (NULL == hex) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 1);
    if (0 == len) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_ZERO_LENGTH, 1);

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
    if (NULL == hex || NULL == str) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 2);
    if ((slen & 1) != 0) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_WRONG_LENGTH, 2);
    if (slen > hlen * 2) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_SPACE_NO_ENOUGH, 2);

    return __str2hex(hex, str, slen);
}

typedef uint32_t (*fn_hexenc)(char *out, uint8_t byte, const char *sep);

static
uint32_t __hex2out(fn_hexenc encoder, char *out, const uint8_t *hex, uint32_t hlen, const char *sep)
{
    char * const begin = out;

    for (uint32_t idx = 0; idx < hlen; ++idx) {
        out += encoder(out, hex[idx], sep);
    }

    if (sep && hlen) --out;

    *out = '\0';

    return out - begin;
}


static
uint32_t __byte2str(char *str, uint8_t byte, const char *sep)
{
    char * const begin = str;

    *str++ = h2c(byte >> 4);
    *str++ = h2c(byte >> 0);

    if (sep) *str++ = *sep;

    return str - begin;
}

static
uint32_t __hex2str(char *str, const uint8_t *hex, uint32_t hlen, const char *sep)
{
    return __hex2out(__byte2str, str, hex, hlen, sep);
}

__ORZ_EXPORT
int orz_hex2str(char *str, uint32_t slen, const void *hex, uint32_t hlen)
{
    if (NULL == str || NULL == hex) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 3);
    if (slen <= hlen * 2) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_SPACE_NO_ENOUGH, 3);

    return __hex2str(str, (const uint8_t *)hex, hlen, NULL);
}

__ORZ_EXPORT
int orz_hex2str_separate(char *str, uint32_t slen, const void *hex, uint32_t hlen, char sep)
{
    if (NULL == str || NULL == hex) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 4);
    if (slen < hlen * 3) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_SPACE_NO_ENOUGH, 4);

    return __hex2str(str, (const uint8_t *)hex, hlen, &sep);
}

static
uint32_t __byte2bits(char *bits, uint8_t byte, const char *sep)
{
    char * const begin = bits;

    for (int idx = 7; idx >= 0; --idx) {
        *bits++ = h2c((byte >> idx) & 1);
    }

    if (sep) *bits++ = *sep;

    return bits - begin;
}

static
uint32_t __hex2bits(char *bits, const uint8_t *hex, uint32_t hlen, char *sep)
{
    return __hex2out(__byte2bits, bits, hex, hlen, sep);
}

__ORZ_EXPORT
int orz_hex2bits(char *bits, uint32_t blen, const void *hex, uint32_t hlen)
{
    if (NULL == bits || NULL == hex) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 5);
    if (blen <= hlen * 8) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_SPACE_NO_ENOUGH, 5);

    return __hex2bits(bits, (const uint8_t *)hex, hlen, NULL);
}

__ORZ_EXPORT
int orz_hex2bits_separate(char *bits, uint32_t blen, const void *hex, uint32_t hlen, char sep)
{
    if (NULL == bits || NULL == hex) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_NULLPTR, 6);
    if (blen < hlen * 9) return ORZ_RETVAL_ERROR(MOD_UTIL, RSN_SPACE_NO_ENOUGH, 6);

    return __hex2bits(bits, (const uint8_t *)hex, hlen, &sep);
}

__ORZ_EXPORT
int orz_bin_1bits(uint64_t value)
{
    int bits = 0;

    while (value) {
        value &= value - 1;
        ++bits;
    }

    return bits;
}

