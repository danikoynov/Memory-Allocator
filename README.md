# Toy Allocator Library

A small custom memory allocator written in C++ for learning purposes.

This project implements a simple free-list allocator on Windows using `VirtualAlloc`. It is meant to help understand how dynamic memory allocation works internally: how memory is requested from the operating system, how free blocks are managed, how allocation metadata is stored, and how freed blocks can be merged back together.

This allocator is a toy project. It is not intended for production use.

---

## Purpose

The main goals of this project are:

- understand how heap allocation works internally
- practice low-level C++
- work with raw pointers and pointer arithmetic
- learn how free-list allocators are structured
- become familiar with Windows virtual memory APIs such as `VirtualAlloc`
- build a project that is useful for systems and quant-dev interview discussions

---

## Project Structure

The project currently has the following structure:

```text
MEMORY ALLOCATOR/
├── build/
├── include/
│   └── allocator.hpp
├── src/
│   └── allocator.cpp
├── tests/
│   ├── Testing/
│   ├── CMakeLists.txt
│   ├── test_allocate_free.cpp
│   ├── test_allocate.cpp
│   └── test_multiple_allocates.cpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```

## Folder Overview

### `include/`

Contains the public header:

- `allocator.hpp`

This file declares the allocator interface and the internal data structures needed by the implementation, such as `FreeNode` and `Metadata`.

---

### `src/`

Contains the allocator implementation:

- `allocator.cpp`

This file contains the core allocator logic:

- initialization
- allocation
- freeing
- requesting additional memory from the operating system
- maintaining the free list
- splitting free blocks
- coalescing adjacent free blocks

---

### `tests/`

Contains test files for the allocator:

- `test_allocate.cpp`
- `test_multiple_allocates.cpp`
- `test_allocate_free.cpp`

These are simple correctness tests that verify the allocator behaves as expected.

Typical tests check:

- allocation returns non-null pointers
- multiple allocations return different addresses
- values written to allocated memory remain correct
- freeing memory does not corrupt other allocations
- freed blocks can be reused

---

### `build/`

Contains generated build files produced by CMake.

This directory is automatically created during compilation and should not be committed to version control.

---

### Root Files

- `CMakeLists.txt` — main CMake build configuration
- `.gitignore` — ignores generated files such as `build/`
- `README.md` — project documentation

---

# High-Level Design

The allocator uses a **free list**.

A free list is a linked list of blocks that are currently available for allocation. Each free block begins with a `FreeNode` header that stores:

- the payload size of the free block
- a pointer to the next free block

Allocated blocks begin with a `Metadata` header that stores the size of the user allocation.

The allocator keeps a global pointer:

```cpp
FreeNode* front = nullptr;
