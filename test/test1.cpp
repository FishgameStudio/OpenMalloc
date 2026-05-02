/* Test code. */

#include <openmalloc/omalloc.h>
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
    return 0;
}