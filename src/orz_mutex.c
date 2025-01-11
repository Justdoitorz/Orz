#include "orz_mutex.h"
#include "orz_retval.h"

static const struct orz_mutex_ops *mutex_ops = NULL;

__ORZ_EXPORT
int orz_mutex_init(const struct orz_mutex_ops *ops)
{
    if (ops == NULL 
        || ops->create == NULL
        || ops->destroy == NULL
        || ops->lock == NULL
        || ops->unlock == NULL) return ORZ_RETVAL_ERROR(MOD_MUTEX, RSN_NULLPTR, 0);
 
    mutex_ops = ops;

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_mutex_deinit(void)
{
    mutex_ops = NULL;

    return RETVAL_OK;
}

__ORZ_EXPORT
int orz_mutex_create(void **mutex)
{
    if (mutex_ops == NULL || mutex_ops->create == NULL) 
        return ORZ_RETVAL_ERROR(MOD_MUTEX, RSN_NULLPTR, 1);
    
    return mutex_ops->create(mutex);
}

__ORZ_EXPORT
int orz_mutex_destroy(void *mutex)
{
    if (mutex_ops == NULL || mutex_ops->destroy == NULL) 
        return ORZ_RETVAL_ERROR(MOD_MUTEX, RSN_NULLPTR, 2);
    
    return mutex_ops->destroy(mutex);
}

__ORZ_EXPORT
int orz_mutex_lock(void *mutex)
{
    if (mutex_ops == NULL || mutex_ops->lock == NULL) 
        return ORZ_RETVAL_ERROR(MOD_MUTEX, RSN_NULLPTR, 3);
    
    return mutex_ops->lock(mutex);
}

__ORZ_EXPORT
int orz_mutex_unlock(void *mutex)
{
    if (mutex_ops == NULL || mutex_ops->unlock == NULL) 
        return ORZ_RETVAL_ERROR(MOD_MUTEX, RSN_NULLPTR, 4);
    
    return mutex_ops->unlock(mutex);
}
