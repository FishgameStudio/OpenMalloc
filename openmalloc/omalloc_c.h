// -*- C -*-
/* SPDX-License-Identifier: Apache-2.0 */

/* The including header of the OpenMalloc project, written by C. */

#ifndef OMALLOC_C_H
#define OMALLOC_C_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OM_OK    0
#define OM_FAIL  1

typedef struct {
    uint8_t* pool;
    size_t   size;
    size_t   offset;
    pthread_mutex_t mtx;
} OM_POOL;
typedef OM_POOL* OM_POOL_PTR;

int   om_poolInit(OM_POOL_PTR __pool, size_t __size);
void  om_poolDestroy(OM_POOL_PTR __pool);
void* om_malloc(OM_POOL_PTR __pool, size_t __size, size_t __align);
int   om_mallocable(OM_POOL_PTR __pool, size_t __size, size_t __align);
void  om_reset(OM_POOL_PTR __pool);
int   om_resize(OM_POOL_PTR __pool, size_t __new_size);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // OMALLOC_C_H