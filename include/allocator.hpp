#pragma once

namespace alloc {
    
    const size_t MINIMAL_BATCH_SIZE = 1024; /// Number of bytes to allocate per batch
    
    struct FreeNode {
        FreeNode* next;
        size_t block_size; /// Usable bytes after the pointer
    };

    struct Metadata {
        size_t occupied_size; /// Usable bytes after the metadata
    };

    extern FreeNode* front;

    void initialize(size_t bytes);
    bool node_address_less(FreeNode* a, FreeNode *b);
    void insert_node(FreeNode* node);
    void request_memory(size_t bytes);
    void free(void* ptr); 
    void* allocate(size_t bytes);
    size_t allocated_size(void *ptr);
     
}