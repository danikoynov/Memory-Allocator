#include "allocator.hpp"
#include <stdexcept>
#include <iostream>
#include <windows.h>

namespace alloc {

    FreeNode* front = nullptr;

    void initialize(size_t bytes) {
        
        void *ptr = VirtualAlloc(
            nullptr,
            bytes + sizeof(FreeNode),
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );   

        if (ptr == nullptr)
            throw std::runtime_error("Virtual memory allocation during initialization failed");

        front = static_cast<FreeNode*>(ptr);
        front -> block_size = bytes;
        front -> next = nullptr;
    }

    bool node_address_less(FreeNode* a, FreeNode *b) {
        return reinterpret_cast<std::uintptr_t>(a) <
                reinterpret_cast<std::uintptr_t>(b);
    }
    
    void try_coalesce(FreeNode* node) {
        if (node -> next == nullptr)
            return;
            
        size_t total_payload = node -> block_size + sizeof(FreeNode);
        
        FreeNode* ptr = reinterpret_cast<FreeNode*>(
            reinterpret_cast<char*>(node) + total_payload
        );

        if (ptr == node -> next) {
            node -> block_size += node -> next -> block_size + sizeof(FreeNode);
            node -> next = node -> next -> next;
        }
    }

    void insert_node(FreeNode* node) {
        FreeNode* current = front;
        FreeNode* prev = nullptr;

        while(current != nullptr) {
            if (node_address_less(node, current))
                break;
            prev = current;
            current = current -> next;
        }

        if (prev != nullptr)
            prev -> next = node;
        else
            front = node;
        
        node -> next = current;

        try_coalesce(node);
        if (prev != nullptr)
            try_coalesce(prev);
    }

    void request_memory(size_t bytes) {
        
        void *ptr = VirtualAlloc(
            nullptr,
            bytes + sizeof(FreeNode),
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        ); 

        if (ptr == nullptr)
            throw std::runtime_error("Virtual memory allocation during request failed");

        FreeNode* new_node = static_cast<FreeNode*>(ptr);
        new_node -> block_size = bytes;
        new_node -> next = nullptr;

        insert_node(new_node);
    }

    void* allocate(size_t bytes) {

        FreeNode* current = front;
        FreeNode* prev = nullptr;

        while(current != nullptr) {

            if (current -> block_size + sizeof(FreeNode) >= bytes + sizeof(Metadata)) {
                size_t initial_block_size = current -> block_size;
                size_t remaining_blocks = current -> block_size + sizeof(FreeNode) - bytes - sizeof(Metadata);

                FreeNode* next_node = current -> next;
                Metadata* data = reinterpret_cast<Metadata*>(current);
                
                if (remaining_blocks <= sizeof(FreeNode)) {
                    data -> occupied_size = bytes + remaining_blocks;
                    
                    if (prev != nullptr)
                        prev -> next = next_node;
                    else
                        front = next_node; /// next_node becomes the front
                }
                else
                {
                    data -> occupied_size = bytes;

                    FreeNode* new_node = reinterpret_cast<FreeNode*>(
                        reinterpret_cast<char*>(data) + bytes + sizeof(Metadata)
                    );
                    
                    new_node -> block_size = initial_block_size - bytes - sizeof(Metadata);
                    
                    if (prev != nullptr)
                        prev -> next = new_node;
                    else
                        front = new_node; /// new_node becomes front

                    new_node -> next = next_node;
                }

                return reinterpret_cast<char*>(data) + sizeof(Metadata);

            }   
            else {
                prev = current;
                current = current -> next;
            }
        }
        
        request_memory(max(MINIMAL_BATCH_SIZE, bytes + max(sizeof(Metadata), sizeof(FreeNode))));
        return allocate(bytes);
    }

    size_t allocated_size(void *ptr) {
        Metadata* meta_ptr = reinterpret_cast<Metadata*>(
            reinterpret_cast<char*>(ptr) - sizeof(Metadata)
        );
        return meta_ptr -> occupied_size;
    }

    void free(void* ptr) {
        /// Each occupied segment should be at least sizeof(FreeNode) large

        Metadata* data = reinterpret_cast<Metadata*>(
            reinterpret_cast<char*>(ptr) - sizeof(Metadata)
        );
        FreeNode* new_node = reinterpret_cast<FreeNode*>(data);

        new_node -> block_size = data -> occupied_size + sizeof(Metadata) - sizeof(FreeNode); 
        new_node -> next = nullptr;

        insert_node(new_node);
    }
}