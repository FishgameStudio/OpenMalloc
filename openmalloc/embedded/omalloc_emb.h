// -*- C -*-
/* SPDX-License-Identifier: Apache-2.0 */

/* The including header, written by C, suitable for the embedded field. */

#ifndef OMALLOC_EMB_H
#define OMALLOC_EMB_H

#include <stddef.h>

#define OM_OK   0
#define OM_FAIL 1

typedef unsigned char u8;

typedef struct
{
    u8*    pool;
    size_t size;
    size_t offset;
} OM_POOL;

typedef OM_POOL* OM_POOL_PTR;

void  OM_PoolInit(OM_POOL_PTR p, u8* buf, size_t buf_size);
void* OM_PoolMalloc(OM_POOL_PTR p, size_t alloc_size, size_t align);
int   OM_PoolMallocable(OM_POOL_PTR p, size_t alloc_size, size_t align);
void  OM_PoolReset(OM_POOL_PTR p);
int OM_Size(OM_POOL_PTR p);

#endif // OMALLOC_EMB_H