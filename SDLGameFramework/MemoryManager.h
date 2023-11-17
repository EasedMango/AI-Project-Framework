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




	static Memory::Allocator& FreeList(void* start = nullptr,void* end=nullptr, size_t size=0)
	{
		static Memory::Allocator blockManager = Memory::Allocator(start,end,size);
		return blockManager;
	}
	inline static bool clean=false;
	inline static int newAndDeleteInFrameNum = 0;

public:
	MemoryManager() = delete;

	static void Init(size_t bufferSize)
	{

		//FreeList().Init(bufferSize);

		void* buffer = calloc(bufferSize,1);
		FreeList(buffer, PTR(buffer)+bufferSize,bufferSize);

		//MemoryTracker();

	};

	~MemoryManager()
	{
		CleanUp();
	}

	static void CleanUp()
	{
		if(!clean)
		{
			clean = true;
			FreeList().CleanUp();
		}
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
		if(!FreeList().WithinHeap(ptr))
		{
			printf("Within heap\n");
		}
		MemoryTracker::UpdateMemoryTracker(true, ptr, numBytes);
		return ptr;
	};
	static void Deallocate(void* memoryLocation, size_t numBytes, size_t alignment = alignof(std::max_align_t))
	{
		//TIMING("Deallocate");
		if (!FreeList().WithinHeap(memoryLocation))
		{
			printf("Within heap\n");
		}
		MemoryTracker::UpdateMemoryTracker(false, memoryLocation, numBytes);
		//free(memoryLocation);
		FreeList().Dealloc(memoryLocation);//free(memoryLocation);//Blocks().Free(memoryLocation);
		//Blocks().Free(memoryLocation);
	};
};
