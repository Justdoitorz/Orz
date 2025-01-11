#include <gtest/gtest.h>

#include "orz_mutex.h"
#include "orz_alloc.h"
#include "orz_retval.h"
#include <pthread.h>
#include <mutex>
#include <thread>
#include <vector>

enum {
    MOD_POSIX_MUTEX = MOD_USR_MINIMUM,
    MOD_CXX_MUTEX,
};

static int posix_mutex_create(void **mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_POSIX_MUTEX, RSN_NULLPTR, 0);

    void *tmp_mutex = orz_malloc(sizeof(pthread_mutex_t));
    if (tmp_mutex == NULL) return RETVAL_MEMORY_EMPTY;

    if (0 != pthread_mutex_init((pthread_mutex_t *)tmp_mutex, NULL)) {
        orz_free(tmp_mutex);
        return RETVAL_INIT_FAILED;
    }

    *mutex = tmp_mutex;

    return 0;
}

static int posix_mutex_destroy(void *mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_POSIX_MUTEX, RSN_NULLPTR, 1);

    pthread_mutex_destroy((pthread_mutex_t *)mutex);
    orz_free(mutex);

    return 0;
}

static int posix_mutex_lock(void *mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_POSIX_MUTEX, RSN_NULLPTR, 2);

    pthread_mutex_lock((pthread_mutex_t *)mutex);

    return 0;
}

static int posix_mutex_unlock(void *mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_POSIX_MUTEX, RSN_NULLPTR, 3);

    pthread_mutex_unlock((pthread_mutex_t *)mutex);

    return 0;
}

static const struct orz_mutex_ops posix_ops = {
    .create = posix_mutex_create,
    .destroy = posix_mutex_destroy,
    .lock = posix_mutex_lock,
    .unlock = posix_mutex_unlock,
};

#define USE_CXX_ALLOC    0

static int cxx_mutex_create(void **mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_CXX_MUTEX, RSN_NULLPTR, 0);

#if USE_CXX_ALLOC
    auto tmp_mutex = new std::mutex();
#else
    void *tmp_mutex = orz_malloc(sizeof(std::mutex));
    new (tmp_mutex) std::mutex();
#endif

    *mutex = tmp_mutex;

    return 0;
}

static int cxx_mutex_destroy(void *mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_CXX_MUTEX, RSN_NULLPTR, 1);
    
#if USE_CXX_ALLOC
    delete static_cast<std::mutex *>(mutex);
#else 
    static_cast<std::mutex *>(mutex)->~mutex();
    orz_free(mutex);
#endif

    return 0;
}

static int cxx_mutex_lock(void *mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_CXX_MUTEX, RSN_NULLPTR, 2);

    static_cast<std::mutex *>(mutex)->lock();

    return 0;
}

static int cxx_mutex_unlock(void *mutex)
{
    if (mutex == NULL) return ORZ_RETVAL_ERROR(MOD_CXX_MUTEX, RSN_NULLPTR, 3);

    static_cast<std::mutex *>(mutex)->unlock();

    return 0;
}


static const struct orz_mutex_ops cxx_ops = {
    .create = cxx_mutex_create,
    .destroy = cxx_mutex_destroy,
    .lock = cxx_mutex_lock,
    .unlock = cxx_mutex_unlock,
};

TEST(Mutex, Init) {
    int ret;
    ret = orz_mutex_init(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_mutex_init(&cxx_ops);
    EXPECT_EQ(ret, RETVAL_OK);
}

TEST(Mutex, Create_Destory)
{
    int ret;
    ret = orz_mutex_create(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);
    ret = orz_mutex_destroy(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);
    
    void *mutex = NULL;
    ret = orz_mutex_create(&mutex);
    EXPECT_EQ(ret, RETVAL_OK);

    ret = orz_mutex_destroy(mutex);
    EXPECT_EQ(ret, RETVAL_OK);
}

uint32_t foo(uint32_t threadnums, uint32_t times, void *mutex)
{
    uint32_t count = 0;
    std::vector<std::thread> threads;

    for (int idx = 0; idx < threadnums; ++idx) {
        threads.emplace_back([=, &count] () { 
            for (int idx = 0; idx < times; ++idx) {
                orz_mutex_lock(mutex);
                ++count;
                orz_mutex_unlock(mutex);
                usleep(1);
            }
        });
    }

    for (auto &th : threads) th.join();

    return count;
}

TEST(Mutex, Lock_Unlock)
{
    int ret;
    ret = orz_mutex_lock(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    ret = orz_mutex_unlock(NULL);
    EXPECT_LT(ret, ORZ_ERROR_LT);

    void *mutex = NULL;
    orz_mutex_create(&mutex);

    ret = orz_mutex_lock(mutex);
    EXPECT_EQ(ret, RETVAL_OK);

    ret = orz_mutex_unlock(mutex);
    EXPECT_EQ(ret, RETVAL_OK);


    const int threadnums = 10;
    const uint32_t times = 1000; 

    EXPECT_EQ(threadnums * times, foo(threadnums, times, mutex));

    orz_mutex_destroy(mutex);
}

TEST(Mutex, Deinit)
{
    int ret = orz_mutex_deinit();
    EXPECT_EQ(ret, RETVAL_OK);
}

TEST(Mutex, tostringInError) {
    char str[RETVAL_STRING_SIZE];
    int ret;

    ret = orz_retval_tostring(ORZ_RETVAL_ERROR(MOD_MUTEX, RSN_NULLPTR, 0), str, sizeof(str));
    ASSERT_GE(ret, 0);
    EXPECT_STREQ(str, "mod[6]: MOD_MUTEX, rsn[0]: RSN_NULLPTR, sub[0]");
}
