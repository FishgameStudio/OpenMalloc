<a id="readme-top"></a>

![logo](assets/logo.png)

<div align="center">

# OpenMalloc
> A simple memory pool allocator & manager!

[**Explore the docs >>>**](docs)

![Stars](https://img.shields.io/github/stars/FishgameStudio/OpenMalloc?style=social)
![Forks](https://img.shields.io/github/forks/FishgameStudio/OpenMalloc?style=social)
![Watchers](https://img.shields.io/github/watchers/FishgameStudio/OpenMalloc?style=social)
![Issues](https://img.shields.io/github/issues/FishgameStudio/OpenMalloc)
![PRs](https://img.shields.io/github/issues-pr/FishgameStudio/OpenMalloc)

![build-pass](https://img.shields.io/badge/build-pass-yellow)
[![C++](https://img.shields.io/badge/C++-26-blue)](https://isocpp.org)
![License](https://img.shields.io/github/license/FishgameStudio/OpenMalloc)

</div>

<details>
    <summary>Table of Contents</summary>
    <li>
        <a href="#Introduction">About the project</a>
    </li>
    <li>
        <a href="#Introduction">About the project</a>
    </li>


</details>

## Introduction
🚀A lightweight, high-performance, thread-safe **custom memory pool allocator** implemented purely in modern C++, designed for game engines, runtime frameworks, and high-frequency memory allocation scenarios.

There are countless general-purpose memory allocators and trivial memory pool implementations scattered across GitHub and open-source communities; however, most of them are either overly bloated with redundant features, lack strict thread safety, ignore C++ object lifecycle rules, or sacrifice readability for obscure low-level optimizations. None of them perfectly matched my demands for **simplicity, safety, standard compliance, and industrial availability**, so I built **OpenMalloc** from scratch as a clean, reliable, and production-ready memory pool solution.

Here’s why OpenMalloc stands out and why you might want to use it in your projects:

- Your development energy should focus on implementing core business logic and architectural design, rather than repeatedly writing fragile memory management code from scratch for every project.
- You deserve a memory pool that strictly follows **C++ object lifecycle semantics** — no unsafe `memcpy` object relocation, no undefined behavior, no manual memory leak troubleshooting.
- You shouldn’t compromise between single-thread performance and multi-thread stability; OpenMalloc adopts standard pessimistic locking to deliver out-of-the-box thread safety without unnecessary performance overhead for common scenarios.
- You can reuse this allocator across game engines, server backends, embedded C++ projects, and runtime component systems, following the DRY principle and avoiding repetitive wheel reinvention.
- You get a minimalist but complete feature set: **automatic memory alignment, typed object construction/destruction, safe pool reset, controlled capacity expansion, forbidden unsafe shrinkage, exception-driven error handling, and complete isolation of object lifecycle management**.

Of course, no single memory allocator can cover every extreme scenario such as ultra-low-latency real-time systems or kernel-level memory management. So I will keep iterating OpenMalloc continuously: adding more allocation strategies, supporting block-based pool partitioning, optimizing lock contention, and adapting to more C++ standard versions in the future. You are also welcome to participate in project improvement by forking the repository, submitting pull requests for feature enhancements or bug fixes, or opening issues to suggest new requirements and optimization ideas. Thanks to every developer who stars, forks, and contributes to OpenMalloc — your support keeps this project evolving and mature.

## Core Features
- ✨ **Modern C++ Native Implementation**  
  Pure C++ standard code, no platform-specific hacks, no C-style forced memory tricks, compatible with C++17 and later standards.
- 🛡️ **Strict C++ Object Lifecycle Safety**  
  Uses `placement new` for in-place object construction and explicit destructor invocation; never uses `memcpy` to copy non-POD objects, completely avoiding double free, wild pointers and undefined behavior.
- 🔒 **Built-in Thread Safety**  
  Equipped with `std::mutex` pessimistic lock protection for core allocation, reset and expansion interfaces, safe for multi-thread concurrent allocation.
- 📏 **Automatic Memory Alignment**  
  Automatically calculates the alignment boundary of any type, complies with CPU memory access rules, avoids unaligned access performance loss and hardware exceptions.
- ♻️ **Lifecycle Independent Management**  
  Maintains a mapping of allocated object addresses and corresponding destructors; automatically destructs all objects on pool reset or destruction to eliminate memory leaks fundamentally.
- 📐 **Controlled Safe Resizing**  
  Provides `resizeAs` interface: only allows capacity expansion, prohibits unsafe shrinkage; clears existing objects and re-initializes the pool to ensure memory security.
- 🧹 **Dual Reset & Erase Mechanism**  
  - `reset()`: Destruct all objects, reset offset, retain memory pool for reuse.  
  - `erase_all()`: Destruct objects, release entire memory pool, completely recycle resources.
- ❌ **Disable Copy Semantics**  
  Forbids copy construction and copy assignment of memory pool instances to avoid accidental memory duplication and resource chaos.
- 🚨 **Exception-based Error Handling**  
  Custom exception classes for allocation failure and illegal resizing, providing clear error information for easy debugging.

## Suitable Scenarios
- Game engine runtime object pool management
- High-frequency temporary object allocation for server backend services
- Embedded C++ projects that require controlled memory footprint
- Framework underlying memory management layer implementation
- Any scenario that needs to reduce frequent `new/delete` overhead and avoid memory fragmentation

## Getting Started
Here is an example to use & build.

At first, please clone the repository on your local computer:
```bash
git clone https://github.com/FishgameStudio/OpenMalloc.git
```

### How to Use

[**Explore the docs >>>**](docs)

This is an example to include the header of this project. Please keep the copyright comments in the header file while using.

```cpp
#include <openmalloc/omalloc.cpp>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    OMalloc::MemoryPool pool(1024); // 1 KB in heap.

    if (!pool.mallocable<string>()) return 1;

    string* ptr = pool.malloc<string>("OpenMemory Is Good!");
    cout << *ptr << endl; // OpenMemory Is Good!

    unsigned long long* ptr2 = pool.malloc<unsigned long long>(1234567890);
    cout << *ptr2;

    pool.reset(); // Reset offset but not clear.
    pool.erase_all(); // erase all data.
    pool.resizeAs(4096);

    /* Other code... */
    return 0;
}
```

*if you modified our source code, please insert these lines into your code:*
```cpp
// Modified by [Your Name] [Modified Date]
// Changes: [Modified Content]
```

### How To Build
This is an example to compile & run the code.

#### Enviroment Required
To compile the code, you need:
- clang/clang++ latest version(22).
- gcc/g++ latest version(16.1).
- MSVC latest version(14.50.35710).

*Your compiler must support libc++.*

#### Compiling Code
This is an example of the compiling code.
```bash
# clang++
clang++ test/your_code.cpp -Wall -std=c++26 -stdlib=libc++ -fexperimental-library -I./openmalloc -o build/your_program
# g++
g++ -o build/your_program -Wall -std=c++26 -I./openmalloc/ test/your_code.cpp
# MSVC (no escape character)
cl /O2 /W4 /std:c++26 /I.\openmalloc/ /Fobuild\ /Febuild\your_program.exe test\your_code.cpp

# Run
build/your_program
```

## Roadmap
- [ ] Implement allocating logic in C
- [ ] Features
- [ ] Features
- [ ] Features

See the [open issues](https://github.com/FishgameStudio/OpenMalloc/issues) for a full list of proposed features (and known issues).

## Contributing

Contributions are what make the open source community such an amazing place **to learn, inspire, and create**. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please **fork the repo** and **create a pull request**. You can also simply open an issue with the tag **"enhancement"**.
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Top contributors:

<a href="https://github.com/FishgameStudio/OpenMalloc/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=FishgameStudio/OpenMalloc" alt="contrib.rocks image" />
</a>

## License

Distributed under the Apache 2.0 License. See [LICENSE](LICENSE) for more information.


## Contact

Nicola Grey - [@popxhxh](mailto:popxhxh@outlookh.com) - popxhxh@outlookh.com

Project Link: [https://github.com/FishgameStudio/OpenMalloc](https://github.com/FishgameStudio/OpenMalloc)


## Acknowledgments

* [Best-README-Template](https://github.com/othneildrew/Best-README-Template)


## Conclusion
Hope OpenMalloc helps you learn C++ memory management and low-level allocation design. Your stars, forks and feedback are highly appreciated. Welcome to contribute and grow the project together.
If you like this project, welcome to **Star** and **Fork**. Any suggestions, bug reports and feature requests are warmly welcomed via Issues and Pull Requests. Thanks again!
