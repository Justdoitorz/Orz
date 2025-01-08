/**
 * @file orz_buffer.h
 * @author Orz
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __ORZ_BUFFER_H__
#define __ORZ_BUFFER_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_util.h"
#include "orz_retval.h"


#define BUFFER_NEED(nmemb, size)    (sizeof(struct buffer) + (nmemb) * (size))

#define INIT_ORZ_BUFFER(buf, nmemb, size)   orz_buffer_init(buf, nmemb, size, MOD_BUFFER)

#define BUFFER_IN_STACK(buf, nmemb, size) \
    uint8_t _stack_ ## buf [BUFFER_NEED(nmemb, size)];     \
    struct buffer *buf = (struct buffer *) _stack_ ## buf; \
    INIT_ORZ_BUFFER(buf, nmemb, size)

#define SNV2BUF(v)      ORZ_CONTAINER_OF(v, struct buffer, snv)


typedef void *snv;

// Structure representing a buffer
struct buffer {
    uint16_t module;  // Module identifier
    uint16_t size;    // Size of each element
    uint32_t nmemb;   // Number of elements
    uint8_t snv[];    // Flexible array member for buffer data
};


__ORZ_C_ENTER__

/**
 * @brief Calculate the memory needed for the buffer
 *
 * @param nmemb Number of elements
 * @param size Size of each element
 * @retval < 0, error
 * @retval > 0, buffer need's memory length
 */
static inline int orz_buffer_need(uint32_t nmemb, uint16_t size)
{
    if (nmemb == 0 || size == 0) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_ZERO_LENGTH, 0);
    if ((uint64_t)nmemb * size > __INT32_MAX__) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_LARGE_LENGTH, 0);

    return BUFFER_NEED(nmemb, size);
}

/**
 * @brief Initialize the buffer
 * 
 * @param module buffer module identifer
 * @param buf Pointer to the buffer
 * @param nmemb Number of elements
 * @param size Size of each element
 * @retval < 0, error
 * @retval 0, success
 */
static inline int orz_buffer_init(struct buffer *buf, uint32_t nmemb, uint16_t size, uint16_t module)
{
    if (buf == NULL) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_NULLPTR, 1);

    buf->module = module;
    buf->size = size;
    buf->nmemb = nmemb;

    return 0; 
}

/**
 * @brief Deinitialize the buffer
 * 
 * @param buf Pointer to the buffer
 * @retval < 0, error
 * @retval 0, success
 */
static inline int orz_buffer_deinit(struct buffer *buf)
{
    if (buf == NULL) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_NULLPTR, 2);

    return 0;
}

/**
 * @brief Get the number of elements in the buffer
 * 
 * @param buf Pointer to the buffer
 * @retval < 0, error
 * @retval > 0, number of elements 
 */
static inline int orz_buffer_nmemb(const struct buffer *buf)
{
    if (buf == NULL) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_NULLPTR, 3);

    return buf->nmemb;
}

/**
 * @brief Get the size of each element in the buffer
 * 
 * @param buf Pointer to the buffer
 * @retval < 0, error
 * @retval > 0, size of each elememt
 */
static inline int orz_buffer_size(const struct buffer *buf)
{
    if (buf == NULL) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_NULLPTR, 4);

    return buf->size;
}

/**
 * @brief Get a pointer to an element in the buffer
 * 
 * @param buf Pointer to the buffer
 * @param out Output pointer to the element
 * @param idx Index of the element
 * @retval < 0, error
 * @retval > 0, remain elements 
 */
static inline int orz_buffer_get(const struct buffer *buf, void **out, uint32_t idx)
{
    if (buf == NULL) return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_NULLPTR, 5);
    if (idx >= buf->nmemb)  return ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_OUT_OF_RANGE, 5);

    if (out)
        *out = (void *)(buf->snv + idx * buf->size);

    return buf->nmemb - idx;
}

__ORZ_C_LEAVE__

#endif //__ORZ_BUFFER_H__
