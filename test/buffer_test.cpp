#include <gtest/gtest.h>

#include "orz_buffer.h"
#include "orz_alloc.h"
#include "orz_retval.h"


TEST(Buffer, need)
{
    int ret = 0;
    ret = orz_buffer_need(0, sizeof(uint8_t));
    EXPECT_LT(ret, ORZ_ERROR_LT);
    ret = orz_buffer_need(1024, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);
    ret = orz_buffer_need(__UINT32_MAX__, sizeof(uint8_t));
    EXPECT_LT(ret, ORZ_ERROR_LT);

    EXPECT_GE(orz_buffer_need(1024, sizeof(uint8_t)), 1024 * sizeof(uint8_t));
    EXPECT_GE(orz_buffer_need(1024, sizeof(uint16_t)), 1024 * sizeof(uint16_t));
}

TEST(Buffer, init)
{
    int ret = 0;
    ret = INIT_ORZ_BUFFER(NULL, 1024, sizeof(uint8_t));
    EXPECT_LT(ret, ORZ_ERROR_LT);

    struct buffer *buf = (struct buffer *)orz_malloc(orz_buffer_need(1024, sizeof(uint8_t)));
    ASSERT_NE(buf, nullptr);

    ret = INIT_ORZ_BUFFER(buf, 1024, sizeof(uint8_t));
    EXPECT_EQ(ret, 0);

    orz_buffer_deinit(buf);
    orz_free(buf);
}

TEST(Buffer, deinit)
{
    int ret = 0;
    ret = orz_buffer_deinit(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    struct buffer *buf = (struct buffer *)orz_malloc(orz_buffer_need(1024, sizeof(uint8_t)));
    ASSERT_NE(buf, nullptr);

    ret = INIT_ORZ_BUFFER(buf, 1024, sizeof(uint8_t));

    ret = orz_buffer_deinit(buf);
    EXPECT_EQ(ret, 0);

    orz_free(buf);
}

TEST(Buffer, nmemb)
{
    int ret = 0;
    ret = orz_buffer_nmemb(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    struct buffer *buf = (struct buffer *)orz_malloc(orz_buffer_need(1024, sizeof(uint8_t)));
    ASSERT_NE(buf, nullptr);

    ret = INIT_ORZ_BUFFER(buf, 1024, sizeof(uint8_t));

    ret = orz_buffer_nmemb(buf);
    EXPECT_EQ(ret, 1024);

    ret = orz_buffer_deinit(buf);
    orz_free(buf);
}

TEST(Buffer, size)
{
    int ret = 0;
    ret = orz_buffer_size(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    struct buffer *buf = (struct buffer *)orz_malloc(orz_buffer_need(1024, sizeof(uint8_t)));
    ASSERT_NE(buf, nullptr);

    ret = INIT_ORZ_BUFFER(buf, 1024, sizeof(uint8_t));

    ret = orz_buffer_size(buf);
    EXPECT_EQ(ret, sizeof(uint8_t));

    ret = orz_buffer_deinit(buf);
    orz_free(buf);
}

TEST(Buffer, get)
{
    int ret = 0;
    ret = orz_buffer_get(NULL, NULL, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    struct buffer *buf = (struct buffer *)orz_malloc(orz_buffer_need(1024, sizeof(uint16_t)));
    ASSERT_NE(buf, nullptr);

    INIT_ORZ_BUFFER(buf, 1024, sizeof(uint16_t));

    ret = orz_buffer_get(buf, NULL, -1);
    EXPECT_LT(ret, ORZ_ERROR_LT);
    ret = orz_buffer_get(buf, NULL, 1024);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_buffer_get(buf, NULL, 0);
    EXPECT_EQ(ret, 1024);

    void *out = NULL;
    ret = orz_buffer_get(buf, &out, 100);
    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out, buf->snv + 100 * sizeof(uint16_t));
    EXPECT_EQ(ret, 1024 - 100);


    orz_buffer_deinit(buf);
    orz_free(buf);
}

TEST(Buffer, inStack)
{
    int ret;
    BUFFER_IN_STACK(hello, 16, sizeof(uint8_t));

    void *out = NULL;
    ret = orz_buffer_get(hello, &out, 0);
    EXPECT_EQ(ret, 16);
    EXPECT_NE(out, nullptr);

    strcpy((char *)out, "Hello World!");

    struct buffer *buf = SNV2BUF(hello->snv); 
    EXPECT_EQ(hello, buf);

    EXPECT_STREQ((char *)(buf->snv), "Hello World!");
}

TEST(Buffer, tostringInError) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_ZERO_LENGTH, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[5]: MOD_BUFFER, rsn[2]: RSN_ZERO_LENGTH, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_LARGE_LENGTH, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[5]: MOD_BUFFER, rsn[7]: RSN_LARGE_LENGTH, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_NULLPTR, 1), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[5]: MOD_BUFFER, rsn[0]: RSN_NULLPTR, sub[1]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BUFFER, RSN_OUT_OF_RANGE, 5), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[5]: MOD_BUFFER, rsn[8]: RSN_OUT_OF_RANGE, sub[5]");
}
