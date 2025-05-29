/**
 * @file orz_ringbuffer.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2025-05-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "orz_ringbuffer.h"
#include "orz_util.h"
#include "orz_alloc.h"
#include "orz_retval.h"


/**
 * 生产数据时，修改写位置达到目的。我们有三种方式来生产数据。
 *
 * 1. 使用 ringbuffer_snoop() 函数窥探空间，然后写数据，再使用 ringbuffer_append() 函数追加数据。
 *    如果数据写入阶段失败，则放弃追加操作，写位置不会被修改。
 * 
 * 2. 使用 ringbuffer_occupy() 函数占用空间，然后写数据，再使用 ringbuffer_confirm() 函数确认数据。
 *    数据写入阶段必须成功。通常调用 ringbuffer_occupy() 和 ringbuffer_confirm() 一次。
 *    有时需要调用两次ringbuffer_occupy(), 当第一次调用 int ret = ringbuffer_occupy(rb, &out1, len) 的返回值ret小于len时，
 *    需要再调用一次ringbuffer_occupy(rb, &out2, len - ret)。
 *
 * 3. 使用 ringbuffer_query() 查询写位置是否更新, 该函数会将写位置移动到剩余空间的起始位置。
 *    通常用在DMA循环接收数据模式。在周期函数里去调用 ringbuffer_query() 函数，查询写位置是否更新。 
 */

struct ringbuffer {
    uint16_t func_idx;
    uint16_t read_pos;
    uint16_t write_pos;
    uint16_t write_pos_save;

    struct buffer buf[];
};


static inline uint16_t __ringbuffer_pos_mod(uint16_t pos, uint32_t nmemb, uint16_t len)
{
    return (pos + len) % nmemb;
}

static inline uint16_t __ringbuffer_pos_and(uint16_t pos, uint32_t nmemb, uint16_t len)
{
    return (pos + len) & (nmemb - 1);
}


static uint16_t __ringbuffer_pos_func(uint16_t idx, uint16_t pos, uint32_t nmemb, uint16_t len)
{
    return __ringbuffer_pos_mod(pos, nmemb, len);

    // if (idx == 1) {
        // return __ringbuffer_pos_and(pos, nmemb, len);
    // } else {
        // return __ringbuffer_pos_mod(pos, nmemb, len);
    // }
}

static inline uint32_t __ringbuffer_nmemb(const ringbuffer *rb)
{
    return orz_buffer_nmemb(rb->buf);
}

__ORZ_EXPORT
int orz_ringbuffer_create(ringbuffer **rb, uint32_t nmemb, uint32_t size)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 0);
    if (nmemb < 2 || nmemb > 65536 || size == 0) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_WRONG_LENGTH, 0);
 
    ringbuffer *tmp_rb = orz_malloc(sizeof(ringbuffer) + orz_buffer_need(nmemb, size));
    if (tmp_rb == NULL) 
        return RETVAL_MEMORY_EMPTY;

    tmp_rb->func_idx = ORZ_IS_POW2(nmemb) ? 1 : 0;
    tmp_rb->read_pos = 0;
    tmp_rb->write_pos = 0;  
    tmp_rb->write_pos_save = 0;
    orz_buffer_init(tmp_rb->buf, nmemb, size, MOD_RINGBUFFER);

    *rb = tmp_rb;

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_ringbuffer_destroy(ringbuffer *rb)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 1);

    orz_buffer_deinit(rb->buf);
    orz_free(rb);

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_ringbuffer_reset(ringbuffer *rb, void **out)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 2);

    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->write_pos_save = 0;

    return orz_buffer_get(rb->buf, out, 0);    
}

__ORZ_EXPORT
int orz_ringbuffer_position(const ringbuffer *rb)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 3);

    return rb->write_pos;
}

__ORZ_EXPORT
int orz_ringbuffer_consume(ringbuffer *rb, fn_consumer consumer, uint16_t write_pos, void *arg)
{
    if (rb == NULL || consumer == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 4);

    uint16_t len = 0;
    uint16_t read_pos = rb->read_pos;

    if (read_pos != write_pos) {
        uint32_t nmemb = __ringbuffer_nmemb(rb);
        uint16_t new_pos = consumer(rb->buf->snv, nmemb, read_pos, write_pos, arg);
        rb->read_pos = new_pos < nmemb ? new_pos : write_pos;

        len = __ringbuffer_pos_func(rb->func_idx, new_pos - read_pos, nmemb, 0);
    }

    return len;
}

__ORZ_EXPORT
int orz_ringbuffer_snoop(const ringbuffer *rb, void **out, uint16_t count)
{
    if (rb == NULL || out == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 5);

    int ret = orz_buffer_get(rb->buf, out, rb->write_pos);

    return ORZ_MIN(count, ret);
}


__ORZ_EXPORT
int orz_ringbuffer_append(ringbuffer *rb, uint16_t count)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 6);

    rb->write_pos = rb->write_pos_save = __ringbuffer_pos_func(rb->func_idx, rb->write_pos, __ringbuffer_nmemb(rb), count);

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_ringbuffer_occupy(ringbuffer *rb, void **out, uint16_t len)
{
    if (rb == NULL || out == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 7);

    int ret = orz_buffer_get(rb->buf, out, rb->write_pos_save);
    uint16_t min = ORZ_MIN(len, ret);

    rb->write_pos_save = __ringbuffer_pos_func(rb->func_idx, rb->write_pos_save, __ringbuffer_nmemb(rb), min);

    return min;
}

__ORZ_EXPORT
int orz_ringbuffer_confirm(ringbuffer *rb)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 8);

    rb->write_pos = rb->write_pos_save;

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_ringbuffer_query(ringbuffer *rb, uint16_t remain)
{
    if (rb == NULL) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 9);
    uint32_t nmemb = __ringbuffer_nmemb(rb);
    if (remain > nmemb) return ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_OUT_OF_RANGE, 9);

    uint16_t new_pos = nmemb - remain;
    if (new_pos != rb->write_pos) {
        rb->write_pos = rb->write_pos_save = new_pos;
        return RETVAL_DATA_COMING;
    } else if (rb->read_pos == rb->write_pos) {
        return RETVAL_HAD_STOPED;
    }

    return rb->write_pos;
}
