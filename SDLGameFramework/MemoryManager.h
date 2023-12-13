#pragma once
#include <mutex>

#include "BlockManager.h"
#include "FreeListAllocator.h"
#include "MemoryTracker.h"
#include "Timing.h"






class MemoryManager
{

	static std::mutex& GetMutex()
	{
		static std::mutex mutex;
		return mutex;
	}




	static Memory::Allocator& FreeList(void* start = nullptr, void* end = nullptr, size_t size = 0)
	{
		static Memory::Allocator blockManager = Memory::Allocator(start, end, size);
		return blockManager;
	}
	inline static bool clean = false;
	inline static int newAndDeleteInFrameNum = 0;
	static MemoryTracker& GetMemoryTracker()
	{
				static MemoryTracker memoryTracker = MemoryTracker();
		return memoryTracker;
	}


public:
	MemoryManager() = delete;

	static void Init(size_t bufferSize)
	{

		//FreeList().Init(bufferSize);

		void* buffer = calloc(bufferSize, 1);
		FreeList(buffer, PTR(buffer) + bufferSize, bufferSize);
		GetMemoryTracker();
	};

	~MemoryManager()
	{
		CleanUp();
	}

	static void CleanUp()
	{
		printf("Cleaning Memory\n");
		if (!clean)
		{
			clean = true;
			FreeList().CleanUp();
			
		}
		//MemoryTracker::DebugMemoryInfo();
		//	GetFreeList()->CleanUp();
	};

	static void CountAllocations()
	{
		printf("Number of new and delete in frame: %d\n", newAndDeleteInFrameNum);
		newAndDeleteInFrameNum = 0;
	}

	static void* Allocate(size_t numBytes, size_t alignment = alignof(std::max_align_t))
	{
		//TIMING("Allocate");
		//auto ptr = malloc(numBytes);
		auto ptr = FreeList().Alloc(numBytes, alignment);//malloc(numBytes);//Blocks().Allocate(numBytes);
		if (ptr == nullptr)
		{
			std::cout << "Failed to allocate memory" << std::endl;
			return nullptr;
		}
		//if(!FreeList().WithinHeap(ptr))
		//{
		//	std::cout << "new not within heap" << std::endl;
		//}
		MemoryTracker::UpdateMemoryTracker(true, ptr, numBytes);
		return ptr;
	};
	static void Deallocate(void* memoryLocation, size_t numBytes, size_t alignment = alignof(std::max_align_t))
	{
		//TIMING("Deallocate");
		//if (!FreeList().WithinHeap(memoryLocation))
		//{
		//	std::cout << "delete not within heap" << std::endl;
		//}
		if (memoryLocation == nullptr) {
			std::cout << "Trying to delete nullptr" << std::endl;
			return;
		}

		MemoryTracker::UpdateMemoryTracker(false, memoryLocation, numBytes);
		//free(memoryLocation);
		FreeList().Dealloc(memoryLocation);//free(memoryLocation);//Blocks().Free(memoryLocation);
		//Blocks().Free(memoryLocation);
	};
};
