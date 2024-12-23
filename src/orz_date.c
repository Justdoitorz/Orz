/**
 * @file orz_date.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-12-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "orz_date.h"
#include "orz_retval.h"


static inline 
int32_t __calc_correct_days(uint32_t isleap, uint32_t month)
{
    /* if month >= 2, then correct = 1(is leap) or 2, 
       else correct = 0;
    */
    int32_t correct = !!(month >> 1);
    correct += correct & !isleap;
    //int32_t correct = (month >> 1) ? 2 - isleap : 0;

    return ((month + 1 + (month >> 3)) >> 1) - correct;
}

static inline 
uint32_t __calc_mdays(uint32_t isleap, uint32_t month)
{
    if (month == 2) return 28 | isleap;

    return 30 | ((month & 1) ^ (month >> 3));
}

static 
uint32_t __date_mdays(uint32_t year, uint32_t month)
{
    return __calc_mdays(orz_date_isleap(year), month);
}

static 
uint32_t __date_accum_days(uint32_t year, uint32_t month)
{
    uint32_t days = month * 30;
    int32_t correct = __calc_correct_days(orz_date_isleap(year), month);
    
    return days + correct;
}

static 
uint32_t __date_calc_month(uint32_t year, uint32_t days)
{
    uint32_t month = days / 30;
    int32_t remain = days % 30;

    int32_t correct = __calc_correct_days(orz_date_isleap(year), month);

    return month + (remain > correct);
}

static 
uint32_t __date_week(uint32_t year, uint32_t month, uint32_t day)
{
    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    uint32_t c = year / 100;
    uint32_t y = year % 100;

    uint32_t week = y + (y >> 2) + (c >> 2) + 6 * (c << 1) 
                    + 26 * (month + 1) / 10 + day - 1;

    return week % 7;
}


static 
int __leaps(int year)
{
    return ((year + 3) >> 2) - (year - 1) / 100 + (year - 1) / 400;
}

static 
int __date_calc_yday(uint32_t year, uint32_t month, uint32_t day)
{
    return __date_accum_days(year, month - 1) + day; 
}


static 
uint32_t __calc_year_and_yday(uint32_t year, uint32_t month, uint32_t day, int offset)
{
    int yday = __date_calc_yday(year, month, day) + offset;

#define DATE_BASE_YEAR  1970
    yday += year * 365 + orz_date_leaps(DATE_BASE_YEAR, year);
    year = DATE_BASE_YEAR + yday / 365 ;
    yday -= year * 365 + orz_date_leaps(DATE_BASE_YEAR, year);

    while (yday < 1) {
       --year; 
       yday += orz_date_ydays(year);
    }

    return (year << 9) | yday;
}

static 
int __date_calc(uint32_t *y, uint32_t *m, uint32_t *d, int offset)
{
    uint32_t ret = __calc_year_and_yday(*y, *m, *d, offset);
    int yday = ret & 0x1FF;
    uint32_t year = ret >> 9;

    uint32_t month = __date_calc_month(year, yday);
    uint32_t day = yday - __date_accum_days(year, month - 1);
    
    *y = year; *m = month; *d = day;

    return yday;
}

__ORZ_EXPORT
int orz_date_isleap(uint32_t year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

__ORZ_EXPORT
int orz_date_leaps(uint32_t year, uint32_t other) 
{
    return __leaps(other) - __leaps(year);
}

__ORZ_EXPORT
int orz_date_ydays(uint32_t year)
{
    return 365 + orz_date_isleap(year);
}

__ORZ_EXPORT
int orz_date_calc_month(uint32_t year, uint32_t days)
{
    if ((days - 1) >= orz_date_ydays(year)) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_EXCEED_YDAYS, 0);
    }

    return __date_calc_month(year, days);
}

__ORZ_EXPORT
int orz_date_accum_days(uint32_t year, uint32_t month)
{
    if (month > 12) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_MONTH, 1);
    }

    return __date_accum_days(year, month);
}


__ORZ_EXPORT
int orz_date_calc_yday(uint32_t year, uint32_t month, uint32_t day)
{
    if ((month - 1) >= 12) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_MONTH, 2);
    } else if ((day - 1) >= __date_mdays(year, month)) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_DAY, 2);
    }

    return __date_calc_yday(year, month, day);
}

__ORZ_EXPORT
int orz_date_mdays(uint32_t year, uint32_t month)
{
    if ((month - 1) >= 12) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_MONTH, 3);
    }

    return __date_mdays(year, month);
}


__ORZ_EXPORT
int orz_date_week(uint32_t year, uint32_t month, uint32_t day)
{
    if ((month - 1) >= 12) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_MONTH, 4);
    } else if ((day - 1) >= __date_mdays(year, month)) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_DAY, 4);
    }

    return __date_week(year, month, day);
}



__ORZ_EXPORT
int orz_date_calc(uint32_t *y, uint32_t *m, uint32_t *d, int offset)
{
    if (y == NULL || m == NULL || d == NULL) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_NULLPTR, 5);
    } else if ((*m - 1) >= 12) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_MONTH, 5);
    } else if ((*d - 1) >= __date_mdays(*y, *m)) {
        return ORZ_RETVAL_ERROR(MOD_DATE, RSN_WRONG_DAY, 5);
    }

    return __date_calc(y, m, d, offset);
}

