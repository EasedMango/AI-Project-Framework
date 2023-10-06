#pragma once
#include <mutex>
#include <functional>
#include "SDL_events.h"

class EventHandler {
private:


    std::mutex queueMutex;
    std::unordered_map<Uint32, std::vector<std::function<void(const SDL_Event&)>>> eventCallbacks;

protected:
    friend class Core;
    void HandleEvents();
public:

    void RegisterCallback(Uint32 eventType, std::function<void(const SDL_Event&)> callback);
};
