#include <gtest/gtest.h>

#include "orz_date.h"
#include "orz_retval.h"


TEST(Date, isleap) 
{
    EXPECT_EQ(false, orz_date_isleap(1900));
    EXPECT_EQ(false, orz_date_isleap(1901));
    EXPECT_EQ(true, orz_date_isleap(1904));
    EXPECT_EQ(true, orz_date_isleap(2000));
}

TEST(Date, leaps) 
{
    EXPECT_EQ(orz_date_leaps(2000, 2000), 0);
    EXPECT_EQ(orz_date_leaps(2000, 2001), 1);
    EXPECT_EQ(orz_date_leaps(2000, 2004), 1);
    EXPECT_EQ(orz_date_leaps(2000, 2005), 2);

    EXPECT_EQ(orz_date_leaps(2001, 2000), -1);
    EXPECT_EQ(orz_date_leaps(2004, 2000), -1);
    EXPECT_EQ(orz_date_leaps(2005, 2000), -2);
}

TEST(Date, ydays)
{
    EXPECT_EQ(365, orz_date_ydays(2100));
    EXPECT_EQ(365, orz_date_ydays(2101));

    EXPECT_EQ(366, orz_date_ydays(2004));
    EXPECT_EQ(366, orz_date_ydays(2000));
}

TEST(Date, mdays)
{
    EXPECT_LT(orz_date_mdays(2024, 0),  ORZ_ERROR_LT);
    EXPECT_LT(orz_date_mdays(2024, 13), ORZ_ERROR_LT);

    EXPECT_EQ(orz_date_mdays(2024, 1), 31);

    EXPECT_EQ(orz_date_mdays(2023, 2), 28);
    EXPECT_EQ(orz_date_mdays(2004, 2), 29);

    EXPECT_EQ(orz_date_mdays(2024, 3), 31);
    EXPECT_EQ(orz_date_mdays(2024, 4), 30);
    EXPECT_EQ(orz_date_mdays(2024, 5), 31);
    EXPECT_EQ(orz_date_mdays(2024, 6), 30);
    EXPECT_EQ(orz_date_mdays(2024, 7), 31);
    EXPECT_EQ(orz_date_mdays(2024, 8), 31);
    EXPECT_EQ(orz_date_mdays(2024, 9), 30);
    EXPECT_EQ(orz_date_mdays(2024, 10), 31); 
    EXPECT_EQ(orz_date_mdays(2024, 11), 30); 
    EXPECT_EQ(orz_date_mdays(2024, 12), 31); 
}

TEST(Date, accum_days) 
{
    EXPECT_LT(orz_date_accum_days(2023, 13), ORZ_ERROR_LT);

    EXPECT_EQ(orz_date_accum_days(2023, 0), 0);
    EXPECT_EQ(orz_date_accum_days(2023, 1), 31);
    EXPECT_EQ(orz_date_accum_days(2023, 2), 31 + 28);
    EXPECT_EQ(orz_date_accum_days(2023, 3), 31 + 28 + 31);
    EXPECT_EQ(orz_date_accum_days(2023, 4), 31 + 28 + 31 + 30);
    EXPECT_EQ(orz_date_accum_days(2023, 5), 31 + 28 + 31 + 30 + 31);
    EXPECT_EQ(orz_date_accum_days(2023, 6), 31 + 28 + 31 + 30 + 31 + 30);
    EXPECT_EQ(orz_date_accum_days(2023, 7), 31 + 28 + 31 + 30 + 31 + 30 + 31);
    EXPECT_EQ(orz_date_accum_days(2023, 8), 365 - 31 - 30 - 31 - 30);
    EXPECT_EQ(orz_date_accum_days(2023, 9), 365 - 31 - 30 - 31);
    EXPECT_EQ(orz_date_accum_days(2023, 10), 365 - 31 - 30);
    EXPECT_EQ(orz_date_accum_days(2023, 11), 365 - 31);
    EXPECT_EQ(orz_date_accum_days(2023, 12), 365);

    EXPECT_EQ(orz_date_accum_days(2024, 0), 0);
    EXPECT_EQ(orz_date_accum_days(2024, 1), 31);
    EXPECT_EQ(orz_date_accum_days(2024, 2), 31 + 29);
    EXPECT_EQ(orz_date_accum_days(2024, 3), 31 + 29 + 31);
    EXPECT_EQ(orz_date_accum_days(2024, 4), 31 + 29 + 31 + 30);
    EXPECT_EQ(orz_date_accum_days(2024, 5), 31 + 29 + 31 + 30 + 31);
    EXPECT_EQ(orz_date_accum_days(2024, 6), 31 + 29 + 31 + 30 + 31 + 30);
    EXPECT_EQ(orz_date_accum_days(2024, 7), 31 + 29 + 31 + 30 + 31 + 30 + 31);
    EXPECT_EQ(orz_date_accum_days(2024, 8), 366 - 31 - 30 - 31 - 30);
    EXPECT_EQ(orz_date_accum_days(2024, 9), 366 - 31 - 30 - 31);
    EXPECT_EQ(orz_date_accum_days(2024, 10), 366 - 31 - 30);
    EXPECT_EQ(orz_date_accum_days(2024, 11), 366 - 31);
    EXPECT_EQ(orz_date_accum_days(2024, 12), 366);
}

