//#include "FreeListAllocator.h"
//
//#include "MemoryUtility.h"
//
//std::pair<FreeListAllocator::FreeBlock*, size_t> FreeListAllocator::FindFreeBlock(const size_t size,
//	const size_t alignment)
//{
//	const auto headerPayloadSize = size + sizeof(AllocationHeader);
//
//	const size_t alignmentRequirement = std::max(alignment, alignof(AllocationHeader));
//
//	FreeBlock* currentBlock = freeList;
//
//	size_t padding = 0;
//
//	while (currentBlock != nullptr)
//	{
//		void* payloadAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(currentBlock) + sizeof(AllocationHeader));
//
//		size_t requiredPadding = GetAlignmentAdjustment(alignmentRequirement, payloadAddress);
//		size_t totalSize = headerPayloadSize + requiredPadding;
//
//		if (totalSize <= currentBlock->size)
//		{
//			return { currentBlock,padding };
//		}
//		currentBlock = currentBlock->next;
//	}
//	return { nullptr,padding };
//}
//
//void FreeListAllocator::AddFreeBlock(FreeBlock* block)
//{
//	if (freeList == nullptr)
//	{
//		freeList = block;
//		return;
//	}
//
//	if (reinterpret_cast<uintptr_t>(block) < reinterpret_cast<uintptr_t>(freeList))
//	{
//		block->next = freeList;
//		freeList->prev = block;
//		freeList = block;
//		return;
//	}
//
//	auto currentBlock = freeList;
//
//	while (currentBlock->next != nullptr)
//	{
//		if (reinterpret_cast<uintptr_t>(block) > reinterpret_cast<uintptr_t>(currentBlock) &&
//			reinterpret_cast<uintptr_t>(block) < reinterpret_cast<uintptr_t>(currentBlock->next))
//		{
//			block->next = currentBlock->next;
//			currentBlock->next->prev = block;
//			block->prev = currentBlock;
//			currentBlock->next = block;
//			AttemptCoalesceBlock(block);
//		}
//		currentBlock = currentBlock->next;
//	}
//
//	currentBlock->next = block;
//	block->prev = currentBlock;
//	AttemptCoalesceBlock(block);
//}
//
//void FreeListAllocator::RemoveFreeBlock(FreeBlock* block)
//{
//
//	if (block == freeList)
//	{
//		freeList = block->next;
//		return;
//	}
//
//	if (block->prev != nullptr)
//	{
//		block->prev->next = block->next;
//	}
//
//	if (block->next != nullptr)
//	{
//		block->next->prev = block->prev;
//	}
//
//}
//
//void FreeListAllocator::AttemptCoalesceBlock(FreeBlock* block)
//{
//
//	if (block->prev != nullptr)
//	{
//		uintptr_t blockStart = reinterpret_cast<uintptr_t>(block->prev) + block->prev->size;
//
//		if ((reinterpret_cast<uintptr_t>(block) == blockStart))
//		{
//			block->prev->next = block->next;
//			if (block->next != nullptr)
//			{
//				block->next->prev = block->prev;
//			}
//			block->prev->size += block->size;
//			block = block->prev;
//		}
//	}
//
//	if (block->next != nullptr)
//	{
//		uintptr_t blockEnd = reinterpret_cast<uintptr_t>(block) + block->size;
//
//		if (blockEnd == reinterpret_cast<uintptr_t>(block->next))
//		{
//			block->size += block->next->size;
//			block->next = block->next->next;
//			if (block->next != nullptr)
//			{
//				block->next->prev = block;
//			}
//		}
//	}
//}
//
//FreeListAllocator::FreeListAllocator() : freeList(nullptr), bufferStart(nullptr), bufferEnd(nullptr)
//{
//}
//
//FreeListAllocator::FreeListAllocator(size_t buffer_size, void* memory) :FreeListAllocator()
//{
//	Init(buffer_size, memory);
//}
//
//
//FreeListAllocator::FreeListAllocator(FreeListAllocator&& other) noexcept
//{
//	freeList = other.freeList;
//	bufferStart = other.bufferStart;
//	bufferEnd = other.bufferEnd;
//	bufferSize = other.bufferSize;
//
//	other.freeList = nullptr;
//	other.bufferStart = nullptr;
//	other.bufferEnd = nullptr;
//	other.bufferSize = 0;
//
//}
//
//FreeListAllocator::~FreeListAllocator()
//{
//}
//
//void FreeListAllocator::Init(size_t buffer_size, void* memory)
//{
//	bufferStart = memory;
//	bufferEnd = reinterpret_cast<uintptr_t*>(memory) + buffer_size;
//
//	FreeBlock* initialBlock = new (bufferStart) FreeBlock(buffer_size);
//	AddFreeBlock(initialBlock);
//}
//
//void* FreeListAllocator::Allocate(const size_t size, const size_t alignment)
//{
//
//	auto [freeBlock, padding] = FindFreeBlock(size, alignment);
//
//	if (freeBlock == nullptr)
//	{
//		printf("No free block found\n");
//		return nullptr;
//	}
//
//	RemoveFreeBlock(freeBlock);
//
//	FreeBlock oldBlockCopy = *freeBlock;
//
//	void* headerAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(freeBlock) + padding);
//	void* payloadAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(headerAddress) + sizeof(AllocationHeader));
//
//	//overwrites data pointed to by freeBlock 
//	auto payloadSpace = oldBlockCopy.size - padding - sizeof(AllocationHeader);
//
//	auto header = new (headerAddress) AllocationHeader(padding, payloadSpace);
//
//	uintptr_t payloadEnd = reinterpret_cast<uintptr_t>(payloadAddress) + size;
//	uintptr_t blockEnd = reinterpret_cast<uintptr_t>(freeBlock) + payloadSpace;
//
//	//padding to put after payload
//	auto adjustment = GetAlignmentAdjustment(alignof(FreeBlock), reinterpret_cast<void*>(payloadEnd));
//
//	auto unusedSpace = blockEnd - (payloadEnd + adjustment);
//
//	if (unusedSpace > MIN_BLOCK_SIZE)
//	{
//		header->size -= unusedSpace;
//
//		void* newBlockAddress = reinterpret_cast<void*>(payloadEnd + adjustment);
//
//		FreeBlock* newBlock = new (newBlockAddress) FreeBlock(unusedSpace);
//
//		AddFreeBlock(newBlock);
//	}
//
//	return payloadAddress;
//}
//
//void FreeListAllocator::Free(void* memory)
//{
//	if (memory == nullptr)
//	{
//		return;
//	}
//
//	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<uintptr_t>(memory) - sizeof(AllocationHeader));
//
//	auto blockSize = header->size + header->padding + sizeof(AllocationHeader);
//	void* blockStart = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(header) - header->padding);
//
//	if (!IsMemoryAligned(alignof(FreeBlock), blockStart))
//	{
//		printf("Block is not aligned\n");
//		return;
//	}
//	FreeBlock* freeBlock = new (blockStart) FreeBlock(blockSize);
//
//	AddFreeBlock(freeBlock);
//}
//
//bool FreeListAllocator::IsInitialized() const
//{
//	return bufferStart != nullptr;
//}
//
//bool FreeListAllocator::IsMemoryInRange(void* memory) const
//{
//	return memory >= bufferStart && memory <= bufferEnd;
//}
//
//uintptr_t FreeListAllocator::Begin() const
//{
//	return reinterpret_cast<uintptr_t>(bufferStart);
//}
//
//uintptr_t FreeListAllocator::End() const
//{
//	return reinterpret_cast<uintptr_t>(bufferEnd);
//}
