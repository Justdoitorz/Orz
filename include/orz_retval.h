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
    MOD_DATE,
    MOD_ALLOC,
    MOD_BUFFER,


    MOD_USR_MINIMUM          = 1 << (ERROR_MODULE_LEN - 1),
    //
    MOD_USR_MAXIMUM          = ERROR_MODULE_MSK,
};

enum {
    RSN_NULLPTR             = 0,
    RSN_SPACE_NO_ENOUGH,
    RSN_ZERO_LENGTH,
    RSN_WRONG_LENGTH,
    RSN_EXCEED_YDAYS,
    RSN_WRONG_MONTH,
    RSN_WRONG_DAY,
    RSN_LARGE_LENGTH,
    RSN_OUT_OF_RANGE,

    RSN_USR_MINIMUM         = 1 << (ERROR_REASON_LEN - 1),
    //
    RSN_USR_MAXIMUM         = ERROR_REASON_MSK,
};



__ORZ_C_ENTER__

/**
 * @brief Get the string representation of an error reason.
 *
 * @param reason The error reason code.
 * @retval non-NULL, The string representation.
 */
const char *orz_retval_reason(uint32_t reason);

/**
 * @brief Convert a return value to a string.
 *
 * @param retval The return value to convert.
 * @param[out] str The buffer to store the string representation.
 * @param size The size of the buffer.
 * @retval < 0, Error
 * @retval else, The number of characters written.
 */
int orz_retval_tostring(int retval, char *str, uint32_t size);

/**
 * @brief Register a user-defined error handling function.
 *
 * @param usr The user-defined error handling function.
 * @retval < 0, Error
 * @retval 0, OK
 */
int orz_retval_reg(int (*usr)(int, char *, uint32_t));

__ORZ_C_LEAVE__




#endif //__ORZ_RETVAL_H__

