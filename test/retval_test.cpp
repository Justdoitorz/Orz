#include <gtest/gtest.h>

#include "orz_retval.h"
#include "orz_tostring.h"
#include "orz_util.h"

TEST(Retval, reason) {
    EXPECT_STREQ("RSN_NULLPTR", orz_retval_reason(RSN_NULLPTR));
    EXPECT_STREQ("RSN_SPACE_NO_ENOUGH", orz_retval_reason(RSN_SPACE_NO_ENOUGH));
    EXPECT_STREQ("RSN_ZERO_LENGTH", orz_retval_reason(RSN_ZERO_LENGTH));
    EXPECT_STREQ("RSN_WRONG_LENGTH", orz_retval_reason(RSN_WRONG_LENGTH));
    EXPECT_STREQ("RSN_EXCEED_YDAYS", orz_retval_reason(RSN_EXCEED_YDAYS));
    EXPECT_STREQ("RSN_WRONG_MONTH", orz_retval_reason(RSN_WRONG_MONTH));
    EXPECT_STREQ("RSN_WRONG_DAY", orz_retval_reason(RSN_WRONG_DAY));
    EXPECT_STREQ("RSN_LARGE_LENGTH", orz_retval_reason(RSN_LARGE_LENGTH));
    EXPECT_STREQ("RSN_OUT_OF_RANGE", orz_retval_reason(RSN_OUT_OF_RANGE));

    EXPECT_STREQ("Invalid", orz_retval_reason(RSN_USR_MINIMUM));
}

TEST(Retval, tostringIncorrectExample) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(0, NULL, sizeof(str));
    ASSERT_LT(ret, ORZ_ERROR_LT);

    ret = orz_retval_tostring(0, str, 0);
    ASSERT_LT(ret, ORZ_ERROR_LT);
    ret = orz_retval_tostring(0, str, RETVAL_STRING_SIZE - 1);
    ASSERT_LT(ret, ORZ_ERROR_LT);
}


TEST(Retval, tostringInRetval) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(RETVAL_NOT_FOUND, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "ret[-127]: RETVAL_NOT_FOUND");

    ret = orz_retval_tostring(-1, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "ret[-1]: Out of retval");

    ret = orz_retval_tostring(-128, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Ret[-128]");

    ret = orz_retval_tostring(0, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "ret[0]");
}

TEST(Retval, tostringInError) {
    char str[RETVAL_STRING_SIZE];
    int ret;
    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[0]: MOD_RETVAL, rsn[0]: RSN_NULLPTR, sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_NULLPTR, 1), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[0]: MOD_RETVAL, rsn[0]: RSN_NULLPTR, sub[1]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_SPACE_NO_ENOUGH, 1), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[0]: MOD_RETVAL, rsn[1]: RSN_SPACE_NO_ENOUGH, sub[1]");


    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(256, 1024, 1), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[256]: Invalid, rsn[1024]: Invalid, sub[1]");
}

TEST(Retval, tostringInUsrErrorWithoutReg) {
    char str[RETVAL_STRING_SIZE];
    char expect[RETVAL_STRING_SIZE];

    for (uint32_t module = MOD_USR_MINIMUM; module <= MOD_USR_MAXIMUM; module += 7) {
        for (uint32_t reason = RSN_USR_MINIMUM; reason <= RSN_USR_MAXIMUM; reason += 31) {
            uint32_t subseq = (module + reason) & ERROR_SUBSEQ_MSK;
            snprintf(expect, sizeof(expect), "Mod[%u], Rsn[%u], Sub[%u]", module, reason, subseq);
            
            int ret = orz_retval_tostring(ORZ_RETVAL_ERROR(module, reason, subseq), str, sizeof(str));
            ASSERT_GE(ret, 0);
            EXPECT_STREQ(str, expect);
        }
    }
}

enum {
    MOD_USR_TEST = MOD_USR_MINIMUM,
    MOD_USR_OTHER,
};