TEST(Date, calc_yday)
{
    EXPECT_LT(orz_date_calc_yday(2023, 0, 1), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_yday(2023, 13, 1), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_yday(2023, 1, 0), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_yday(2023, 1, 32), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_yday(2023, 2, 29), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_yday(2000, 2, 30), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_yday(2023, 4, 31), ORZ_ERROR_LT);


    EXPECT_EQ(orz_date_calc_yday(2023, 1, 1), 1);
    EXPECT_EQ(orz_date_calc_yday(2023, 1, 31), 31);
    EXPECT_EQ(orz_date_calc_yday(2023, 2, 1), 32);
    EXPECT_EQ(orz_date_calc_yday(2023, 2, 28), 59);
    EXPECT_EQ(orz_date_calc_yday(2023, 3, 1), 60);
    EXPECT_EQ(orz_date_calc_yday(2023, 3, 31), 90);
    EXPECT_EQ(orz_date_calc_yday(2023, 4, 1), 91);
    EXPECT_EQ(orz_date_calc_yday(2023, 4, 30), 120);
    EXPECT_EQ(orz_date_calc_yday(2023, 5, 1), 121);
    EXPECT_EQ(orz_date_calc_yday(2023, 5, 31), 151);
    EXPECT_EQ(orz_date_calc_yday(2023, 6, 1), 152);
    EXPECT_EQ(orz_date_calc_yday(2023, 6, 30), 181);

    EXPECT_EQ(orz_date_calc_yday(2000, 1, 1), 1);
    EXPECT_EQ(orz_date_calc_yday(2000, 1, 31), 31);
    EXPECT_EQ(orz_date_calc_yday(2000, 2, 1), 32);
    EXPECT_EQ(orz_date_calc_yday(2000, 2, 28), 59);
    EXPECT_EQ(orz_date_calc_yday(2000, 2, 29), 60);
    EXPECT_EQ(orz_date_calc_yday(2000, 3, 1), 61);

    EXPECT_EQ(orz_date_calc_yday(2023, 12, 31), 365);
    EXPECT_EQ(orz_date_calc_yday(2000, 12, 31), 366);
}

TEST(Date, calc_month)
{
    EXPECT_LT(orz_date_calc_month(2023, 0), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_month(2023, 366), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_calc_month(2024, 367), ORZ_ERROR_LT);

    EXPECT_EQ(orz_date_calc_month(2023, 1), 1);
    EXPECT_EQ(orz_date_calc_month(2023, 31), 1);
    EXPECT_EQ(orz_date_calc_month(2023, 32), 2);
    EXPECT_EQ(orz_date_calc_month(2023, 59), 2);
    EXPECT_EQ(orz_date_calc_month(2023, 60), 3);
    EXPECT_EQ(orz_date_calc_month(2023, 90), 3); // 3月31日
    EXPECT_EQ(orz_date_calc_month(2023, 91), 4); // 4月1日

    EXPECT_EQ(orz_date_calc_month(2023, 365), 12);
    EXPECT_EQ(orz_date_calc_month(2024, 366), 12);

    EXPECT_EQ(orz_date_calc_month(2024, 59), 2); // 闰年，2月28日
    EXPECT_EQ(orz_date_calc_month(2024, 60), 2); // 闰年，2月29日
    EXPECT_EQ(orz_date_calc_month(2024, 61), 3); // 闰年，3月1日
}


TEST(Date, week)  
{
    EXPECT_LT(orz_date_week(2023, 0, 1), ORZ_ERROR_LT);
    EXPECT_LT(orz_date_week(2023, 13, 1), ORZ_ERROR_LT);

    //2023.1.1 is Sunday
    EXPECT_EQ(Sun, orz_date_week(2023, 1, 1));

    //2023.12.31 is Sunday
    EXPECT_EQ(Sun, orz_date_week(2023, 12, 31));

    const uint32_t year = 2022;
    const uint32_t firstday_week = Sat;
    for (uint32_t days = 1; days <= orz_date_ydays(year); ++days) {
        uint32_t month = orz_date_calc_month(year, days);
        uint32_t day = days - orz_date_accum_days(year, month - 1);
        EXPECT_EQ((days - 1 + firstday_week) % 7, orz_date_week(year, month, day));
    }
}


static int __wrong_calc(uint32_t year, uint32_t month, uint32_t day)
{
    return orz_date_calc(&year, &month, &day, 0);
}

TEST(Date, calc)
{
    EXPECT_LT(orz_date_calc(NULL, NULL, NULL, 0), ORZ_ERROR_LT);
    EXPECT_LT(__wrong_calc(2024, 0, 21), ORZ_ERROR_LT);
    EXPECT_LT(__wrong_calc(2024, 13, 21), ORZ_ERROR_LT);
    EXPECT_LT(__wrong_calc(2024, 12, 0), ORZ_ERROR_LT);
    EXPECT_LT(__wrong_calc(2024, 12, 32), ORZ_ERROR_LT);

    uint32_t year = 2024;
    uint32_t month = 12;
    uint32_t day = 31;
    orz_date_calc(&year, &month, &day, 1);
    EXPECT_EQ(year, 2025);
    EXPECT_EQ(month, 1);
    EXPECT_EQ(day, 1);

    orz_date_calc(&year, &month, &day, 365);
    EXPECT_EQ(year, 2026);
    EXPECT_EQ(month, 1);
    EXPECT_EQ(day, 1);

    orz_date_calc(&year, &month, &day, -365-366);
    EXPECT_EQ(year, 2024);
    EXPECT_EQ(month, 1);
    EXPECT_EQ(day, 1);
}

TEST(Date, tostringInError)
{
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_DATE, RSN_NULLPTR, 5), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[3]: MOD_DATE, rsn[0]: RSN_NULLPTR, sub[5]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_DATE, RSN_EXCEED_YDAYS, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[3]: MOD_DATE, rsn[4]: RSN_EXCEED_YDAYS, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_MONTH, 1), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[3]: MOD_DATE, rsn[5]: RSN_WRONG_MONTH, sub[1]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_DAY, 2), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[3]: MOD_DATE, rsn[6]: RSN_WRONG_DAY, sub[2]");
}
