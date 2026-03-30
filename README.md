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

## High-Level Design

This allocator is built around a **free-list design**.

At a high level, the allocator manages memory as a collection of blocks. Some blocks are currently free and available for future allocations, while others are occupied and have been returned to the user.

The allocator keeps track of free memory using a linked list called the **free list**. Each node in this list represents one free block. The list is kept sorted by memory address, which makes it easier to merge neighboring free blocks when memory is returned.

When the user requests memory, the allocator searches the free list for a block that is large enough. If it finds one, it uses that block to satisfy the request. If the block is larger than necessary, it is split into two parts: one part becomes the allocated block, and the other remains in the free list as a smaller free block. If the block is too small to split safely, the allocator uses the whole block.

Each allocated block stores a small piece of metadata before the memory returned to the user. This metadata records the size of the allocation, so that when the user frees the block later, the allocator knows how large that block is.

When memory is freed, the allocator converts the block back into a free-list node and reinserts it into the free list in the correct address order. After reinsertion, it attempts to **coalesce** the block with neighboring free blocks if they are directly adjacent in memory. This helps reduce fragmentation and recover larger continuous regions of free memory.

If no existing free block is large enough to satisfy an allocation request, the allocator requests an additional region of memory from the operating system using `VirtualAlloc`. That new region is then inserted into the free list and the allocation is retried.

Overall, the design demonstrates the core ideas behind custom memory allocators:

- requesting raw memory from the OS
- managing free memory explicitly
- storing allocation metadata
- splitting blocks during allocation
- reinserting and merging blocks during deallocation
- handling fragmentation through coalescing