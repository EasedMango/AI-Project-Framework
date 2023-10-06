#pragma once
using ID = unsigned int;
constexpr size_t MAX_COMPONENTS = 64;
inline static size_t componentTypeCounter = 0;
template <typename T>
static size_t GetComponentTypeId()
{
	static size_t typeId = componentTypeCounter++;
	return typeId;
}