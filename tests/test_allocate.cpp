#include <iostream>
#include "allocator.hpp"
#include <cassert>

void test_allocate() {
    alloc::initialize(1000);
    int* ptr = static_cast<int*>(alloc::allocate(sizeof(int)));
    assert(ptr != nullptr);
    assert(alloc::allocated_size(ptr) == sizeof(int));
    *ptr = 5;
    assert(*ptr == 5);
}

int main()
{
    test_allocate();
    return 0;
}