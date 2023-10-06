#include "EventHandler.h"


//macro to print name of enum
#define NAME_OF_ENUM(ENUM) #ENUM

void EventHandler::HandleEvents()
{

	//loop queue against callbacks
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		//printf("Event Type: %u\n", (event.type));
		for (auto& callback : eventCallbacks[event.type]) {
			callback(event);
		}
	}


}

void EventHandler::RegisterCallback(Uint32 eventType, std::function<void(const SDL_Event&)> callback)
{
	eventCallbacks[eventType].push_back(callback);
}
