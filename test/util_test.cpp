#include <gtest/gtest.h>

#include "orz_util.h"


TEST(Util, TestBit) 
{
    EXPECT_EQ(ORZ_TSTBIT(0x12345678, 0), 0);
    EXPECT_NE(ORZ_TSTBIT(0x12345678, 3), 0);
}

TEST(Util, SetBit) 
{
    EXPECT_EQ(ORZ_SETBIT(0x12345678, 0), 0x12345679);
    EXPECT_EQ(ORZ_SETBIT(0x12345678, 3), 0x12345678);
}

TEST(Util, ClearBit) 
{
    EXPECT_EQ(ORZ_CLRBIT(0x12345678, 0), 0x12345678);
    EXPECT_EQ(ORZ_CLRBIT(0x12345678, 3), 0x12345670);
}

TEST(Util, ToggleBit) 
{
    EXPECT_EQ(ORZ_TOGBIT(0x12345678, 0), 0x12345679);
    EXPECT_EQ(ORZ_TOGBIT(0x12345678, 3), 0x12345670);
}

TEST(Util, Max)
{
    EXPECT_EQ(ORZ_MAX(1, 2), 2);
    EXPECT_EQ(ORZ_MAX(1, -2), 1);
    EXPECT_EQ(ORZ_MAX(-1, 2), 2);
    EXPECT_EQ(ORZ_MAX(-1, -2), -1);
}

TEST(Util, Min)
{
    EXPECT_EQ(ORZ_MIN(1, 2), 1);
    EXPECT_EQ(ORZ_MIN(1, -2), -2);
    EXPECT_EQ(ORZ_MIN(-1, 2), -1);
    EXPECT_EQ(ORZ_MIN(-1, -2), -2);
}

TEST(Util, Max3)
{
    EXPECT_EQ(ORZ_MAX3(1, 2, 3), 3);
    EXPECT_EQ(ORZ_MAX3(1, 2, -3), 2);
    EXPECT_EQ(ORZ_MAX3(1, -2, 3), 3);
    EXPECT_EQ(ORZ_MAX3(1, -2, -3), 1);
    EXPECT_EQ(ORZ_MAX3(-1, 2, 3), 3);
    EXPECT_EQ(ORZ_MAX3(-1, 2, -3), 2);
    EXPECT_EQ(ORZ_MAX3(-1, -2, 3), 3);
    EXPECT_EQ(ORZ_MAX3(-1, -2, -3), -1);
}

TEST(Util, Min3)
{
    EXPECT_EQ(ORZ_MIN3(1, 2, 3), 1);
    EXPECT_EQ(ORZ_MIN3(1, 2, -3), -3);
    EXPECT_EQ(ORZ_MIN3(1, -2, 3), -2);
    EXPECT_EQ(ORZ_MIN3(1, -2, -3), -3);
    EXPECT_EQ(ORZ_MIN3(-1, 2, 3), -1);
    EXPECT_EQ(ORZ_MIN3(-1, 2, -3), -3);
    EXPECT_EQ(ORZ_MIN3(-1, -2, 3), -2);
    EXPECT_EQ(ORZ_MIN3(-1, -2, -3), -3);
}

TEST(Util, AlignUp) 
{
    EXPECT_EQ(ORZ_ALIGNUP(5, 3), 6);
    EXPECT_EQ(ORZ_ALIGNUP(6, 3), 6);
}

TEST(Util, AlignDown) 
{
    EXPECT_EQ(ORZ_ALIGNDOWN(5, 3), 3);
    EXPECT_EQ(ORZ_ALIGNDOWN(6, 3), 6);
}

TEST(Util, AlignUp2) 
{
    EXPECT_EQ(ORZ_ALIGNUP2(5, 2), 6);
    EXPECT_EQ(ORZ_ALIGNUP2(6, 2), 6);
}

TEST(Util, AlignDown2) 
{
    EXPECT_EQ(ORZ_ALIGNDOWN2(5, 2), 4);
    EXPECT_EQ(ORZ_ALIGNDOWN2(6, 2), 6);
}

TEST(Util, Pow2N)
{
    EXPECT_EQ(ORZ_POW2_N(0), 1);
    EXPECT_EQ(ORZ_POW2_N(2), 4);
    EXPECT_EQ(ORZ_POW2_N(10), 1024);    
}

