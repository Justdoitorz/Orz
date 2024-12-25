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

/**
 * @enum Week
 * @brief Enum representing the days of the week.
 */
enum Week {
    Sun = 0, /**< Sunday */
    Mon,     /**< Monday */
    Tues,    /**< Tuesday */
    Wed,     /**< Wednesday */
    Thur,    /**< Thursday */
    Fri,     /**< Friday */
    Sat,     /**< Saturday */
};

/**
 * @enum Month
 * @brief Enum representing the months of the year.
 */
enum Month {
    Jan = 1, /**< January */
    Feb,     /**< February */
    Mar,     /**< March */
    Apr,     /**< April */
    May,     /**< May */
    Jun,     /**< June */
    Jul,     /**< July */
    Aug,     /**< August */
    Sep,     /**< September */
    Oct,     /**< October */
    Nov,     /**< November */
    Dec,     /**< December */
};


__ORZ_C_ENTER__

/**
 * @brief Check if a year is a leap year.
 *
 * @param year The year to check.
 * @retval 0, Not a leap year.
 * @retval 1, A leap year.
 */
int orz_date_isleap(uint32_t year);

/**
 * @brief Calculate the number of leap years between two years[min, max), if year > other, it is negative.
 *
 * @param year a year
 * @param other other year
 * @retval < 0, in case year > other, it is negative.
 * @retval > 0, in case year < other, it is positive.
 * @retval 0, in case year == other.
 */
int orz_date_leaps(uint32_t year, uint32_t other);

/**
 * @brief Calculate the number of days in a year.
 *
 * @param year The year to check.
 * @retval 365, Not a leap year.
 * @retval 366, A leap year.
 */
int orz_date_ydays(uint32_t year);

/**
 * @brief Calculate the number of days in a month.
 *
 * @param year The year of the month.
 * @param month The month to check.
 * @retval < 0, Error
 * @retval else, The number of days in the month.
 */
int orz_date_mdays(uint32_t year, uint32_t month);

/**
 * @brief Calculate the accumulated number of days up to a given month in a year.
 *
 * @param year The year to check.
 * @param month The month to check.
 * @retval < 0, Error
 * @retval else, The accumulated number of days.
 */
int orz_date_accum_days(uint32_t year, uint32_t month);

/**
 * @brief Calculate the month from the number of days in a year.
 *
 * @param year The year to check.
 * @param days The number of days to convert.
 * @retval < 0, Error
 * @retval else, The month of the year.
 */
int orz_date_calc_month(uint32_t year, uint32_t days);

/**
 * @brief Calculate the day of the year from a given date.
 *
 * @param year The year of the date.
 * @param month The month of the date.
 * @param day The day of the date.
 * @retval < 0, Error
 * @retval else, The day of the year.
 */
int orz_date_calc_yday(uint32_t year, uint32_t month, uint32_t day);

/**
 * @brief Calculate the week number from a given date.
 *
 * @param year The year of the date.
 * @param month The month of the date.
 * @param day The day of the date.
 * @retval < 0, Error
 * @retval else, The week number of the date.
 */
int orz_date_week(uint32_t year, uint32_t month, uint32_t day);

/**
 * @brief Calculate the date after adding an offset to a given date.
 *
 * @param[inout] year Pointer to the year of the date.
 * @param[inout] month Pointer to the month of the date.
 * @param[inout] day Pointer to the day of the date.
 * @param offset The number of days to add to the date.
 * @retval < 0, Error
 * @retval else , The day of the year.
 */
int orz_date_calc(uint32_t *year, uint32_t *month, uint32_t *day, int offset);

__ORZ_C_LEAVE__


#endif //__ORZ_DATE_H__
