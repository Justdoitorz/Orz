/**
 * @file orz_bitset.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-05-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "orz_bitset.h"
#include "orz_alloc.h"
#include "orz_util.h"
#include <string.h>

#define BITSET_MEM_BITS             8
#define BITSET_GET_MEM(number)      ((number) >> 3)
#define BITSET_GET_BIT(number)      ((number) &  7)


__ORZ_EXPORT
int orz_bitset_create(bitset **bs, uint32_t nbits, uint8_t initval)
{
    if (bs == NULL) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 0);
    if (nbits == 0) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_ZERO_LENGTH, 0);

    uint32_t up = ORZ_ALIGNUP2(nbits, BITSET_MEM_BITS) / BITSET_MEM_BITS;
    bitset *tmp_bs = orz_malloc(orz_buffer_need(up, sizeof(uint8_t)));
    if (tmp_bs == NULL) 
        return RETVAL_MEMORY_EMPTY;
    
    orz_buffer_init(tmp_bs, up, sizeof(uint8_t), MOD_BITSET);
    memset(tmp_bs->snv, signum(-initval), up);
    // memset(tmp_bs->snv, initval ? 0xFF : 0, up);
    *bs = tmp_bs;

    return up * BITSET_MEM_BITS;
}

__ORZ_EXPORT
int orz_bitset_destroy(bitset *bs)
{
    if (bs == NULL) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 1);

    // orz_buffer_deinit(bs);
    orz_free(bs);

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_bitset_test(const bitset *bs, uint32_t bit)
{
    if (bs == NULL) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 2);
    if (bit >= bs->nmemb * BITSET_MEM_BITS) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_OUT_OF_RANGE, 2);

    const uint8_t *data = &bs->snv[BITSET_GET_MEM(bit)];

    return ORZ_TSTBIT(*data, BITSET_GET_BIT(bit));
}

__ORZ_EXPORT
int orz_bitset_set(bitset *bs, uint32_t bit)
{
    if (bs == NULL) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 3);
    if (bit >= bs->nmemb * BITSET_MEM_BITS) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_OUT_OF_RANGE, 3);

    uint8_t *data = &bs->snv[BITSET_GET_MEM(bit)];
    *data = ORZ_SETBIT(*data, BITSET_GET_BIT(bit));

    return ORZ_TSTBIT(*data, BITSET_GET_BIT(bit));
}

__ORZ_EXPORT
int orz_bitset_clear(bitset *bs, uint32_t bit)
{
    if (bs == NULL) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 4);
    if (bit >= bs->nmemb * BITSET_MEM_BITS) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_OUT_OF_RANGE, 4);

    uint8_t *data = &bs->snv[BITSET_GET_MEM(bit)];
    *data = ORZ_CLRBIT(*data, BITSET_GET_BIT(bit));

    return ORZ_TSTBIT(*data, BITSET_GET_BIT(bit));
}

__ORZ_EXPORT
int orz_bitset_toggle(bitset *bs, uint32_t bit)
{
    if (bs == NULL) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 5);
    if (bit >= bs->nmemb * BITSET_MEM_BITS) return ORZ_RETVAL_ERROR(MOD_BITSET, RSN_OUT_OF_RANGE, 5);

    uint8_t *data = &bs->snv[BITSET_GET_MEM(bit)];
    *data = ORZ_TOGBIT(*data, BITSET_GET_BIT(bit));

    return ORZ_TSTBIT(*data, BITSET_GET_BIT(bit));
}

