/**
 * @file orz_util.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_UTIL_H__
#define __ORZ_UTIL_H__


#include "orz_cdefs.h"
#include "orz_types.h"

#define ORZ_TSTBIT(num, bit)    ((num) & (1ULL << (bit)))
#define ORZ_SETBIT(num, bit)    ((num) | (1ULL << (bit)))
#define ORZ_CLRBIT(num, bit)    ((num) &~(1ULL << (bit)))
#define ORZ_TOGBIT(num, bit)    ((num) ^ (1ULL << (bit)))

#define ORZ_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define ORZ_MIN(a, b)           ((a) < (b) ? (a) : (b))
#define ORZ_MAX3(a, b, c)       ORZ_MAX(ORZ_MAX(a, b), c) 
#define ORZ_MIN3(a, b, c)       ORZ_MIN(ORZ_MIN(a, b), c) 

#define ORZ_ALIGNUP(x, a)       ((((x) + (a) - 1) / (a)) * (a))
#define ORZ_ALIGNDOWN(x, a)     (((x) / (a)) * (a))
#define ORZ_ALIGNUP2(x, a)      (((x) + (a) - 1) & ~((a) - 1))
#define ORZ_ALIGNDOWN2(x, a)    ((x) & ~((a) - 1))

#define ORZ_POW2_N(n)           (1ULL << (n))
#define ORZ_ARRLEN(array)       ((sizeof(array)/sizeof(0[array])))

#define ORZ_ENTRY_EXPAND3(entry)    entry, ORZ_ARRLEN(entry), sizeof(0[entry])

/**
 * @brief Convert hexadecimal values to characters
 * 
 * @param h expect:[0, 16)
 * @retval ['0', '9'], ['A', 'F']
 */
static inline char h2c(uint8_t h)
{
    return "0123456789ABCDEF"[h & 0x0F];
}

/**
 * @brief Convert characters to hexadecimal values
 * 
 * @param c expect:['0', '9'], ['A', 'F'], ['a', 'f']
 * @retval [0, 16)
 */
static inline uint8_t c2h(char c)
{
    uint8_t l4b = c & 0x0F;

    return c >> 6 ? l4b + 9 : l4b;
}

/**
 * @brief Combine the high 4 bits and low 4 bits into one byte
 * 
 * @param h4b expect:[0, 16)
 * @param l4b expect:[0, 16)
 * @retval [0, 256)
 */
static inline uint8_t mkbyte(uint8_t h4b, uint8_t l4b)
{
    return (h4b << 4) | l4b;
}

/**
 * @brief For the freedom!
 * 
 * @param num 
 * @retval -1, if num < 0 
 * @retval 0, if num eq 0 
 * @retval 1, if num > 0 
 */
static inline int signum(int num)
{
    uint32_t neg = -num;
    return num >> 31 | neg >> 31;
}

/**
 * @brief Calculate the address of the index based on 
 *      the array address, the size of each element, and the index
 * 
 * @param[in] entry array entry
 * @param size size of a element 
 * @param idx target index
 * @retval target index element's address
 */
static inline const void *tovptr(const void *entry, uint32_t size, uint32_t idx)
{
    return (const uint8_t *)entry + size * idx;
}

__ORZ_C_ENTER__

/**
 * @brief Compare whether all bytes are the same
 * 
 * @param[in] hex bytes entry
 * @param len bytes length
 * @retval < 0, error if hex is null, or len eq 0
 * @retval 0, Not all are the same
 * @retval 1, All are the same
 */
int orz_allsame(const void *hex, uint32_t len);

/**
 * @brief Compare whether all bytes are equal to a certain value
 * 
 * @param[in] hex bytes entry
 * @param len bytes length
 * @param value certain value
 * @retval < 0, error if hex is null, or len eq 0
 * @retval 0, Not all are the same
 * @retval 1, All are the same
 */
int orz_allsame_value(const void *hex, uint32_t len, uint8_t value);


/**
 * @brief Convert every two hexadecimal characters into bytes
 *
 * @param[out] hex target's space
 * @param hlen target max length
 * @param[in] str source string's space
 * @param slen source stirng's length
 * @retval < 0, error
 * @retval >= 0, convert result length
 */
int orz_str2hex(uint8_t *hex, uint32_t hlen, const char *str, uint32_t slen);


/**
 * @brief Convert multiple hexadecimal values to strings
 *
 * @param[out] str result strings entry
 * @param slen result's max length
 * @param[in] hex source hex value entry
 * @param hlen source hex value length
 * @retval < 0, error
 * @retval >= 0, actual result length
 */
int orz_hex2str(char *str, uint32_t slen, const void *hex, uint32_t hlen);


/**
 * @brief Convert multiple hexadecimal values to strings, Use separators between each byte
 *
 * @param[out] str result strings entry
 * @param slen result's max length
 * @param[in] hex source hex value entry
 * @param hlen source hex value length
 * @param sep separator character
 * @retval < 0, error
 * @retval >= 0, actual result length
 */
int orz_hex2str_separate(char *str, uint32_t slen, const void *hex, uint32_t hlen, char sep);


/**
 * @brief Convert multiple hexadecimal values to binary strings
 *
 * @param[out] bits result strings entry
 * @param blen result's max length
 * @param[in] hex source hex value entry
 * @param hlen source hex value length
 * @retval < 0, error
 * @retval >= 0, actual result length
 */
int orz_hex2bits(char *bits, uint32_t blen, const void *hex, uint32_t hlen);


/**
 * @brief Convert multiple hexadecimal values to binary strings, Use separators between each byte
 *
 * @param[out] bits result strings entry
 * @param blen result's max length
 * @param[in] hex source hex value entry
 * @param hlen source hex value length
 * @param sep separator character
 * @retval < 0, error
 * @retval >= 0, actual result length
 */
int orz_hex2bits_separate(char *bits, uint32_t blen, const void *hex, uint32_t hlen, char sep);


/**
 * @brief The count of bits with a value of 1
 *
 * @param value
 * @retval >= 0, binary 1 bit count
 */
int orz_bin_1bits(uint64_t value);


__ORZ_C_LEAVE__



#endif //__ORZ_UTIL_H__

