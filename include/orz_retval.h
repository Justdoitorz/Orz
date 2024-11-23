/**
 * @file orz_retval.h
 * @author Orz (justdoit_dog@163.com)
 * @brief For the freedom!
 * @version 0.1
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __ORZ_RETVAL_H__
#define __ORZ_RETVAL_H__

#include "orz_cdefs.h"
#include "orz_types.h"

#define MAKE_MASK32(len)      (0xFFFFFFFFu ^ (0xFFFFFFFFu << (len)))

#define ERROR_MODULE_POS    20
#define ERROR_MODULE_LEN    10
#define ERROR_MODULE_MSK    MAKE_MASK32(ERROR_MODULE_LEN)

#define ERROR_REASON_POS    8
#define ERROR_REASON_LEN    12
#define ERROR_REASON_MSK    MAKE_MASK32(ERROR_REASON_LEN)

#define ERROR_SUBSEQ_POS    0
#define ERROR_SUBSEQ_LEN    8
#define ERROR_SUBSEQ_MSK    MAKE_MASK32(ERROR_SUBSEQ_LEN)

#define VMP_SET_ERROR(value, mask, pos)     (((value) & (mask)) << (pos))
#define EMP_GET_VALUE(error, mask, pos)     (((error) >> (pos)) & (mask))

#define ERROR_GET_MODULE(e)     EMP_GET_VALUE(e, ERROR_MODULE_MSK, ERROR_MODULE_POS)
#define ERROR_GET_REASON(e)     EMP_GET_VALUE(e, ERROR_REASON_MSK, ERROR_REASON_POS)
#define ERROR_GET_SUBSEQ(e)     EMP_GET_VALUE(e, ERROR_SUBSEQ_MSK, ERROR_SUBSEQ_POS)

/**
 *     3                   2                   1                   0
 *   1 0 9 8 7 6 5 4|3 2 1 0 9 8 7 6|5 4 3 2 1 0 9 8|7 6 5 4 3 2 1 0
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |1|0|      module       |        reason         |    subseq     |
 */
#define ORZ_RETVAL_ERROR(m, r, s)   (\
    (1 << 31) | \
    VMP_SET_ERROR(m, ERROR_MODULE_MSK, ERROR_MODULE_POS) |\
    VMP_SET_ERROR(r, ERROR_REASON_MSK, ERROR_REASON_POS) |\
    VMP_SET_ERROR(s, ERROR_SUBSEQ_MSK, ERROR_SUBSEQ_POS)  \
)

#define ORZ_ERROR_LT        (-65535) //It can be least 0xC0000000
#define RETVAL_STRING_SIZE  (256)


enum {
    RETVAL_USR_MINIMUM      = -255,
    //
    RETVAL_USR_MAXIMUM      = -128,
    

    RETVAL_MINIMUM          = -127,
    RETVAL_NOT_FOUND        = RETVAL_MINIMUM, 
    RETVAL_MOMORY_EMPTY, 


/***************************************/   
    RETVAL_OK               = 0,

    RETVAL_MAXIMUM          = 127,
};

enum {
    MOD_RETVAL              = 0,
    MOD_UTIL,                
    MOD_ALGORITHM,


    MOD_USR_MINIMUM          = 1 << (ERROR_MODULE_LEN - 1),
    //
    MOD_USR_MAXIMUM          = ERROR_MODULE_MSK,
};

enum {
    RSN_NULLPTR             = 0,
    RSN_SPACE_NO_ENOUGH     = 1,

    RSN_USR_MINIMUM         = 1 << (ERROR_REASON_LEN - 1),
    //
    RSN_USR_MAXIMUM         = ERROR_REASON_MSK,
};



__ORZ_C_ENTER__

const char *orz_retval_reason(uint32_t reason);
int orz_retval_tostring(int retval, char *str, uint32_t size);
int orz_retval_reg(int (*usr)(int, char *, uint32_t));

__ORZ_C_LEAVE__




#endif //__ORZ_RETVAL_H__

