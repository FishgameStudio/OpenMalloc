#include "omalloc_emb.h"

static size_t 
OM_AlignAddr(size_t addr, size_t align)
{
    return (addr + align - 1) & ~(align - 1);
}

void 
OM_PoolInit(OM_POOL_PTR p, u8* buf, size_t buf_size)
{
    if (!p || !buf || buf_size == 0) return;

    p->pool   = buf;
    p->size   = buf_size;
    p->offset = 0;
}

void* 
OM_PoolMalloc(OM_POOL_PTR p, size_t alloc_size, size_t align)
{
    if (!p) return NULL;

    size_t aligned = OM_AlignAddr(p->offset, align);

    if (aligned + alloc_size > p->size) return NULL;

    u8* ptr = p->pool + aligned;
    p->offset = aligned + alloc_size;

    return (void*)ptr;
}

int 
OM_PoolMallocable(OM_POOL_PTR p, size_t alloc_size, size_t align)
{
    if (!p) return 0;

    size_t aligned = OM_AlignAddr(p->offset, align);
    return (aligned + alloc_size <= p->size) ? 1 : 0;
}

void 
OM_PoolReset(OM_POOL_PTR p)
{
    if (!p) return;
    p->offset = 0;
}

int 
OM_Size(OM_POOL_PTR p)
{
    if (!p) return;
    return p->size;
}