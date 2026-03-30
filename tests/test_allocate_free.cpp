#include <iostream>
#include "allocator.hpp"
#include <cassert>

void test_allocate_and_free() {
    alloc::initialize(1000);

    int* ptr1 = static_cast<int*>(alloc::allocate(sizeof(int)));
    int* ptr2 = static_cast<int*>(alloc::allocate(sizeof(int)));

    /// Pointers are not nullptr
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);

    /// Pointers are different
    assert(ptr1 != ptr2);

    *ptr1 = 42;
    *ptr2 = 99;

    /// Values stay the same
    assert(*ptr1 == 42);
    assert(*ptr2 == 99);

    /// Free first block
    alloc::free(ptr1);

    /// Allocate again
    int* ptr3 = static_cast<int*>(alloc::allocate(sizeof(int)));

    /// New allocation succeeds
    assert(ptr3 != nullptr);

    *ptr3 = 123;

    /// Remaining old allocation is still valid
    assert(*ptr2 == 99);

    /// New allocation is writable
    assert(*ptr3 == 123);

    /// Free remaining blocks
    alloc::free(ptr2);
    alloc::free(ptr3);
}

int main()
{
    test_allocate_and_free();
    return 0;
}