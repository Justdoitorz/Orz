#include <gtest/gtest.h>

#include "orz_algorithm.h"
#include "orz_retval.h"


TEST(Algorithm, CommonSearch) 
{
    uint8_t disorder[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
    auto comp = [](const void *lhs, const void *rhs) -> int {
        return (*(uint8_t *)lhs) - (*(uint8_t *)rhs);
    };
    uint8_t target = 7;
    uint8_t notfound = 11;

    EXPECT_LT(orz_common_search(NULL, ORZ_ARRLEN(disorder), sizeof(disorder[0]), comp, &target), ORZ_ERROR_LT);
    EXPECT_LT(orz_common_search(disorder, ORZ_ARRLEN(disorder), sizeof(disorder[0]), NULL, &target), ORZ_ERROR_LT);


    EXPECT_EQ(ORZ_COMMON_SEARCH(disorder, comp, &target), 3);

    EXPECT_EQ(ORZ_COMMON_SEARCH(disorder, comp, &notfound), RETVAL_NOT_FOUND);
}


TEST(Algorithm, BinarySearch) 
{ 
    uint8_t order[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto comp = [](const void *lhs, const void *rhs) -> int {
        return (*(uint8_t *)lhs) - ((uint8_t)(uintptr_t)rhs);
    };

    EXPECT_LT(orz_binary_search(NULL, ORZ_ARRLEN(order), sizeof(order[0]), comp, (const void *)7), ORZ_ERROR_LT);
    EXPECT_LT(orz_binary_search(order, ORZ_ARRLEN(order), sizeof(order[0]), NULL, (const void *)7), ORZ_ERROR_LT);


    EXPECT_EQ(ORZ_BINARY_SEARCH(order, comp, (const void *)7), 6);
    EXPECT_EQ(ORZ_BINARY_SEARCH(order, comp, (const void *)11), RETVAL_NOT_FOUND);
}


TEST(Algorithm, tostringInError) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_ALGORITHM, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[2]: MOD_ALGORITHM, rsn[0]: RSN_NULLPTR, sub[0]");
}

