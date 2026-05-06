// -*- C++ -*-
/* SPDX-License-Identifier: Apache-2.0 */


/* The main including header of the OpenMalloc project. */

#pragma once
#include <memory>        // std::unique_ptr std::make_unique
#include <stdexcept>     // std::exception std::runtime_error
#include <string>        // std::string
#include <utility>       // std::forward
#include <unordered_map> // std::unordered_map
#include <mutex>         // std::mutex
#include <algorithm>     // std::find_if

namespace OMalloc {

    class AllocFail : public std::runtime_error {
    public:
        explicit AllocFail(const std::string& msg)
            : std::runtime_error(msg) {}
    };
    class ResizeFail : public std::runtime_error {
    public:
        explicit ResizeFail(const std::string& msg)
            : std::runtime_error(msg) {}
    };
    class InitFail : public std::runtime_error {
    public:
        explicit InitFail(const std::string& msg)
            : std::runtime_error(msg) {}
    };


    using POOL_PTR = std::unique_ptr<uint8_t[]>;
    // The unique_ptr only manages the memory instead of the objects on the memory.
    // It only can give the memory back, doesn't destruct the objects.

    class MemoryPool {
    private:
        POOL_PTR pool=nullptr; // Point to the start of the memory pool.
        size_t size;
        size_t used=0;
        std::mutex mtx; // pessimistic locking.
        /*
         * The offset.
         * This pointer points the current allocated object.
         */
        size_t curr_offset = 0;

        /* Each object pointers on the memory pool & the destructors of objects. */
        std::unordered_map<void*, void(*)(void*)> objs;
        
        void destruct_all_obj_no_lock() {
            /** 
             * Destruct every objects on the pool. 
             * No `delete p;` because the memory is the memory pool's!
             */
            for (auto& pair : objs) {
                auto obj = pair.first;
                auto destructor = pair.second;
                destructor(obj);
            }
            objs.clear();
        }
        void destruct_all_obj() {
            std::lock_guard<std::mutex> lock(mtx);
            destruct_all_obj_no_lock();
        }
    public:
        explicit MemoryPool(size_t s) 
            : size(s)
        {
            /* Initialize the pool. */
            if (s == 0) {
                throw InitFail("Pool size cannot be 0");
            }
            pool = std::make_unique<uint8_t[]>(size);
        }

        // Disable constructor function copying.
        MemoryPool(const MemoryPool&) = delete;
        MemoryPool& operator=(const MemoryPool&) = delete;

        /* Args is for the arguments of other constructors. */
        template<typename T, class... Args>
        [[nodiscard]] T* malloc(Args&&... args) {
            std::lock_guard<std::mutex> lock(mtx);

            if (!pool) {
                throw AllocFail("Try to allocate on a destoryed pool");
            }
            /* Align the memory. */
            size_t typeSize = sizeof(T);
            const size_t align = alignof(T);
            size_t alignedOff = ((curr_offset + align - 1) / align) * align;

            /* Overflow */
            if (alignedOff > size - typeSize) {
                throw AllocFail(
                    "Allocate failed: offset " + std::to_string(alignedOff)
                    + " with type size " + std::to_string(typeSize)
                    + " reached limit pool size " + std::to_string(size)
                );
            }

            /* Get the address and hard-cast to T pointer. */
            uint8_t* raw = pool.get() + alignedOff;
            T* p = reinterpret_cast<T*>(raw);

            // Construct the object without newing memory.
            // The object will be created on address `p`.
            new (p) T(std::forward<Args>(args)...);

            curr_offset = alignedOff + typeSize;
            used = curr_offset;
            // Store object.
            objs.try_emplace(
                p, 
                [](void* ptr) {
                    static_cast<T*>(ptr)->~T();
                }
            );
            return p;
        }

        template<typename T>
        [[nodiscard]]
        bool mallocable() {
            std::lock_guard<std::mutex> lock(mtx);
            /* Return wether the type can be allocated. */
            size_t typeSize = sizeof(T);
            const size_t align = alignof(T);
            size_t alignedOff = ((curr_offset + align - 1) / align) * align;
            return !(alignedOff > size - typeSize);
        }

        void reset() {
            std::lock_guard<std::mutex> lock(mtx);
            /*
             * Reset offset pointer.
             * All the objects on the pool will be lost!
             */ 
            destruct_all_obj();
            curr_offset = 0;
            used = 0;
        }
        
        void resizeAs(size_t s) {
            // DANGER FUNCTION: All the memory will be cleared after resizing!
            std::lock_guard<std::mutex> lock(mtx);
            /* Resize the pool. */

            if (s < size) {
                // Cannot reduce the pool.
                throw ResizeFail("Cannot reduce the pool as size " + std::to_string(s));
                return;
            }
            if (s == size) return;
            /*
             * Destruct all object, empty the pool,
             * then new larger memory pool.
             * The pool will move to another memory address,
             * so the private map `objs` is invalid after this change.
             */
            destruct_all_obj();
            pool.reset();

            size = s;
            pool = std::make_unique<uint8_t[]>(size);

            curr_offset = 0;
            used = 0;
        }

        size_t poolSize() const {
            return size;
        }
        size_t used() const {
            return used;
        }
        
        void erase_all() {
            std::lock_guard<std::mutex> lock(mtx);
            /* Erase all memory & destroy the pool. */
            destruct_all_obj();
            size = 0;
            pool.reset();
            pool = nullptr;
            curr_offset = 0;
        }

        ~MemoryPool() {
            destruct_all_obj_no_lock();
            pool.reset();
            pool = nullptr;
            size = 0;
            curr_offset = 0;
            used = 0;
        }
        
    };
}
