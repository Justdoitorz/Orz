#include <gtest/gtest.h>

#include "orz_algorithm.h"


TEST(Algorithm, CommonSearch) 
{
    uint8_t disorder[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
    auto comp = [](const void *lhs, const void *rhs) -> int {
        return (*(uint8_t *)lhs) - (*(uint8_t *)rhs);
    };
    uint8_t target = 7;
    uint8_t notfound = 11;

    EXPECT_LT(orz_common_search(NULL, ORZ_ARRLEN(disorder), sizeof(disorder[0]), comp, &target), 0);
    EXPECT_LT(orz_common_search(disorder, ORZ_ARRLEN(disorder), sizeof(disorder[0]), NULL, &target), 0);


    EXPECT_EQ(ORZ_COMMON_SEARCH(disorder, comp, &target), 3);
    EXPECT_LT(ORZ_COMMON_SEARCH(disorder, comp, &notfound), 0);
}


TEST(Algorithm, BinarySearch) 
{ 
    uint8_t order[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto comp = [](const void *lhs, const void *rhs) -> int {
        return (*(uint8_t *)lhs) - ((uint8_t)(uintptr_t)rhs);
    };

    EXPECT_LT(orz_binary_search(NULL, ORZ_ARRLEN(order), sizeof(order[0]), comp, (const void *)7), 0);
    EXPECT_LT(orz_binary_search(order, ORZ_ARRLEN(order), sizeof(order[0]), NULL, (const void *)7), 0);


    EXPECT_EQ(ORZ_BINARY_SEARCH(order, comp, (const void *)7), 6);
    EXPECT_LT(ORZ_BINARY_SEARCH(order, comp, (const void *)11), 0);
}

