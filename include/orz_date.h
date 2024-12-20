/**
 * @file orz_date.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-12-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_DATE_H__
#define __ORZ_DATE_H__

#include "orz_cdefs.h"
#include "orz_types.h"


enum Week {
    Sun = 0,
    Mon,
    Tues,
    Wed,
    Thur,
    Fri,
    Sat,
};


enum Month {
    Jan = 1,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec,
};


__ORZ_C_ENTER__

int orz_date_isleap(uint32_t year);
int orz_date_leaps(uint32_t year, uint32_t other);

int orz_date_ydays(uint32_t year);
int orz_date_mdays(uint32_t year, uint32_t month);
int orz_date_accum_days(uint32_t year, uint32_t month);
int orz_date_calc_month(uint32_t year, uint32_t days);
int orz_date_calc_yday(uint32_t year, uint32_t month, uint32_t day);
int orz_date_week(uint32_t year, uint32_t month, uint32_t day);

int orz_date_calc(uint32_t *year, uint32_t *month, uint32_t *day, int offset);

__ORZ_C_LEAVE__


#endif //__ORZ_DATE_H__
