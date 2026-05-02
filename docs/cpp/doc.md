# OpenMalloc C++ Edition Documentation
This is clean, professional, English documentation **directly matching your C++ code** — ready to put into `docs/` or GitHub.

---

# OpenMalloc C++ API Documentation
## Overview
OpenMalloc C++ is a **modern, thread-safe, object-aware memory pool allocator** built for high-performance applications.
It strictly follows C++ object lifetime rules, supports automatic alignment, in-place construction, and automatic object destruction.

- **Standard**: C++17 or later
- **Safety**: No raw `new[]` / `delete[]`
- **Thread-safe**: Uses `std::mutex` pessimistic locking
- **Object lifetime safe**: Uses placement new + explicit destructor calls
- **Non-copyable**: Memory pool cannot be copied
- **Exception-based error handling**

## Dependencies
- C++17 Standard Library
- `<memory>`, `<mutex>`, `<stdexcept>`, `<unordered_map>`, `<algorithm>`

## Core Classes
### Exceptions
All exceptions inherit from `std::runtime_error`.

- **`AllocFail`**
  Thrown when memory allocation fails.

- **`ResizeFail`**
  Thrown when resizing to a smaller size or invalid capacity.

- **`InitFail`**
  Thrown when initializing a memory pool with size 0.

### MemoryPool
Main memory pool class. **Non-copyable, thread-safe**.

#### Member Types
```cpp
using POOL_PTR = std::unique_ptr<uint8_t[]>;
```

## Public API

### Constructor
```cpp
explicit MemoryPool(size_t s);
```
Creates a memory pool with fixed capacity.
- Throws `InitFail` if size is zero.

### malloc()
```cpp
template<typename T, class... Args>
[[nodiscard]] T* malloc(Args&&... args);
```
Allocates and **constructs an object of type `T`** in the pool.
- Automatically aligns memory according to `alignof(T)`
- Uses **placement new**
- Perfect forwarding for constructor arguments
- Stores destructor for automatic cleanup
- Throws `AllocFail` if out of memory
- **[[nodiscard]]** — return value must be used

### mallocable()
```cpp
template<typename T>
[[nodiscard]] bool mallocable();
```
Checks whether an object of type `T` can be allocated.
- Thread-safe
- Returns `true` if space is available

### reset()
```cpp
void reset();
```
Destroys all objects in the pool and resets offset to 0.
- Memory buffer remains usable
- Thread-safe

### resizeAs()
```cpp
void resizeAs(size_t s);
```
Resizes the pool **only to a larger capacity**.
- Destroys all existing objects
- Reallocates internal buffer
- Throws `ResizeFail` if shrinking
- Thread-safe

### erase_all()
```cpp
void erase_all();
```
Destroys all objects, frees internal memory, and invalidates the pool.
Automatically called by destructor.

### size()
```cpp
size_t size();
```
Returns total capacity of the memory pool.

### Destructor
```cpp
~MemoryPool();
```
Automatically calls `erase_all()`.

---

## Usage Example
```cpp
#include "OpenMalloc.hpp"

struct MyObject {
    int value;
    MyObject(int v) : value(v) {}
};

int main() {
    OMalloc::MemoryPool pool(1024);

    auto obj = pool.malloc<MyObject>(42);

    if (pool.mallocable<MyObject>()) {
        auto obj2 = pool.malloc<MyObject>(123);
    }

    pool.reset();
    pool.resizeAs(2048);
}
```

---

## Key Features
- ✅ **C++17 Modern Design**
- ✅ **Thread-safe**
- ✅ **Automatic object destruction**
- ✅ **Strict alignment**
- ✅ **Non-copyable semantics**
- ✅ **Exception safety**
- ✅ **No memory leaks**
- ✅ **No undefined behavior**
- ✅ **\[\[nodiscard\]\]** for safe API usage

---

## Compilation
Compile with C++17 or higher:
```bash
clang++ test/your_code.cpp -Wall -std=c++26 -stdlib=libc++ -fexperimental-library -I./openmalloc -o build/your_program
g++ -o build/your_program -Wall -std=c++26 -I./openmalloc/ test/your_code.cpp
cl /O2 /W4 /std:c++26 /I.\openmalloc/ /Fobuild\ /Febuild\your_program.exe test\your_code.cpp
```