TEST(Util, ArrayLength)
{
    char hello[] = "hello";
    EXPECT_EQ(ORZ_ARRLEN(hello), 6);

    uint8_t a[] = {1, 2, 4, 4};
    EXPECT_EQ(ORZ_ARRLEN(a), 4);

    uint32_t b[] = {1, 2, 3, 4};
    EXPECT_EQ(ORZ_ARRLEN(b), 4);

    uint32_t matrix[][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8}, 
        {9, 0, 1, 2},
    };
    EXPECT_EQ(ORZ_ARRLEN(matrix), 3);
    EXPECT_EQ(ORZ_ARRLEN(matrix[0]), 4);
}

TEST(Util, h2c) 
{
    const char *ch = "0123456789ABCDEF";
    for (int h = 0; h < 16; ++h) {
        EXPECT_EQ(h2c(h), ch[h]);
    }
}

TEST(Util, c2h) 
{
    const char *digit = "0123456789";
    for (uint8_t d = 0; d < 10; ++d) {
        EXPECT_EQ(c2h(digit[d]), d);
    }

    const char *upper = "0123456789ABCDEF";
    const char *lower = "0123456789abcdef";
    for (uint8_t a = 10; a < 16; ++a) {
        EXPECT_EQ(c2h(upper[a]), a);
        EXPECT_EQ(c2h(lower[a]), a);
    }
}

TEST(Util, mkbyte)
{
    EXPECT_EQ(mkbyte(1, 2), 0x12);
    EXPECT_EQ(mkbyte(0xF, 0xA), 0xFA);

    uint8_t idx = 0;
    for (uint8_t h4b = 0; h4b < 16; ++h4b) {
        for (uint8_t l4b = 0; l4b < 16; ++l4b) {
            EXPECT_EQ(mkbyte(h4b, l4b), idx);
            ++idx;
        }
    }
}

TEST(Util, signum)
{
    EXPECT_EQ(signum(0), 0);

    for (int num = 1; num < 1024; num <<= 1) {
        EXPECT_EQ(signum(num), 1);
    }

    for (int num = -1; num > -1024; num <<= 1) {
        EXPECT_EQ(signum(num), -1);
    }
}

TEST(Util, tovptr)
{
    uint32_t array[] = {111, 222, 333, 444, 555};
    const void *third = tovptr(array, sizeof(array[0]), 2);
    EXPECT_EQ(third, &array[2]);
    EXPECT_EQ(*(const uint32_t *)third, 333);

    *(uint32_t *)third = 666;
    EXPECT_EQ(array[2], 666);
}

TEST(Util, AllSame)
{
    EXPECT_LT(orz_allsame(NULL, 0), 0);
    uint8_t tmp[1];
    EXPECT_LT(orz_allsame(tmp, 0), 0);

    uint8_t notsame[] = {1, 2, 1};
    EXPECT_EQ(orz_allsame(notsame, ORZ_ARRLEN(notsame)), false);

    uint8_t same[] = {1, 1, 1};
    EXPECT_EQ(orz_allsame(same, ORZ_ARRLEN(same)), true);
    EXPECT_EQ(orz_allsame(same, 1), true);
}

TEST(Util, AllSameValue)
{
    EXPECT_LT(orz_allsame_value(NULL, 0, 0), 0);
    uint8_t tmp[1];
    EXPECT_LT(orz_allsame_value(tmp, 0, 0), 0);

    uint8_t notsame[] = {1, 2, 1};
    EXPECT_EQ(orz_allsame_value(notsame, ORZ_ARRLEN(notsame), 1), false);

    uint8_t same[] = {1, 1, 1};
    EXPECT_EQ(orz_allsame_value(same, ORZ_ARRLEN(same), 1), true);
    EXPECT_EQ(orz_allsame_value(same, 1, 1), true);

    EXPECT_EQ(orz_allsame_value(same, ORZ_ARRLEN(same), 2), false);
    EXPECT_EQ(orz_allsame_value(same, 1, 2), false);
}

TEST(Util, Str2Hex)
{
    uint8_t hex[2];
    EXPECT_LT(orz_str2hex(NULL, sizeof(hex), "BEEF", 4), 0);
    EXPECT_LT(orz_str2hex(hex, sizeof(hex), NULL, 4), 0);
    EXPECT_LT(orz_str2hex(hex, 1, "BEEF", 4), 0);
    EXPECT_LT(orz_str2hex(hex, sizeof(hex), "BEEF", 3), 0);


    EXPECT_EQ(orz_str2hex(hex, sizeof(hex), "BEEF", 0), 0);
    EXPECT_EQ(orz_str2hex(hex, sizeof(hex), "BEEF", 2), 1);
    EXPECT_EQ(orz_str2hex(hex, sizeof(hex), "BEEF", 4), 2);
    EXPECT_EQ(hex[0], 0xBE);
    EXPECT_EQ(hex[1], 0xEF);
}

