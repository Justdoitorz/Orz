/**
 * @file orz_bitset.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-05-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __ORZ_BITSET_H__
#define __ORZ_BITSET_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_buffer.h"


typedef struct buffer bitset;

__ORZ_C_ENTER__

/**
 * @brief Create a new bitset.
 *
 * @param bs A pointer to the bitset to create.
 * @param nbits The number of bits in the bitset, must be greater than 0.
 * @param initval The initial value of the bits, 0 for clear, non 0 for set.
 * @retval < 0, Error
 * @retval > 0, The actual number of bits in the bitset, align up to 8
 */
int orz_bitset_create(bitset **bs, uint32_t nbits, uint8_t initval);

/**
 * @brief Destroy a bitset.
 *
 * @param bs The bitset to destroy.
 * @retval < 0, Error
 * @retval 0, Success
 */
int orz_bitset_destroy(bitset *bs);

/**
 * @brief Test a bit in the bitset.
 *
 * @param bs The bitset to test.
 * @param bit The index of the bit to test.
 * @retval < 0, Error
 * @retval 0, The bit is cleared 
 * @retval > 0, The bit is not cleared
 */
int orz_bitset_test(const bitset *bs, uint32_t bit);

/**
 * @brief Set a bit in the bitset.
 *
 * @param bs The bitset to modify.
 * @param bit The index of the bit to set.
 * @retval < 0, Error
 * @retval 0, The bit is cleared 
 * @retval > 0, The bit is not cleared
 */
int orz_bitset_set(bitset *bs, uint32_t bit);

/**
 * @brief Clear a bit in the bitset.
 *
 * @param bs The bitset to modify.
 * @param bit The index of the bit to clear.
 * @retval < 0, Error
 * @retval 0, The bit is cleared 
 * @retval > 0, The bit is not cleared
 */
int orz_bitset_clear(bitset *bs, uint32_t bit);

/**
 * @brief Toggle a bit in the bitset.
 *
 * @param bs The bitset to modify.
 * @param bit The index of the bit to toggle.
 * @retval < 0, Error
 * @retval 0, The bit is cleared 
 * @retval > 0, The bit is not cleared
 */
int orz_bitset_toggle(bitset *bs, uint32_t bit);


__ORZ_C_LEAVE__


#endif //__ORZ_BITSET_H__
