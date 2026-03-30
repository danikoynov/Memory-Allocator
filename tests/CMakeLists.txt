add_executable(test_allocator
    test_allocator.cpp
)

target_link_libraries(test_allocator
    PRIVATE allocator
)

add_test(NAME test_allocator COMMAND test_allocator)