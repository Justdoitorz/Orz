#include <gtest/gtest.h>

#include "orz_endian.h"

TEST(Endian, Htons)
{
    uint8_t be[] = {0x12, 0x34};
    EXPECT_EQ(orz_htons(*(uint16_t *)be), 0x1234);
    EXPECT_EQ(orz_ntohs(*(uint16_t *)be), 0x1234);
}

TEST(Endian, Htonl)
{
    uint8_t be[] = {0x12, 0x34, 0x56, 0x78};
    EXPECT_EQ(orz_htonl(*(uint32_t *)be), 0x12345678);
    EXPECT_EQ(orz_ntohl(*(uint32_t *)be), 0x12345678);
}

TEST(Endian, Htonx)
{
    uint8_t be[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    EXPECT_EQ(orz_htonx(*(uint64_t *)be), 0x123456789abcdef0);
    EXPECT_EQ(orz_htonx(*(uint64_t *)be), 0x123456789abcdef0);
}

