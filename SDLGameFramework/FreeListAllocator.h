//
// Created by Joshua on 2023-10-22.
//

#ifndef MEMORYCPP_ALLOCATOR_H
#define MEMORYCPP_ALLOCATOR_H
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include "MemoryUtility.h"
#include <new>
#include <iostream>
#include <bitset>
namespace Memory {

	class Allocator {
	private:
		//size MUST be a multiple of 8
		struct Metadata {
		private:
			uint32_t sizeAndIsFree;
		public:
			uint32_t GetSize() const {
				return sizeAndIsFree & ~7;
			}

			void SetSize(const uint32_t& size) {
				//              clear the last 3 bytes of size then combine with the last 3 bytes of sizeAndFlags
				sizeAndIsFree = (size & ~7) | (sizeAndIsFree & 7);
			}

			bool IsFree() {
				return sizeAndIsFree & 1;
			}

			void SetIsFree(bool isFree) {
				sizeAndIsFree = (sizeAndIsFree & ~7) | (isFree & 7);
			}

			void SetSizeAndIsFree(uint32_t size, bool isFree) {
				sizeAndIsFree = (size & ~7) | (isFree & 7);
			}
		};

		struct Block {
		private:
			Metadata header;
			//Metadata is used to represent the footer's size in the Block.
			Metadata payload;

			//The footer would be below the payload
			//Metadata footer
			Metadata* GetFooter() { return reinterpret_cast<Metadata*>(PTR(&header) + header.GetSize() - METADATA_SIZE); }

			void SyncFooter() {
				*GetFooter() = header;
			}

		public:

			bool Validate() {
				auto footer = GetFooter();

				auto header32 = *reinterpret_cast<uint32_t*>(&header);
				//	std::cout << "Header Address: " << &header << std::endl;
				//	std::cout << "Header32: " << header32 << std::endl;
				auto footer32 = *reinterpret_cast<uint32_t*>(footer);
				//	std::cout << "Footer Address: " << footer << std::endl;
					//std::cout << "Footer32: " << footer32 << std::endl;

				bool valid = footer32 == header32;
				//	DebugPrint();
				return valid;
			}

			void DebugPrint() {
				auto footer = GetFooter();

				std::cout << "Header\n" << "    Size: " << header.GetSize() << "\n    IsFree: " << std::boolalpha << header.IsFree() << std::endl;
				std::cout << "Footer <<"
					<< footer << "\n" << "    Size: " << footer->GetSize() << "\n    IsFree: " << std::boolalpha << footer->IsFree() << std::endl;
			}
			Block(uint32_t size, bool isFree) : header(), payload() {
				SetSizeAndIsFree(size, isFree);
			}

			void SetSizeAndIsFree(uint32_t size, bool isFree) {
				header.SetSizeAndIsFree(size, isFree);
				SyncFooter();
			}

			uint32_t GetSize() {
				return header.GetSize();
			}

			bool IsFree() {
				return header.IsFree();
			}

		};

		static constexpr auto BLOCK_SIZE = sizeof(Block);

		static constexpr auto MIN_PAYLOAD_SIZE = 8;

		static constexpr auto MIN_BLOCK_SIZE = sizeof(Block) + MIN_PAYLOAD_SIZE;

		static constexpr auto METADATA_SIZE = sizeof(Metadata);



		Block* heapStart;
		void* heapEnd;
		size_t heapSize;



		uint32_t RoundUpSizeTo8(uint32_t size) {
			return (size + 7) & ~7;
		}



	protected:
		friend class MemoryManager;


	public:
		void CleanUp()
		{
			free(heapStart);
		}
		bool WithinHeap(void* address) {
			return address >= heapStart && address < heapEnd;
		}

