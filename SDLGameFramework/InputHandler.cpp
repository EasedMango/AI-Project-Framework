#include "InputHandler.h"
#include "EventHandler.h"
std::size_t InputHandler::SDL_MouseButtonEventHash::operator()(const SDL_MouseButtonEvent& e) const
{
	// Implement your custom hash function here
	return std::hash<int>()(e.button) ^ std::hash<int>()(e.clicks) ^ std::hash<int>()(e.state);
}

bool InputHandler::SDL_MouseButtonEventEqual::operator()(const SDL_MouseButtonEvent& lhs,
	const SDL_MouseButtonEvent& rhs) const
{
	// Implement your custom equality check here
	return lhs.button == rhs.button && lhs.clicks == rhs.clicks && lhs.state == rhs.state;
}

InputHandler::InputHandler() = default;

void InputHandler::InjectHandler(Ref<EventHandler>& sdlEventHandler)
{
	this->sdlEventHandler = sdlEventHandler;
}



void InputHandler::Start()
{
	sdlEventHandler->RegisterCallback(SDL_KEYDOWN, [this](const SDL_Event& e) {this->KeyDown(e); });
	sdlEventHandler->RegisterCallback(SDL_KEYUP, [this](const SDL_Event& e) {this->KeyUp(e); });
	sdlEventHandler->RegisterCallback(SDL_MOUSEBUTTONDOWN, [this](const SDL_Event& e) {this->MouseButtonDown(e); });
	sdlEventHandler->RegisterCallback(SDL_MOUSEBUTTONUP, [this](const SDL_Event& e) {this->MouseButtonUp(e); });
	sdlEventHandler->RegisterCallback(SDL_MOUSEMOTION, [this](const SDL_Event& e) {this->MouseMotion(e); });
	//sdlEventHandler->RegisterCallback(SDL_MOUSEMOTION, [this](const SDL_Event& e) {this->KeyDown(e); });
	//  sdlEventHandler->RegisterCallback(SDL_MOUSEWHEEL, [this](const SDL_Event& e) {this->KeyDown; });

}

void InputHandler::KeyDown(const SDL_Event& e)
{
	std::lock_guard<std::mutex> lock(mutex);

	const SDL_Keycode key = e.key.keysym.sym;


	if (keyDownCallbacks.contains(key)) {
		keyDownCallbacks[key](e);
	}
	keysHeld.insert(key);

}

void InputHandler::KeyUp(const SDL_Event& e)
{
	std::lock_guard<std::mutex> lock(mutex);

	const SDL_Keycode key = e.key.keysym.sym;

	if (keyUpCallbacks.contains(key)) {
		keyUpCallbacks[key](e);
	}
	keysHeld.erase(key);
}

void InputHandler::KeyHeld(const SDL_Event& e)
{
	std::lock_guard<std::mutex> lock(mutex);

	const SDL_Keycode key = e.key.keysym.sym;

	if (keyHoldCallbacks.contains(key)) {
		keyHoldCallbacks[key]();
	}
}

void InputHandler::MouseButtonDown(const SDL_Event& e)
{
	std::lock_guard<std::mutex> lock(mutex);

	const auto button = e.button;

	if (mouseButtonDownCallbacks.contains(button)) {
		mouseButtonDownCallbacks[button](e);
	}
}

void InputHandler::MouseButtonUp(const SDL_Event& e)
{
	std::lock_guard<std::mutex> lock(mutex);

	const auto button = e.button;

	if (mouseButtonUpCallbacks.contains(button)) {
		mouseButtonUpCallbacks[button](e);
	}
}

void InputHandler::MouseMotion(const SDL_Event& e)
{
	std::lock_guard<std::mutex> lock(mutex);

	prevMousePosition = mousePosition;
	mousePosition .x = e.motion.x;
	mousePosition .y = e.motion.y;

	for (const auto& callback : mouseMotionCallbacks) {
		callback(e);
	}
}



void InputHandler::KeyHoldChecker()
{
	for (const auto& key : keysHeld)
	{
		if (keyHoldCallbacks.contains(key))
			keyHoldCallbacks[key]();
	}
}

InputHandler& InputHandler::Instance()
{
	static InputHandler instance;  // Guaranteed to be destroyed, instantiated on first use
	return instance;
}

bool InputHandler::KeyHeld(SDL_Keycode key)
{
	std::lock_guard<std::mutex> lock(mutex);
	return keysHeld.contains(key);
}

void InputHandler::RegisterKeyPressCallback(SDL_Keycode key, const  std::function<void(const SDL_Event&)> callback)
{
	std::lock_guard<std::mutex> lock(mutex);
	keyDownCallbacks[key] = callback;
}

void InputHandler::RegisterKeyHoldCallback(SDL_Keycode key, std::function<void()> callback)
{
	std::lock_guard<std::mutex> lock(mutex);
	keyHoldCallbacks[key] = callback;
}

void InputHandler::RegisterKeyReleaseCallback(SDL_Keycode key, std::function<void(const SDL_Event&)> callback)
{
	std::lock_guard<std::mutex> lock(mutex);
	keyUpCallbacks[key] = callback;
}
