/**
 * @file orz_retval.c
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "orz_retval.h"
#include "orz_tostring.h"
#include "orz_util.h"

#include <stdio.h>

static const char *tostring_module(uint32_t module) 
{
    static const char * const module_str[] = {
        ORZ_XSTR(MOD_RETVAL),
        ORZ_XSTR(MOD_UTIL),
        ORZ_XSTR(MOD_ALGORITHM),
        ORZ_XSTR(MOD_DATE),
    };

    return ORZ_TOSTRING_EASY(module_str, module);
}


static const char *tostring_reason(uint32_t reason) 
{
    static const char * const reason_str[] = {
        ORZ_XSTR(RSN_NULLPTR),
        ORZ_XSTR(RSN_SPACE_NO_ENOUGH),
        ORZ_XSTR(RSN_ZERO_LENGTH),
        ORZ_XSTR(RSN_WRONG_LENGTH),
        ORZ_XSTR(RSN_EXCEED_YDAYS),
        ORZ_XSTR(RSN_WRONG_MONTH),
        ORZ_XSTR(RSN_WRONG_DAY),
    };

    return ORZ_TOSTRING_EASY(reason_str, reason);
}

__ORZ_EXPORT
const char *orz_retval_reason(uint32_t reason) 
{
    return tostring_reason(reason);
}

static int default_error_tostring(int error, char *str, uint32_t size)
{
    uint32_t module = ERROR_GET_MODULE(error);
    uint32_t reason = ERROR_GET_REASON(error);
    uint32_t subseq = ERROR_GET_SUBSEQ(error);

    return snprintf(str, size, "Mod[%u], Rsn[%u], Sub[%u]", module, reason, subseq);
}

static int default_usr_tostring(int retval, char *str, uint32_t size)
{
    if (retval < ORZ_ERROR_LT) {
        return default_error_tostring(retval, str, size);
    } else {
        return snprintf(str, size, "Ret[%d]", retval);
    }
}

static int (*usr_tostring)(int, char *, uint32_t) = default_usr_tostring;

__ORZ_EXPORT
int orz_retval_reg(int (*tostring)(int, char *, uint32_t))
{
    if (tostring == NULL) return ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_NULLPTR, 0);

    usr_tostring = tostring;

    return RETVAL_OK;
}

static int error_tostring(int error, char *str, uint32_t size)
{
    uint32_t module = ERROR_GET_MODULE(error);
    uint32_t reason = ERROR_GET_REASON(error);
    uint32_t subseq = ERROR_GET_SUBSEQ(error);

    if (module < MOD_USR_MINIMUM) {
        if (reason < RSN_USR_MINIMUM) {
            return snprintf(str, size,
                "mod[%u]: %s, rsn[%u]: %s, sub[%u]",
                module, tostring_module(module), 
                reason, tostring_reason(reason), subseq);
        } else {
            return snprintf(str, size, 
                "MOD[%u], RSN[%u], SUB[%u]", 
                module, reason, subseq);
        }
    } else {
        return usr_tostring(error, str, size);
    }
}


static const char *tostring_retval(int retval) 
{
    static const char * const retval_str[] = {
        ORZ_XSTR(RETVAL_NOT_FOUND),
        ORZ_XSTR(RETVAL_MOMORY_EMPTY), 
    };

    return ORZ_TOSTRING_DFT(retval_str, retval - RETVAL_MINIMUM, "Out of retval");
}

static int retval_tostring(int retval, char *str, uint32_t size)
{
    if (retval < RETVAL_MINIMUM) {
        return usr_tostring(retval, str, size);
    } else if (retval < 0) {
        return snprintf(str, size, 
            "ret[%d]: %s", retval, tostring_retval(retval));
    } else {
        return snprintf(str, size, "ret[%d]", retval); 
    }
}

__ORZ_EXPORT
int orz_retval_tostring(int retval, char *str, uint32_t size)
{
    if (str == NULL) return ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_NULLPTR, 1);
    if (size < RETVAL_STRING_SIZE) return ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_SPACE_NO_ENOUGH, 1);

    if (retval < ORZ_ERROR_LT) {
        return error_tostring(retval, str, size);
    } else {
        return retval_tostring(retval, str, size);
    } 
}

