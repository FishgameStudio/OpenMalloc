# OpenMalloc C Version Documentation
## omalloc_c.h & omalloc_c.c

### 1. Overview
OpenMalloc **C language implementation** (C99 compatible).
A **thread-safe, linear memory pool allocator** with manual memory management.
Suitable for:
- High-frequency allocation
- Embedded systems (with OS / pthread)
- Middleware & framework memory management
- Avoiding memory fragmentation

Features:
- Thread-safe via `pthread_mutex`
- Manual alignment control
- Linear (arena) allocation model
- No object construction/destruction (raw memory only)
- Expand-only resize (no unsafe shrink)
- Simple error code return

---

### 2. Dependencies
- C Standard Library: `stdlib.h`, `string.h`
- POSIX Threads: `pthread.h` (for mutex)
- Compiler: C99 or later

---

### 3. Types
```c
typedef struct OM_POOL* OM_POOL_PTR;
```
- Opaque pointer to a memory pool instance.

---

### 4. Return Values
- `OM_OK` (0): Success
- `OM_FAIL` (-1): Error

---

### 5. API Reference

#### om_poolInit
```c
int om_poolInit(OM_POOL_PTR __pool, size_t __size);
```
**Initialize a memory pool.**
- Allocates internal buffer with `malloc(__size)`
- Initializes internal mutex
- Returns `OM_OK` / `OM_FAIL`

---

#### om_poolDestroy
```c
void om_poolDestroy(OM_POOL_PTR __pool);
```
**Destroy the pool and release all resources.**
- Locks the pool
- Frees internal buffer
- Resets size/offset to 0
- Destroys mutex
- Safe to call with NULL

---

#### om_malloc
```c
void* om_malloc(OM_POOL_PTR __pool, size_t __size, size_t __align);
```
**Allocate aligned memory from the pool.**
- Thread-safe
- Automatically aligns the address
- Returns pointer or NULL if out of memory
- **Does not construct objects** (raw memory only)

---

#### om_mallocable
```c
int om_mallocable(OM_POOL_PTR __pool, size_t __size, size_t __align);
```
**Check if allocation is possible.**
- Returns 1 if available
- Returns 0 if NULL or insufficient space

---

#### om_reset
```c
void om_reset(OM_POOL_PTR __pool);
```
**Reset the pool offset to 0.**
- Does NOT free memory
- Pool can be reused immediately
- Thread-safe

---

#### om_resize
```c
int om_resize(OM_POOL_PTR __pool, size_t __new_size);
```
**Resize pool (only expand allowed).**
- If `__new_size < current size` → returns `OM_FAIL`
- Frees old buffer, allocates new buffer
- Resets offset to 0
- Thread-safe

---

#### om_size
```c
int om_size(OM_POOL_PTR __pool);
```
**Get total pool capacity.**
- Returns size (positive integer)
- Returns `OM_FAIL` if NULL

---

### 6. Usage Example (C)
```c
OM_POOL pool;
om_poolInit(&pool, 1024);

void* data = om_malloc(&pool, 64, 8);
if (data) {
    // use memory
}

om_reset(&pool);
om_resize(&pool, 2048);
om_poolDestroy(&pool);
```

---

### 7. Notes
- Pure **raw memory allocation** (no C++ object lifecycle)
- Thread-safe via POSIX mutex
- Linear allocator: fast but no free of individual blocks
- Designed for **batch allocation + reset** patterns
