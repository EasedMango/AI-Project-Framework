#include "MemoryManager.h"

#include "MemoryTracker.h"
void* operator new(std::size_t size) {
    void* ptr = MemoryManager::Allocate(size);
    if (!ptr) {
        throw std::bad_alloc();
    }
    return ptr;
}

// Overload scalar delete
void operator delete(void* ptr) noexcept {
    MemoryManager::Deallocate(ptr,0);
}

// Overload array new
void* operator new[](std::size_t size) {
    void* ptr = MemoryManager::Allocate(size);
    if (!ptr) {
        throw std::bad_alloc();
    }
    return ptr;
    }

    // Overload array delete
        void operator delete[](void* ptr) noexcept {
        MemoryManager::Deallocate(ptr,0);
        }