#pragma once
#include <mutex>

#include "BlockManager.h"
#include "MemoryTracker.h"



static constexpr size_t BYTE = 1;
static constexpr size_t KILO_BYTE = 1024 * BYTE;
static constexpr size_t MEGA_BYTE = 1024 * KILO_BYTE;
static constexpr size_t GIGA_BYTE = 1024 * MEGA_BYTE;




class MemoryManager
{

	static std::mutex& GetMutex()
	{
		static std::mutex mutex;
		return mutex;
	}



	static BlockManager& Blocks(size_t size = 0)
	{
		static BlockManager blockManager = BlockManager(size);
		return blockManager;
	}

	inline static int newAndDeleteInFrameNum = 0;

public:
	MemoryManager() = delete;

	static void Init(size_t bufferSize)
	{

		//FreeList().Init(bufferSize);
		Blocks(bufferSize);

	};

	static void CleanUp()
	{
		//	GetFreeList()->CleanUp();
	};

	static void CountAllocations()
	{
		printf("Number of new and delete in frame: %d\n", newAndDeleteInFrameNum);
		newAndDeleteInFrameNum = 0;
	}

	static void* Allocate(size_t numBytes, size_t alignment = alignof(std::max_align_t))
	{
		auto ptr = malloc(numBytes);//Blocks().Allocate(numBytes);
		MemoryTracker::UpdateMemoryTracker(false, ptr, numBytes);
		return ptr;
	};
	static void Deallocate(void* memoryLocation, size_t numBytes, size_t alignment = alignof(std::max_align_t))
	{
		MemoryTracker::UpdateMemoryTracker(false, memoryLocation, numBytes);
		free(memoryLocation);//Blocks().Free(memoryLocation);
		//Blocks().Free(memoryLocation);
	};
};
