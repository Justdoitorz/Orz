#include <gtest/gtest.h>

#include "orz_alloc.h"

TEST(Alloc, init)
{
#ifdef USE_MACRO_ALLOC
    EXPECT_EQ(orz_alloc_init(NULL, NULL), RETVAL_OK);
#else 
    EXPECT_LT(orz_alloc_init(NULL, NULL), ORZ_ERROR_LT);
#endif
}

TEST(Alloc, deinit)
{
    EXPECT_EQ(orz_alloc_deinit(), RETVAL_OK);
}

TEST(Alloc, malloc_free)
{   
    void *p = orz_malloc(sizeof(int));
    ASSERT_NE(p, nullptr);
 
    *(int*)p = -1;
    EXPECT_EQ(__UINT8_MAX__, ((uint8_t *)p)[0]);
    EXPECT_EQ(__UINT8_MAX__, ((uint8_t *)p)[1]);
    EXPECT_EQ(-1, ((int8_t *)p)[2]);
    EXPECT_EQ(-1, ((int8_t *)p)[3]);

    EXPECT_EQ(__UINT16_MAX__, ((uint16_t *)p)[0]);
    EXPECT_EQ(-1, ((int16_t *)p)[1]);

    EXPECT_EQ(__UINT32_MAX__, ((uint32_t *)p)[0]);
    EXPECT_EQ(-1, ((int32_t *)p)[0]);

    orz_free(p);
}


TEST(Alloc, tostringInError) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_ALLOC, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[4]: MOD_ALLOC, rsn[0]: RSN_NULLPTR, sub[0]");
}
