#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>

class BlockManager
{
public:
	uint8_t GetAlignmentOffset(void* address, size_t alignment)
	{
		return static_cast<uint8_t>(alignment - (reinterpret_cast<uintptr_t>(address) & (alignment - 1)));
	}

	void* AlignTo8Bytes(void* address)
	{
		return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(address) + 7) & ~7);
	}

	void AlignTo8Bytes(uint32_t& numBytes)
	{
		numBytes = (numBytes + 7) & ~7;
	}


	//bridge between blocks, header keeps track of prev block size
	//used to traverse the list in either direction
	//keeps payload separate from header
	//prev represent the previous block, it's size and allocated bit
	//next represent the next block, it's size and allocated bit
	//a bridge represents two different blocks
	//this allows calls directly to prev and next without having to calculate where they are instead
	//size should always be a multiple of 8 bytes so the last 3 bytes are free  
	class BridgeBlock // Size of BridgeBlock is 8 bytes
	{
	private:
		uint32_t prev; // size of prev and its allocatedBit | 4 bytes

		uint32_t next; // size of next and allocatedBit | 4 bytes
	public:
		BridgeBlock() = delete;
		BridgeBlock(uint32_t prevSize, uint32_t nextSize, bool prevAllocated, bool nextAllocated)
		{
			prev = prevSize;
			next = nextSize;
			SetPrevAllocated(prevAllocated);
			SetNextAllocated(nextAllocated);
		}

		uint32_t GetPrevSize() const { return prev & ~7; }
		void SetPrevSize(uint32_t size) { prev = (prev & 7) | (size & ~7); }


		uint32_t GetNextSize() const { return next & ~7; }
		void SetNextSize(uint32_t size) { next = (next & 7) | (size & ~7); }

		bool IsPrevAllocated() const { return prev & 1; }
		void SetPrevAllocated(bool isAllocated) { prev = isAllocated ? prev | 1 : prev & ~1; }

		bool IsNextAllocated() const { return next & 1; }
		void SetNextAllocated(bool isAllocated) { next = isAllocated ? next | 1 : next & ~1; }

		void* GetPrevPayload() { return reinterpret_cast<uint8_t*>(this) - GetPrevSize(); }

		void* GetNextPayload() { return reinterpret_cast<uint8_t*>(this) + sizeof(BridgeBlock) + GetNextSize(); }


		BridgeBlock* GetPrevBridge() { return reinterpret_cast<BridgeBlock*>(reinterpret_cast<uint8_t*>(this) - GetPrevSize()); }

		BridgeBlock* GetNextBridge() { return reinterpret_cast<BridgeBlock*>(reinterpret_cast<uint8_t*>(this) + sizeof(BridgeBlock) + GetNextSize()); }

	};

	//if the Block a Bridge is pointing to is free, then it's a FreeBlock
	//FreeBlocks keep an explicit list of free blocks
	//since the BridgeBlock is always used to link blocks, you don't need to keep track of the size of the block
	//or even the allocated bit, since a FreeBlock is always free if the BridgeBlock is pointing to it
	//
	struct FreeBlock
	{
		FreeBlock* prev;
		FreeBlock* next;

		//Size needs to be atleast 
		auto GetSize() { return GetHeaderBridge()->GetNextSize(); }

		BridgeBlock* GetHeaderBridge() { return reinterpret_cast<BridgeBlock*>(this) - 1; }
		BridgeBlock* GetFooterBridge() { return reinterpret_cast<BridgeBlock*>(reinterpret_cast<uint8_t*>(this) + GetSize()); }



		/*
		 *Taking a Free Block
		 *	|Bridge|____________________FreeBlock____________________|Bridge|
		 *Splitting it into two blocks
		 *	|Bridge|________Allocated_______|________FreeBlock_______|Bridge|
		 *If the left over space from the requested split size is large enough to hold a BridgeBlock+FreeBlock
		 *	|Bridge|________Allocated_______|Bridge|____FreeBlock____|Bridge|
		 *Else if there is not enough space for a BridgeBlock+FreeBlock, then just allocate the entire block
		 *	|Bridge|______________Allocated__________________|Padding|Bridge|
		 */
		 //Assumes numBytes is already aligned to 8 bytes
		FreeBlock* Split(uint32_t numBytes)
		{
			uint32_t currentBlockSize = GetSize();
			if (currentBlockSize < numBytes + MIN_BLOCK_SIZE)
			{
				return nullptr;
			}

			uint32_t newBlockSize = currentBlockSize - (numBytes + BRIDGE_SIZE);

			BridgeBlock* headerBridge = GetHeaderBridge();
			BridgeBlock* footerBridge = GetFooterBridge();

			// If there is enough space to create a new FreeBlock after splitting
			if (newBlockSize > MIN_BLOCK_SIZE)
			{
				// Create new BridgeBlock for the remaining free block
				uint8_t* newBridgeStart = reinterpret_cast<uint8_t*>(this) + numBytes;
				BridgeBlock* newBridge = new (newBridgeStart) BridgeBlock(numBytes, newBlockSize, true, false);

				// Update the current block (which will become the allocated block)
				headerBridge->SetNextSize(numBytes);
				headerBridge->SetNextAllocated(true);

				// Update footer bridge of the new free block
				footerBridge->SetPrevSize(newBlockSize);
				footerBridge->SetPrevAllocated(false);

				// Create the new FreeBlock
				uint8_t* newBlockStart = newBridgeStart + BRIDGE_SIZE;
				FreeBlock* newFreeBlock = new (newBlockStart) FreeBlock();

				// Handle linked list operations for the free block list here, if needed
				if (WithinRange(prev))
					prev->next = newFreeBlock;
				if (WithinRange(next))
					next->prev = newFreeBlock;

				return newFreeBlock;
			}

			// If there isn't enough space to create a new FreeBlock
			headerBridge->SetNextAllocated(true);
			footerBridge->SetPrevAllocated(true);

			// Handle linked list operations for the free block list here, if needed
			if (WithinRange(prev))
				prev->next = prev;
			if (WithinRange(next))
				next->prev = next;

			return nullptr;
		}



	};




	void* heapStart;
	void* heapEnd;
	size_t heapSize;

	FreeBlock* freeListHead;

	FreeBlock* FindFreeBlock(uint32_t numBytes)
	{
		FreeBlock* currentBlock = freeListHead;
		while (currentBlock != nullptr)
		{
			if (currentBlock->GetSize() >= numBytes)
			{
				return currentBlock;
			}
			currentBlock = currentBlock->next;
		}
		return nullptr;
	}

	static bool WithinRange(void* address, void* start = nullptr, void* end = nullptr)
	{
		static void* heapStart = start;
		static void* heapEnd = end;

		return address >= heapStart && address < heapEnd;
	}