static const char *tostring_module_usr(uint32_t module)
{
    static const char * const module_usr_str[] = {
        ORZ_XSTR(MOD_USR_TEST),
        ORZ_XSTR(MOD_USR_OTHER),
    };

    return ORZ_TOSTRING_DFT(module_usr_str, module - MOD_USR_MINIMUM, "Out of usr module");
}

enum {
    RSN_USR_TEST = RSN_USR_MINIMUM,
    RSN_USR_OTHER,
};

static const char *tostring_reason_usr(uint32_t reason)
{
    static const char * const reason_usr_str[] = {
        ORZ_XSTR(RSN_USR_TEST),
        ORZ_XSTR(RSN_USR_OTHER),
    };

    return ORZ_TOSTRING_DFT(reason_usr_str, reason - RSN_USR_MINIMUM, "Out of usr reason");
}

static const char *tostring_reason_ex(uint32_t reason)
{
    return reason < RSN_USR_MINIMUM ? 
        orz_retval_reason(reason) : tostring_reason_usr(reason);
}

static int usr_error_tostring(int error, char *str, uint32_t size)
{
    uint32_t module = ERROR_GET_MODULE(error);
    uint32_t reason = ERROR_GET_REASON(error);
    uint32_t subseq = ERROR_GET_SUBSEQ(error);

    return snprintf(str, size, "Mod[%u]: %s, Rsn[%u]: %s, Sub[%u]",
        module, tostring_module_usr(module), 
        reason, tostring_reason_ex(reason), subseq);
}

enum {
    RETVAL_USR_FOR_TEST = RETVAL_USR_MINIMUM,
    RETVAL_USR_OTHER,
};


static const char *tostring_retval_usr(int retval)
{
    static const char * const retval_usr_str[] = {
        ORZ_XSTR(RETVAL_USR_FOR_TEST),
        ORZ_XSTR(RETVAL_USR_OTHER),
    };

    return ORZ_TOSTRING_DFT(retval_usr_str, retval - RETVAL_USR_MINIMUM, "Out of usr retval");
}

static int usr_tostring(int retval, char *str, uint32_t size)
{
    if (retval < ORZ_ERROR_LT) {
        return usr_error_tostring(retval, str, size);
    } else {
        return snprintf(str, size, "Ret[%d]: %s", retval, tostring_retval_usr(retval));
    }
}


TEST(Retval, reg) {
    int ret;

    ret = orz_retval_reg(NULL);
    ASSERT_LT(ret, ORZ_ERROR_LT);

    ret = orz_retval_reg(usr_tostring);
    ASSERT_EQ(ret, RETVAL_OK);
}

TEST(Retval, usrRetvalAfterReg) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(RETVAL_USR_FOR_TEST, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Ret[-255]: RETVAL_USR_FOR_TEST");

    ret = orz_retval_tostring(RETVAL_USR_OTHER, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Ret[-254]: RETVAL_USR_OTHER");

    ret = orz_retval_tostring(RETVAL_USR_MAXIMUM, str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Ret[-128]: Out of usr retval");
}

TEST(Retval, usrErrorAfterReg) {
    char str[RETVAL_STRING_SIZE];
    int ret;


    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_USR_TEST, RSN_USR_OTHER, 2), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Mod[512]: MOD_USR_TEST, Rsn[2049]: RSN_USR_OTHER, Sub[2]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_USR_OTHER, RSN_USR_TEST, 23), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Mod[513]: MOD_USR_OTHER, Rsn[2048]: RSN_USR_TEST, Sub[23]");


    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(768, 3333, 255), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Mod[768]: Out of usr module, Rsn[3333]: Out of usr reason, Sub[255]");
}


TEST(Retval, usrErrorCombainOrigin) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_USR_TEST, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "Mod[512]: MOD_USR_TEST, Rsn[0]: RSN_NULLPTR, Sub[0]");

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_RETVAL, RSN_USR_TEST, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "MOD[0], RSN[2048], SUB[0]");
}