TEST(Util, Hex2Str)
{
    char str[2 * 2 + 1];
    EXPECT_LT(orz_hex2str(NULL, sizeof(str), "\xBE\xEF", 2), 0);
    EXPECT_LT(orz_hex2str(str, sizeof(str), NULL, 2), 0);
    EXPECT_LT(orz_hex2str(str, 4, "\xBE\xEF", 2), 0);


    EXPECT_EQ(orz_hex2str(str, sizeof(str), "\xBE\xEF", 0), 0);
    EXPECT_STREQ(str, "");
    EXPECT_EQ(orz_hex2str(str, sizeof(str), "\xBE\xEF", 1), 2);
    EXPECT_STRCASEEQ(str, "be");
    EXPECT_EQ(orz_hex2str(str, sizeof(str), "\xBE\xEF", 2), 4);
    EXPECT_STRCASEEQ(str, "beef");
}

TEST(Util, Hex2StrSeparate)
{
    char str[2 * 3];
    EXPECT_LT(orz_hex2str_separate(NULL, sizeof(str), "\xBE\xEF", 2, ' '), 0);
    EXPECT_LT(orz_hex2str_separate(str, sizeof(str), NULL, 2, ' '), 0);
    EXPECT_LT(orz_hex2str_separate(str, 5, "\xBE\xEF", 2, ' '), 0);


    EXPECT_EQ(orz_hex2str_separate(str, sizeof(str), "\xBE\xEF", 0, ' '), 0);
    EXPECT_STREQ(str, "");
    EXPECT_EQ(orz_hex2str_separate(str, sizeof(str), "\xBE\xEF", 1, ' '), 2);
    EXPECT_STRCASEEQ(str, "be");
    EXPECT_EQ(orz_hex2str_separate(str, sizeof(str), "\xBE\xEF", 2, ' '), 5);
    EXPECT_STRCASEEQ(str, "be ef");
}


TEST(Util, Hex2Bits)
{
    char str[2 * 8 + 1];
    EXPECT_LT(orz_hex2bits(NULL, sizeof(str), "\xBE\xEF", 2), 0);
    EXPECT_LT(orz_hex2bits(str, sizeof(str), NULL, 2), 0);
    EXPECT_LT(orz_hex2bits(str, 16, "\xBE\xEF", 2), 0);


    EXPECT_EQ(orz_hex2bits(str, sizeof(str), "\xBE\xEF", 0), 0);
    EXPECT_STREQ(str, "");
    EXPECT_EQ(orz_hex2bits(str, sizeof(str), "\xBE\xEF", 1), 8);
    EXPECT_STREQ(str, "10111110");
    EXPECT_EQ(orz_hex2bits(str, sizeof(str), "\xBE\xEF", 2), 16);
    EXPECT_STREQ(str, "1011111011101111");
}

TEST(Util, Hex2BitsSeparate)
{
    char str[2 * 9];
    EXPECT_LT(orz_hex2bits_separate(NULL, sizeof(str), "\xBE\xEF", 2, ' '), 0);
    EXPECT_LT(orz_hex2bits_separate(str, sizeof(str), NULL, 2, ' '), 0);
    EXPECT_LT(orz_hex2bits_separate(str, 17, "\xBE\xEF", 2, ' '), 0);


    EXPECT_EQ(orz_hex2bits_separate(str, sizeof(str), "\xBE\xEF", 0, ' '), 0);
    EXPECT_STREQ(str, "");
    EXPECT_EQ(orz_hex2bits_separate(str, sizeof(str), "\xBE\xEF", 1, ' '), 8);
    EXPECT_STREQ(str, "10111110");
    EXPECT_EQ(orz_hex2bits_separate(str, sizeof(str), "\xBE\xEF", 2, ' '), 17);
    EXPECT_STREQ(str, "10111110 11101111");
}

TEST(Util, Bin1Bits)
{
    EXPECT_EQ(orz_bin_1bits(-1), 64);
    EXPECT_EQ(orz_bin_1bits(0), 0);
    EXPECT_EQ(orz_bin_1bits(0x12345678), 13);
    EXPECT_EQ(orz_bin_1bits(0x87654321), 13);
    EXPECT_EQ(orz_bin_1bits(0x123456789abcdef), 13 + 19);

    for (uint32_t idx = 0; idx < 64; ++idx) {
        EXPECT_EQ(orz_bin_1bits(1ULL << idx), 1);
    }
}

