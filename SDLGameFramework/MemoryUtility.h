#pragma once
#include <cstdint>
#include <cstdio>

using uintptr = uint8_t*;
#define PTR(ptr) reinterpret_cast<uintptr>(ptr)
static constexpr size_t BYTE = 1;
static constexpr size_t KILO_BYTE = 1024 * BYTE;
static constexpr size_t MEGA_BYTE = 1024 * KILO_BYTE;
static constexpr size_t GIGA_BYTE = 1024 * MEGA_BYTE;

inline uintptr_t GetAlignmentOffset(size_t alignment, void* address)
{
	return reinterpret_cast<uintptr_t>(address) & (alignment - 1);
}

inline uintptr_t GetAlignmentAdjustment(size_t alignment, void* address)
{
	auto offset = GetAlignmentOffset(alignment, address);

	if (offset == 0)
	{
		return 0;
	}
	return alignment - offset;
}


inline bool IsMemoryAligned(size_t alignment, void* address)
{
	return GetAlignmentOffset(alignment, address) == 0;
}
inline void* AlignMemory(size_t alignment, size_t size, void* bufferStart, size_t bufferSize)
{
	uintptr_t adjustment = GetAlignmentAdjustment(alignment, bufferStart);

	if (bufferSize < adjustment || bufferSize - adjustment < size)
	{
		printf("Not enough memory to allocate");
		return nullptr;
	}

	bufferStart = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(bufferStart) + adjustment);
	return bufferStart;

}