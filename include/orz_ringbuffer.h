/**
 * @file orz_ringbuffer.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-05-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __ORZ_RINGBUFFER_H__
#define __ORZ_RINGBUFFER_H__

#include "orz_cdefs.h"
#include "orz_types.h"
#include "orz_buffer.h"

// Forward declaration of the ringbuffer structure
typedef struct ringbuffer ringbuffer;


/**
 * @brief consume data
 * 
 * @param v data pointer
 * @param nmemb number of elements
 * @param rd read position
 * @param wr write position
 * @param arg user argument
 * @retval >= 0, consume to position
 */
typedef uint16_t (*fn_consumer)(snv v, uint32_t nmemb, uint32_t rd, uint32_t wr, void *arg);    

__ORZ_C_ENTER__

/**
 * @brief Create a ringbuffer object
 * @param rb Pointer to the ringbuffer pointer to be created
 * @param nmemb Number of elements in the buffer, it must >= 2 and <= 65536
 * @param size Size of each element, it must > 0
 * @retval < 0, error
 * @retval 0, ok
 */
int orz_ringbuffer_create(ringbuffer **rb, uint32_t nmemb, uint32_t size);

/**
 * @brief Destroy a ringbuffer object
 * @param rb Pointer to the ringbuffer to destroy
 * @retval < 0, error
 * @retval 0, ok
 */
int orz_ringbuffer_destroy(ringbuffer *rb);

/**
 * @brief Reset the ringbuffer
 * @param rb Pointer to the ringbuffer
 * @param out Optional output pointer
 * @retval < 0, error
 * @retval > 0, buffer nmemb
 */
int orz_ringbuffer_reset(ringbuffer *rb, void **out);

/**
 * @brief Get the current write position of the ringbuffer, lock if necessary
 * @param rb Pointer to the ringbuffer
 * @retval < 0, error
 * @retval > 0, write position
 */
int orz_ringbuffer_position(const ringbuffer *rb);

/**
 * @brief Consume data from the ringbuffer using a consumer callback
 * @param rb Pointer to the ringbuffer
 * @param consumer Consumer callback function
 * @param write_pos Write pointer position
 * @param arg User argument
 * @retval < 0, error
 * @retval >= 0, number of elements consumed
 */
int orz_ringbuffer_consume(ringbuffer *rb, fn_consumer consumer, uint16_t write_pos, void *arg);

/**
 * @brief Peek data from the ringbuffer without moving pointers
 * @param rb Pointer to the ringbuffer
 * @param out Output data pointer
 * @param count Number of elements to peek
 * @retval < 0, error
 * @retval >= 0, count of elements peeked
 */
int orz_ringbuffer_snoop(const ringbuffer *rb, void **out, uint16_t count);

/**
 * @brief Append data to the ringbuffer, lock if necessary
 * @param rb Pointer to the ringbuffer
 * @param count Number of elements to append
 * @retval < 0, error
 * @retval 0, ok
 */
int orz_ringbuffer_append(ringbuffer *rb, uint16_t count);

/**
 * @brief Occupy space in the ringbuffer for writing
 * @param rb Pointer to the ringbuffer
 * @param out Output data pointer
 * @param len Length to occupy
 * @retval < 0, error
 * @retval >= 0, number of elements occupied
 */
int orz_ringbuffer_occupy(ringbuffer *rb, void **out, uint16_t len);

/**
 * @brief Confirm the written data in the ringbuffer, lock if necessary
 * @param rb Pointer to the ringbuffer
 * @retval < 0, error
 * @retval 0, ok
 */
int orz_ringbuffer_confirm(ringbuffer *rb);

/**
 * @brief Query the ringbuffer for remaining elements, lock if necessary
 * @param rb Pointer to the ringbuffer
 * @param remain Number of remaining elements
 * @retval < ORZ_ERROR_LT, error
 * @retval RETVAL_DATA_COMING, data coming
 * @retval RETVAL_HAD_STOPED, data stopped
 * @retval >= 0, write position
 */
int orz_ringbuffer_query(ringbuffer *rb, uint16_t remain);

__ORZ_C_LEAVE__


#endif // __ORZ_RINGBUFFER_H__
