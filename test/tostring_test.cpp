#include <gtest/gtest.h>

#include "orz_tostring.h"


TEST(Tostring, bool) {
    EXPECT_STRCASEEQ("false", orz_tostring_bool(0));
    EXPECT_STRCASEEQ("true", orz_tostring_bool(1));

    EXPECT_STRCASEEQ("invalid", orz_tostring_bool(2));
    EXPECT_STRCASEEQ("invalid", orz_tostring_bool(0x12345678));
}

