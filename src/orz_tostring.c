/**
 * @file orz_tostring.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "orz_tostring.h"


static const char * const boolstr[] = {
    "False",
    "True",
};

__ORZ_EXPORT
const char *orz_tostring_bool(uint32_t xbool)
{
    return ORZ_TOSTRING_EASY(boolstr, xbool);
}

