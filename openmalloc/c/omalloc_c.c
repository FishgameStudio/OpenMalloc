// -*- C -*-

#include "omalloc_c.h"
#include <stdlib.h>
#include <string.h>

/* The implement of omalloc_c.h */

static size_t 
om_alignAddr(size_t __addr, size_t __align) {
    return (__addr + __align - 1) & ~(__align - 1);
}

int 
om_poolInit(OM_POOL_PTR __pool, size_t __size) {
    if (__pool == NULL || __size == 0) return OM_FAIL;

    __pool->pool = malloc(__size);
    if (__pool->pool == NULL) return OM_FAIL;

    __pool->size = __size;
    pthread_mutex_init(&__pool->mtx, NULL);
    return OM_OK;
}
void 
om_poolDestroy(OM_POOL_PTR __pool) {
    if (__pool == NULL) return;

    pthread_mutex_lock(&__pool->mtx);
    free(__pool->pool);
    __pool->pool = NULL;
    __pool->size = 0;
    __pool->offset = 0;
    pthread_mutex_unlock(&__pool->mtx);

    pthread_mutex_destroy(&__pool->mtx);
}
void* 
om_malloc(OM_POOL_PTR __pool, size_t __size, size_t __align) {
    if (__pool == NULL) return NULL;

    pthread_mutex_lock(&__pool->mtx);
    size_t aligned = om_alignAddr(__pool->offset, __align);

    if (aligned + __size > __pool->size) {
        pthread_mutex_unlock(&__pool->mtx);
        return NULL;
    }

    void* ptr = __pool->pool + aligned;
    __pool->offset = aligned + __size;
    pthread_mutex_unlock(&__pool->mtx);

    return ptr;
}
int 
om_mallocable(OM_POOL_PTR __pool, size_t __size, size_t __align) {
    if (__pool == NULL) return 0;

    pthread_mutex_lock(&__pool->mtx);

    size_t aligned_offset = (__pool->offset + __align - 1) & ~(__align - 1);
    int ok = (aligned_offset + __size <= __pool->size);

    pthread_mutex_unlock(&__pool->mtx);
    return ok;
}
void 
om_reset(OM_POOL_PTR __pool) {
    if (__pool == NULL) return;

    pthread_mutex_lock(&__pool->mtx);
    __pool->offset = 0;
    pthread_mutex_unlock(&__pool->mtx);
}
int 
om_resize(OM_POOL_PTR __pool, size_t __new_size) {
    if (__pool == NULL || __new_size < __pool->size) return OM_FAIL;

    pthread_mutex_lock(&__pool->mtx);
    free(__pool->pool);

    __pool->pool = malloc(__new_size);
    if (!__pool->pool) {
        pthread_mutex_unlock(&__pool->mtx);
        return OM_FAIL;
    }

    __pool->size = __new_size;
    __pool->offset = 0;
    pthread_mutex_unlock(&__pool->mtx);
    return OM_OK;
}
int 
om_size(OM_POOL_PTR __pool) {
    if (__pool == NULL) return OM_FAIL;
    return __pool->size;
}
int
om_used(OM_POOL_PTR __pool) {
    if (__pool == NULL) return OM_FAIL;
    return __pool->offset;
}
