#include "MemoryTracker.h"


bool StackAllocatedMap::insert(void* address, const AllocationInfo& info)
{
    if (count >= MaxEntries) {
        // The map is full
        printf("The map is full\n");
        return false;
    }
    data[count++] = { address, info };
    return true;
}

bool StackAllocatedMap::erase(void* address)
{
    for (size_t i = 0; i < count; ++i) {
        if (data[i].first == address) {
            // Erase by moving all subsequent entries one position to the left
            for (size_t j = i; j < count - 1; ++j) {
                data[j] = data[j + 1];
            }
            --count;
            return true;
        }
    }
    // Address not found
    return false;
}

AllocationInfo* StackAllocatedMap::find(void* address)
{
    for (size_t i = 0; i < count; ++i) {
        if (data[i].first == address) {
            return &data[i].second;
        }
    }
    // Address not found
    return nullptr;
}

bool FixedSizeQueue::push(const AllocationAction& action)
{
    if (count >= MaxQueue) return false;  // Queue is full
    data[backIndex] = action;
    backIndex = (backIndex + 1) % MaxQueue;
    ++count;
    return true;
}

bool FixedSizeQueue::pop(AllocationAction& action)
{
    if (count == 0) return false;  // Queue is empty
    action = data[frontIndex];
    frontIndex = (frontIndex + 1) % MaxQueue;
    --count;
    return true;
}

bool FixedSizeQueue::empty() const
{
    return count == 0;
}

void MemoryTracker::Updater()
{
  //  printf("TRACKING`````````````````````````````````````````\n");
    while (true) {
          //  printf("TRACKING`````````````````````````````````````````\n");
        std::unique_lock<std::mutex> lock(mtx);
        //print current queue size
      //  printf("Queue size: %zu\n", actionsInstance.count);  // Updated here to work with your custom queue
        //print current map size
      //  printf("Map size: %zu\n", allocationsInstance.count);  // Updated here to work with your custom map
       // printf("memo1");
        cv.wait(lock, [] { return !actionsInstance.empty() || exitFlag; });  // Updated here
        if (exitFlag && actionsInstance.empty()) break;  // And here
        if(actionsInstance.empty()) printf("\nEmpty!\n");;  // Updated here to work with your custom queue)
        while (!actionsInstance.empty()) {  // And here
           //    printf("TRACKING MEMORUY`````````````````````````````````````````\n");
            AllocationAction action;
            actionsInstance.pop(action);  // Updated here to work with your custom queue
           // printf("action : ");
           // printf("%d\n", action.isAllocation);


            if (action.isAllocation) {
                allocationsInstance.insert(action.address, { action.size });  // Updated here
                currentMemoryUsage += action.size;
                peakMemoryUsage = std::max(peakMemoryUsage, currentMemoryUsage);
                totalMemoryAllocations++;
             //  printf("Allocated %zu bytes at address %p\n", action.size, action.address);
            }
            else {
                auto info = allocationsInstance.find(action.address);  // Updated here
                if (info != nullptr) {
                    currentMemoryUsage -= info->size;
                    allocationsInstance.erase(action.address);  // Updated here
                    totalMemoryDeallocations++;
                //   printf("Deallocated %zu bytes at address %p\n", info->size, action.address);
                }
            }
        }
        lock.unlock();
    }
}
MemoryTracker::MemoryTracker()
{
    printf("mem");
    if (!updaterThread.joinable()) {
        updaterThread = std::thread(Updater);
    }
}

MemoryTracker::~MemoryTracker()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        exitFlag = true;
    }
    cv.notify_one();
    updaterThread.join();
    DebugMemoryInfo();
}

void MemoryTracker::UpdateMemoryTracker(const bool& isAllocation, const void* address, const size_t& size)
{
    std::lock_guard<std::mutex> lock(mtx);
    actionsInstance.push({ isAllocation, const_cast<void*>(address), size });
    cv.notify_one();
}

void MemoryTracker::DebugMemoryInfo()
{
    std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex for thread-safety

    printf("Current memory usage: %zu\n", currentMemoryUsage);
    printf("Peak memory usage: %zu\n", peakMemoryUsage);
    printf("Total memory allocations: %zu\n", totalMemoryAllocations);
    printf("Total Memory deallocations: %zu\n", totalMemoryDeallocations);
    printf("Current total memory leaks: %zu\n", totalMemoryAllocations - totalMemoryDeallocations);
   
    if (allocationsInstance.count == 0) {  // Updated here
        printf("No memory leaks detected.\n");
    }
    else {
        printf("Memory leaks detected:\n");
        for (size_t i = 0; i < allocationsInstance.count; ++i) {  // Updated here to work with your custom map
            auto& [address, info] = allocationsInstance.data[i];  // And here
            printf("Address: %p, Size: %zu bytes\n", address, info.size);
        }
    }
   // _CrtDumpMemoryLeaks();
    printf("\n");
}

