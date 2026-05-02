// -*- C++ -*
/* SPDX-License-Identifier: Apache-2.0 */


/* The main including header of the OpenMalloc project. */

#include <memory>     // std::unique_ptr std::make_unique
#include <stdexcept>  // std::exception std::runtime_error
#include <string>     // std::string
#include <utility>    // std::forward

namespace OMalloc {

    class AllocFail : public std::runtime_error {
    public:
        explicit AllocFail(const std::string& msg)
            : std::runtime_error(msg) {}
    };

    using POOL_PTR = std::unique_ptr<uint8_t[]>;

    class MemoryPool {
    private:
        POOL_PTR pool=nullptr; // Point to the start of the memory pool.
        size_t size;
        bool is_allocated = false;
        /*
         * The offset.
         * This pointer points the current allocated object.
         */
        size_t curr_offset = 0;
    public:
        explicit MemoryPool(size_t s) 
            : size(s)
        {
            /* Initialize the pool. */
            pool = std::make_unique<uint8_t[]>(size);
        }

        // Disable constructor function copying.
        MemoryPool(const MemoryPool&) = delete;
        MemoryPool& operator=(const MemoryPool&) = delete;

        /* Args is for the arguments of other constructors. */
        template<typename T, class... Args>
        [[nodiscard]] T* malloc(Args&&... args) {
            /* Align the memory. */
            size_t typeSize = sizeof(T);
            size_t alignMark = alignof(T) - 1;
            size_t alignedOff = (curr_offset + alignMark) & ~alignMark;

            /* Overflow */
            if (alignedOff + typeSize > size) {
                throw AllocFail(
                    "Allocate failed: offset " + std::to_string(alignedOff)
                    + " with type size " + std::to_string(typeSize)
                    + " reached limit pool size " + std::to_string(size)
                );
                return nullptr;
            }

            /* Get the address and hard-cast to T pointer. */
            uint8_t* raw = pool.get() + alignedOff;
            T* p = reinterpret_cast<T*>(raw);
            // Construct the object in situ.
            new (p) T(std::forward<Args>(args)...);

            curr_offset = alignedOff + typeSize;
            is_allocated = true;
            return p;
        }

        template<typename T>
        [[nodiscard]]
        bool mallocable() const {
            /* Return wether the type can be allocated. */
            size_t typeSize = sizeof(T);
            size_t alignMark = alignof(T) - 1;
            size_t alignedOff = (curr_offset + alignMark) & ~alignMark;
            return (alignedOff + typeSize) <= size;
        }

        void reset() {
            /* Reset offset pointer. */
            curr_offset = 0;
        }

        void erase_all() {
            /* Erase all memory. */
            size = 0;
            pool.reset();
            pool = nullptr;
            is_allocated = false;
            curr_offset = 0;
        }

        ~MemoryPool() {
            erase_all();
        }
        
    };
}