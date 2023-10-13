#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

struct AllocationInfo {
	size_t size;
	// ... other info ...
};

struct AllocationAction {
	bool isAllocation;
	void* address;
	size_t size;
};
#include <iostream>
#include <array>
#include <utility>  // for std::pair

constexpr size_t MaxEntries = 10000;

constexpr size_t entriesSize = sizeof(std::pair<void*, AllocationInfo>) * MaxEntries;
constexpr size_t MaxQueue = 100;

struct StackAllocatedMap {

	std::array<std::pair<void*, AllocationInfo>, MaxEntries> data;

	size_t count = 0;


	bool insert(void* address, const AllocationInfo& info);

	bool erase(void* address);

	AllocationInfo* find(void* address);
};
struct FixedSizeQueue {

	AllocationAction data[MaxQueue] = {};
	size_t frontIndex = 0;
	size_t backIndex = 0;

	size_t count = 0;


	bool push(const AllocationAction& action);

	bool pop(AllocationAction& action);

	bool empty() const;
};



class MemoryTracker {
private:
	inline static size_t currentMemoryUsage = 0;
	inline static size_t peakMemoryUsage = 0;
	inline static size_t totalMemoryAllocations = 0;
	inline static size_t totalMemoryDeallocations = 0;
	inline static StackAllocatedMap allocationsInstance;
	inline static FixedSizeQueue actionsInstance;
	inline static std::mutex mtx;
	inline static std::condition_variable cv;
	inline static bool exitFlag = false;
	inline static std::thread updaterThread;

	static void Updater();

public:
	MemoryTracker();

	~MemoryTracker();
	static MemoryTracker& GetMemoryTracker() {
		static MemoryTracker instance;  // Constructed on first call
		return instance;
	}
	static void UpdateMemoryTracker(const bool& isAllocation, const void* address, const size_t& size);


	static  void DebugMemoryInfo();
};


