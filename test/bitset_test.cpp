#include <gtest/gtest.h>

#include "orz_bitset.h"
#include "orz_retval.h"


TEST(Bitset, Create_Destroy) 
{
    bitset *bs = NULL;
    int ret;

    ret = orz_bitset_create(NULL, 10, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);
    ret = orz_bitset_create(&bs, 0, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_bitset_create(&bs, 10, 0);
    EXPECT_EQ(ret, 16);
    
    ret = orz_bitset_destroy(bs);
    EXPECT_EQ(ret, RETVAL_OK);
}

TEST(Bitset, Test) 
{
    bitset *bs = NULL;
    int ret;

    ret = orz_bitset_test(NULL, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    orz_bitset_create(&bs, 10, 0);

    ret = orz_bitset_test(bs, 16);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    for (uint32_t bit = 0; bit < 16; ++bit) {
        ret = orz_bitset_test(bs, bit);
        EXPECT_EQ(ret, 0);
    }
    orz_bitset_destroy(bs);


    orz_bitset_create(&bs, 10, 1);
    for (uint32_t bit = 0; bit < 16; ++bit) {
        ret = orz_bitset_test(bs, bit);
        EXPECT_GT(ret, 0);
    }
    orz_bitset_destroy(bs);
}

TEST(Bitset, Set) 
{
    bitset *bs = NULL;
    int ret;

    ret = orz_bitset_set(NULL, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    orz_bitset_create(&bs, 8, 0);

    ret = orz_bitset_set(bs, 8);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    for (uint32_t bit = 0; bit < 8; ++bit) {
        ret = orz_bitset_set(bs, bit);
        EXPECT_GT(ret, 0);
    }

    orz_bitset_destroy(bs);
}

TEST(Bitset, Clear) 
{
    bitset *bs = NULL;
    int ret;

    ret = orz_bitset_clear(NULL, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    orz_bitset_create(&bs, 8, 1);

    ret = orz_bitset_clear(bs, 8);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    for (uint32_t bit = 0; bit < 8; ++bit) {
        ret = orz_bitset_clear(bs, bit);
        EXPECT_EQ(ret, 0);
    }

    orz_bitset_destroy(bs);
}

TEST(Bitset, Toggle) 
{
    bitset *bs = NULL;
    int ret;

    ret = orz_bitset_toggle(NULL, 0);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    orz_bitset_create(&bs, 8, 1);

    ret = orz_bitset_toggle(bs, 9);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    for (uint32_t bit = 0; bit < 8; ++bit) {
        ret = orz_bitset_toggle(bs, bit);
        EXPECT_EQ(ret, 0);
    }

    for (uint32_t bit = 0; bit < 8; ++bit) {
        ret = orz_bitset_toggle(bs, bit);
        EXPECT_GT(ret, 0);
    }

    orz_bitset_destroy(bs);
}


TEST(Bitset, tostringInError)
{
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BITSET, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[7]: MOD_BITSET, rsn[0]: RSN_NULLPTR, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BITSET, RSN_ZERO_LENGTH, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[7]: MOD_BITSET, rsn[2]: RSN_ZERO_LENGTH, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_BITSET, RSN_OUT_OF_RANGE, 2), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[7]: MOD_BITSET, rsn[8]: RSN_OUT_OF_RANGE, sub[2]");
}

