# OpenMalloc - Embedded Bare-Metal Documentation
## Overview
OpenMalloc **Embedded Edition** is a **zero-dependency, bare-metal compatible memory pool allocator** written in standard **C99**.  
It requires **no OS, no malloc, no libc, no threads**, and is designed for **MCU, ARM, RTOS, and resource-constrained embedded systems**.

This implementation uses **static user-provided memory buffers** and supports **automatic alignment**, fast linear allocation, and pool reset.

## Features
- **Bare-metal friendly** (no dynamic allocation, no OS required)
- **Zero dependencies** (no stdlib, no pthread, no heap)
- **Static memory buffer** (user-defined global or static array)
- **Automatic memory alignment**
- **Fast O(1) linear allocation**
- **No threads, no locks** (single‑threaded embedded environment)
- **Tiny code size** (ideal for flash/ROM constrained devices)
- **Reusable memory pool** (reset without reallocation)

## Types
```c
typedef unsigned char u8;

typedef struct {
    u8*     pool;
    size_t  size;
    size_t  offset;
} OM_POOL;

typedef OM_POOL* OM_POOL_PTR;
```

## API Reference

### OM_PoolInit
```c
void OM_PoolInit(OM_POOL_PTR p, u8* buf, size_t buf_size);
```
Initialize a memory pool with a **static pre-allocated buffer**.  
- `buf`: pointer to static/global memory array  
- `buf_size`: total size of the buffer  

### OM_PoolMalloc
```c
void* OM_PoolMalloc(OM_POOL_PTR p, size_t alloc_size, size_t align);
```
Allocate aligned memory from the pool.  
- Returns valid pointer on success  
- Returns `NULL` if out of memory  

### OM_PoolMallocable
```c
int OM_PoolMallocable(OM_POOL_PTR p, size_t alloc_size, size_t align);
```
Check if an allocation can be performed.  
- Returns `1` if possible  
- Returns `0` otherwise  

### OM_PoolReset
```c
void OM_PoolReset(OM_POOL_PTR p);
```
Reset pool offset to 0.  
Memory buffer remains available for reuse.

### OM_Size
```c
int OM_Size(OM_POOL_PTR p);
```
Return total pool size.

## Usage Example (Bare-Metal Embedded)
```c
#include "omalloc_emb.h"

// Static memory pool (1KB)
static u8 pool_buffer[1024];
OM_POOL my_pool;

int main(void) {
    // Initialize pool
    OM_PoolInit(&my_pool, pool_buffer, sizeof(pool_buffer));

    // Allocate 32 bytes with 4-byte alignment
    void* data = OM_PoolMalloc(&my_pool, 32, 4);

    // Reset pool
    OM_PoolReset(&my_pool);

    while (1) {
        // main loop
    }
}
```

---

# Compilation Commands (3 Embedded Compilers)
## 1. ARM GCC (ARM Cortex‑M / STM32 / NXP)
```bash
arm-none-eabi-gcc -c omalloc_emb.c -o omalloc_emb.o -mcpu=cortex-m4 -mthumb -O2 -std=c99
```

## 2. Keil ARMCC (ARM Compiler 5 / 6)
```bash
armclang -c omalloc_emb.c -o omalloc_emb.o --target=arm-arm-none-eabi -mcpu=cortex-m3 -O2 -std=c99
```

## 3. IAR EWARM Compiler
```bash
iccarm omalloc_emb.c -o omalloc_emb.o --cpu Cortex-M4 --thumb -O2 --std=c99
```

---

## Target Platforms
- ARM Cortex‑M0/M0+/M3/M4/M7  
- STM32, ESP32, GD32, AVR, PIC  
- Bare-metal embedded  
- RTOS environments (FreeRTOS, RT-Thread, UCOS)  
- Low-power IoT & industrial devices

---

## Notes
- No dynamic memory allocation (`malloc`/`free` free)  
- No multithreading support (safe for single‑threaded embedded)  
- Designed for **linear allocation + batch reset** pattern  
