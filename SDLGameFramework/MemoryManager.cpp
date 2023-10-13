#include "MemoryManager.h"

#include "MemoryTracker.h"

struct ArraySize {
	std::size_t numBytes;
};

void* operator new(std::size_t numBytes)
{
	constexpr int asize = sizeof(std::align_val_t);
	constexpr size_t mask = sizeof(size_t);
	//std::cout << "allocating " << numBytes <<  "\n";
	void* ptr = MemoryManager::Allocate(numBytes);
	if (!ptr) throw std::bad_alloc();
	return ptr;
}

void operator delete(void* memoryLocation, std::size_t numBytes) noexcept {
	//	std::cout << "freeing " << numBytes  <<  "\n";
	MemoryManager::Deallocate(memoryLocation, numBytes);
}

void* operator new[](std::size_t numBytes) {
	//std::cout << "allocating an array of " << numBytes  << "\n";
	ArraySize* array = reinterpret_cast<ArraySize*>(MemoryManager::Allocate(numBytes + sizeof(ArraySize)));
	array->numBytes = numBytes;
	return array + 1;
	}

	void operator delete[](void* memoryLocation) noexcept {
		ArraySize* array = reinterpret_cast<ArraySize*>(memoryLocation) - 1;
		//std::cout << "freeing array " << array->numBytes  <<  "\n";
		MemoryManager::Deallocate(memoryLocation, array->numBytes);
		}

		void* operator new(std::size_t numBytes, std::align_val_t alignment)
		{
			constexpr int asize = sizeof(std::align_val_t);
			constexpr size_t mask = sizeof(size_t);
			//std::cout << "allocating " << numBytes <<  "\n";
			return MemoryManager::Allocate(numBytes, static_cast<size_t>(alignment));
		}

		void operator delete(void* memoryLocation, std::size_t numBytes, std::align_val_t alignment) noexcept {
			//	std::cout << "freeing " << numBytes  <<  "\n";
			MemoryManager::Deallocate(memoryLocation, numBytes, static_cast<size_t>(alignment));
		}

		void* operator new[](std::size_t numBytes, std::align_val_t alignment) {
			//std::cout << "allocating an array of " << numBytes  << "\n";
			ArraySize* array = reinterpret_cast<ArraySize*>(MemoryManager::Allocate(numBytes + sizeof(ArraySize), static_cast<size_t>(alignment)));
			array->numBytes = numBytes;
			return array + 1;
			}

			void operator delete[](void* memoryLocation, std::align_val_t alignment) noexcept {
				ArraySize* array = reinterpret_cast<ArraySize*>(memoryLocation) - 1;
				//std::cout << "freeing array " << array->numBytes  <<  "\n";
				MemoryManager::Deallocate(memoryLocation, array->numBytes, static_cast<size_t>(alignment));
				}