public:
	static constexpr uint32_t BRIDGE_SIZE = sizeof(BridgeBlock);

	static constexpr uint32_t FREEBLOCK_SIZE = sizeof(FreeBlock);

	static constexpr uint32_t MIN_BLOCK_SIZE = BRIDGE_SIZE + FREEBLOCK_SIZE;
	BlockManager(size_t size)
	{
		heapSize = size;
		heapStart = malloc(size);
		heapEnd = reinterpret_cast<uint8_t*>(heapStart) + size;
		freeListHead = reinterpret_cast<FreeBlock*>(heapStart);
		freeListHead->prev = nullptr;
		freeListHead->next = nullptr;
		WithinRange(nullptr, heapStart, heapEnd);
		freeListHead->GetHeaderBridge()->SetPrevSize(0);
		freeListHead->GetHeaderBridge()->SetPrevAllocated(false);
		freeListHead->GetHeaderBridge()->SetNextSize(size - BRIDGE_SIZE);
		freeListHead->GetHeaderBridge()->SetNextAllocated(false);
		freeListHead->GetFooterBridge()->SetPrevSize(size - BRIDGE_SIZE);
		freeListHead->GetFooterBridge()->SetPrevAllocated(false);
		freeListHead->GetFooterBridge()->SetNextSize(0);
		freeListHead->GetFooterBridge()->SetNextAllocated(false);
	}
	//When allocating, you need to make sure the size is a multiple of 8 bytes, and that it's aligned to the strictest alignment

	void* Allocate(uint32_t numbytes, size_t alignment = alignof(std::max_align_t))
	{
		// existing alignment and size computation
		auto alignedSize = numbytes;
		AlignTo8Bytes(alignedSize);

		FreeBlock* oldFreeBlock = FindFreeBlock(alignedSize);
		if (oldFreeBlock == nullptr)
		{
			printf("No free block found\n");
			return nullptr;
		}

		FreeBlock* newFreeBlock = oldFreeBlock->Split(alignedSize);

		if (oldFreeBlock == freeListHead)
		{
			freeListHead = newFreeBlock; // Update the head of the free list
		}

		return reinterpret_cast<void*>(oldFreeBlock);
	}

	void Free(void* address)
	{
		if (address == nullptr)
		{
			return; // Nothing to free
		}

		// Get the BridgeBlock corresponding to this block
		uint8_t* blockStart = reinterpret_cast<uint8_t*>(address);
		BridgeBlock* headerBridge = reinterpret_cast<BridgeBlock*>(blockStart - sizeof(BridgeBlock));

		// Mark the block as free
		headerBridge->SetNextAllocated(false);

		// Get the footer bridge
		BridgeBlock* footerBridge = headerBridge->GetNextBridge();

		// Mark the next block's previous block as free
		footerBridge->SetPrevAllocated(false);

		// Create a FreeBlock to represent the newly freed block
		FreeBlock* freedBlock = reinterpret_cast<FreeBlock*>(blockStart);

		// Add to the free list
		freedBlock->prev = nullptr;
		freedBlock->next = freeListHead;
		if (freeListHead != nullptr)
		{
			freeListHead->prev = freedBlock;
		}
		freeListHead = freedBlock;
	}






};