		bool VerifyHeap()
		{
		//	printf("--------------------------------------------------------------------------------------Verifying heap\n");
			auto current = heapStart;
			int count = 0;

		//	std::cout << "HeapStart: " << heapStart << std::endl;
		//	std::cout << "HeapEnd:   " << heapEnd << std::endl;
			auto prev = current;

			uint32_t totalSize = 0;
			//PrintMemoryInfo((char*)(heapStart), (char* )(heapEnd));
			while (current < heapEnd) {

				if (!WithinHeap(current))
				{
					printf("Block %i at %p is not within heap\n", count, current);
					return false;
				}

				if (current < prev)
				{
					printf("Block %i at %p is before previous block\n", count, current);
					return false;
				}

				if (!current->Validate()) {
					printf("Block %i at %p is invalid\n", count, current);
					current->DebugPrint();
				}
				totalSize += current->GetSize();
				//printf("Total Size: %u\n", totalSize);
			//	printf("Block  %i at %p is valid\n", count, current);
				//current->DebugPrint();
				prev = current;
				current = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(current) + current->GetSize());
				count++;
			}
			return true;
		}


		Allocator() : heapStart(nullptr), heapEnd(nullptr), heapSize(0) {

		}

		Allocator(void* start, void* end, size_t size) : heapStart(new (start) Block(size + BLOCK_SIZE, true)), heapEnd(end), heapSize(size) {}




		bool TrySplitBlock(Block* allocateBlock, uint32_t size) {
			auto totalAllocationSize = RoundUpSizeTo8(size + BLOCK_SIZE);
			if (allocateBlock->GetSize() >= totalAllocationSize) {
				auto remainingSize = allocateBlock->GetSize() - totalAllocationSize;
				if (remainingSize >= MIN_BLOCK_SIZE) {
					allocateBlock->SetSizeAndIsFree(totalAllocationSize, false);
					auto freeBlock = new(PTR(allocateBlock) + totalAllocationSize) Block(remainingSize, true);
					return true;
				}
			}
			// Removed unnecessary state change
			return false;
		}

		void* Alloc(uint32_t size, size_t alignment) {
			if (size == 0) {
				printf("Requested Alloc size was zero\n");
				return nullptr;
			}
			auto current = heapStart;
			while (current < heapEnd) {
				if (!current->Validate()) {
					VerifyHeap();
					return nullptr;
				}
				if (current->IsFree() && TrySplitBlock(current, size)) {  // Used the return value of TrySplitBlock
					VerifyHeap();
					return PTR(current) + METADATA_SIZE;
				}
				current = reinterpret_cast<Block*>(PTR(current) + current->GetSize());
			}

			printf("No free block found\n");
			//VerifyHeap();
			return nullptr;
		}


		void AttemptCoalesce(Block* block)
		{
			// Coalesce with next block if possible
			auto nextBlock = reinterpret_cast<Block*>(PTR(block) + block->GetSize());
			if (PTR(nextBlock) < PTR(heapEnd) && nextBlock->IsFree()) {
				block->SetSizeAndIsFree(block->GetSize() + nextBlock->GetSize(), true);
			}

			// Coalesce with previous block if possible
			auto prevFooter = reinterpret_cast<Metadata*>(PTR(block) - METADATA_SIZE);
			if (PTR(prevFooter) >= PTR(heapStart) && prevFooter->IsFree()) {
				auto prevBlockSize = prevFooter->GetSize();
				auto previousBlock = reinterpret_cast<Block*>(PTR(block) - prevBlockSize);
				previousBlock->SetSizeAndIsFree(previousBlock->GetSize() + block->GetSize(), true);
			}
		}


		void Dealloc(void* address) {
			if (address == nullptr) {
				printf("Cannot dealloc nullptr\n");
				return;  // Added return to prevent further execution
			}
			auto block = reinterpret_cast<Block*>(PTR(address) - METADATA_SIZE);
			block->SetSizeAndIsFree(block->GetSize(), true);

			// Uncommented AttemptCoalesce to reduce fragmentation
			AttemptCoalesce(block);
			//VerifyHeap();
		}
	};

} // Memory

#endif //MEMORYCPP_ALLOCATOR_H
