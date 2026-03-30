#include <iostream>
#include "allocator.hpp"
#include <cassert>

void test_multiple_allocates() {
    alloc::initialize(1000);
    
    int* ptr1 = static_cast<int*>(alloc::allocate(sizeof(int)));
    int* ptr2 = static_cast<int*>(alloc::allocate(sizeof(int)));
    int* ptr3 = static_cast<int*>(alloc::allocate(sizeof(int)));

    /// Pointers are not nullptr
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);
    assert(ptr3 != nullptr);

    /// Pointers are different
    assert(ptr1 != ptr2);
    assert(ptr1 != ptr3);
    assert(ptr2 != ptr3);

    *ptr1 = 10;
    *ptr2 = 5;
    *ptr3 = 8;

    /// Values stay the same
    assert(*ptr1 == 10);
    assert(*ptr2 == 5);
    assert(*ptr3 == 8);
}

int main()
{
    test_multiple_allocates();
    return 0;
}