#include <gtest/gtest.h>

#include "orz_ringbuffer.h"
#include "orz_retval.h"

TEST(Ringbuffer, Create_Destroy) 
{
    ringbuffer *rb = NULL;
    int ret;

    ret = orz_ringbuffer_create(NULL, 0, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_ringbuffer_create(&rb, 0, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_ringbuffer_create(&rb, 2, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_ringbuffer_create(&rb, 65537, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_ringbuffer_create(&rb, 2, sizeof(uint8_t));
    EXPECT_EQ(ret, RETVAL_OK);

    ret = orz_ringbuffer_destroy(rb);
    EXPECT_EQ(ret, RETVAL_OK);
}

// Test reset and position
TEST(Ringbuffer, Reset)
{
    ringbuffer *rb = NULL;
    int ret = orz_ringbuffer_create(&rb, 8, sizeof(uint32_t));
    ASSERT_EQ(ret, RETVAL_OK);

    void *out = nullptr;
    ret = orz_ringbuffer_reset(rb, &out);
    EXPECT_EQ(ret, 8);
    EXPECT_NE(out, nullptr);

    orz_ringbuffer_destroy(rb);
}

static uint16_t consumer(snv v, uint32_t nmemb, uint32_t rd, uint32_t wr, void *arg) {
    uint32_t *arr = static_cast<uint32_t*>(v);
    uint32_t *sum = static_cast<uint32_t*>(arg);

    if (rd > wr) {
        for (uint32_t idx = rd; idx < nmemb; ++idx) *sum += arr[idx];
        rd = 0;
    }

    for (uint32_t idx = rd; idx < wr; ++idx) *sum += arr[idx];

    return wr;
}

// Test occupy, append, confirm, snoop, and consume
TEST(Ringbuffer, Snoop_Append)
{
    ringbuffer *rb = NULL;
    int ret = orz_ringbuffer_create(&rb, 8, sizeof(uint32_t));
    ASSERT_EQ(ret, RETVAL_OK);

    void *out = nullptr;
    ret = orz_ringbuffer_snoop(rb, &out, 4);
    EXPECT_EQ(ret, 4);
    EXPECT_NE(out, nullptr);

    uint32_t *data = static_cast<uint32_t*>(out);
    for (int i = 0; i < 4; ++i) data[i] = i + 1;
    bool write_success = true;//Always true for this test

    uint32_t sum = 0;
    int write_pos = orz_ringbuffer_position(rb);
    EXPECT_EQ(write_pos, 0);
    ret = orz_ringbuffer_consume(rb, consumer, write_pos, &sum);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(sum, 0);

    if (write_success) { 
        ret = orz_ringbuffer_append(rb, 4);
        EXPECT_EQ(ret, RETVAL_OK);
    }

    write_pos = orz_ringbuffer_position(rb);
    EXPECT_EQ(write_pos, 4);
    sum = 0;
    ret = orz_ringbuffer_consume(rb, consumer, write_pos, &sum);
    EXPECT_EQ(ret, 4);
    EXPECT_EQ(sum, 1 + 2 + 3 + 4);
    
    orz_ringbuffer_destroy(rb);
}

TEST(Ringbuffer, Occupy_Confirm)
{
    ringbuffer *rb = NULL;
    int ret = orz_ringbuffer_create(&rb, 8, sizeof(uint32_t));
    ASSERT_EQ(ret, RETVAL_OK);

    void *out = nullptr;
    ret = orz_ringbuffer_occupy(rb, &out, 6);
    EXPECT_EQ(ret, 6);
    EXPECT_NE(out, nullptr);

    ret = orz_ringbuffer_confirm(rb); //Let write position move to 6
    int write_pos = orz_ringbuffer_position(rb);
    auto ignore = [] (snv v, uint32_t nmemb, uint32_t rd, uint32_t wr, void *arg) -> uint16_t { return wr; };
    ret = orz_ringbuffer_consume(rb, ignore, write_pos, nullptr);//Don't care this time
    EXPECT_EQ(ret, 6);

    uint16_t len = 4; //Simulate Call Occupy Twice, Loopback Happen
    void *out1 = nullptr;
    int first = orz_ringbuffer_occupy(rb, &out1, len);
    EXPECT_LT(first, 4);
    EXPECT_NE(out1, nullptr);
    uint32_t *data = static_cast<uint32_t*>(out1);
    uint32_t sequence = 1;
    for (int i = 0; i < first; ++i) data[i] = sequence++;

    if (first < len) {
        void *out2 = nullptr;
        int second = orz_ringbuffer_occupy(rb, &out2, len - first);
        EXPECT_EQ(second, len - first);
        EXPECT_NE(out2, nullptr);

        uint32_t *data2 = static_cast<uint32_t*>(out2);
        for (int i = 0; i < second; ++i) data2[i] = sequence++;
    }

    ret = orz_ringbuffer_confirm(rb);
    EXPECT_EQ(ret, RETVAL_OK);

    uint32_t sum = 0;
    write_pos = orz_ringbuffer_position(rb);
    ret = orz_ringbuffer_consume(rb, consumer, write_pos, &sum);
    EXPECT_EQ(ret, 4);
    EXPECT_EQ(sum, 1 + 2 + 3 + 4);

    orz_ringbuffer_destroy(rb);
}

// Test Query 
TEST(Ringbuffer, Query)
{
    ringbuffer *rb = NULL;
    int ret;

    ret = orz_ringbuffer_query(NULL, 0);
    ASSERT_LT(ret, ORZ_ERROR_LT);

    orz_ringbuffer_create(&rb, 16, sizeof(uint32_t));

    ret = orz_ringbuffer_query(rb, 17);
    ASSERT_LT(ret, ORZ_ERROR_LT);

    void *out = nullptr;
    ret = orz_ringbuffer_reset(rb, &out);//Usually DMA need a Start Address and Length, out is the Start Address, ret is the Length
    EXPECT_NE(out, nullptr);
    uint32_t *data = static_cast<uint32_t*>(out);

    data[0] = 1; data[1] = 2;//simulate DMA Rx 2 Data
    ret = orz_ringbuffer_query(rb, 14);
    EXPECT_EQ(ret, RETVAL_DATA_COMING);
    //If you want to consume at Data coming, Call orz_ringbuffer_position() to get the write position, Then Consume
    if (ret == RETVAL_DATA_COMING) {
        // Do it.
        // int write_pos = orz_ringbuffer_position(rb);
        // int ret = orz_ringbuffer_consume(rb, consumer, write_pos, nullptr);
    }

    data[2] = 3; data[3] = 4; data[4] = 5; data[5] = 6; //simulate DMA Rx 4 Data
    ret = orz_ringbuffer_query(rb, 10);
    EXPECT_EQ(ret, RETVAL_DATA_COMING);

    int write_pos = orz_ringbuffer_query(rb, 10);
    EXPECT_GE(write_pos, 0);
    if (write_pos >= 0) {
        uint32_t sum = 0;
        ret = orz_ringbuffer_consume(rb, consumer, write_pos, &sum);
        EXPECT_EQ(ret, 6);
        EXPECT_EQ(sum, 1 + 2 + 3 + 4 + 5 + 6);
    }

    ret = orz_ringbuffer_query(rb, 10);
    EXPECT_EQ(ret, RETVAL_HAD_STOPED);

    orz_ringbuffer_destroy(rb);
}

TEST(Ringbuffer, tostringInError) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[8]: MOD_RINGBUFFER, rsn[0]: RSN_NULLPTR, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_SPACE_NO_ENOUGH, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[8]: MOD_RINGBUFFER, rsn[1]: RSN_SPACE_NO_ENOUGH, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RINGBUFFER, RSN_OUT_OF_RANGE, 9), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[8]: MOD_RINGBUFFER, rsn[8]: RSN_OUT_OF_RANGE, sub[9]");
}
