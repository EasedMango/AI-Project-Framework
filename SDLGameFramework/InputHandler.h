#include <SDL.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <glm/vec2.hpp>

#include "Common.h"


class EventHandler;



class InputHandler {
private:
	Ref<EventHandler> sdlEventHandler;
	std::mutex mutex;

	glm::vec2 prevMousePosition;
	glm::vec2 mousePosition;


	//Keyboard callbacks
	std::unordered_map<SDL_Keycode, std::function<void(const SDL_Event&)>> keyDownCallbacks;
	std::unordered_map<SDL_Keycode, std::function<void()>> keyHoldCallbacks;
	std::unordered_map<SDL_Keycode, std::function<void(const SDL_Event&)>> keyUpCallbacks;
	// Set of keys that are currently being held down
	std::unordered_set<SDL_Keycode> keysHeld;


	struct SDL_MouseButtonEventHash {
		std::size_t operator()(const SDL_MouseButtonEvent& e) const;
	};

	struct SDL_MouseButtonEventEqual {
		bool operator()(const SDL_MouseButtonEvent& lhs, const SDL_MouseButtonEvent& rhs) const;
	};
	//Mouse callbacks
	std::unordered_map<SDL_MouseButtonEvent, std::function<void(const SDL_Event&)>, SDL_MouseButtonEventHash, SDL_MouseButtonEventEqual> mouseButtonDownCallbacks;
	std::unordered_map<SDL_MouseButtonEvent, std::function<void(const SDL_Event&)>, SDL_MouseButtonEventHash, SDL_MouseButtonEventEqual> mouseButtonUpCallbacks;
	std::vector<std::function<void(const SDL_Event&)>> mouseMotionCallbacks;



	//turn into to a singleton
	// Deleted copy constructor and assignment operator
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;
	InputHandler();

protected:
	friend class Core;

	void InjectHandler(Ref<EventHandler>& sdlEventHandler);



	void Start();

	void KeyDown(const SDL_Event& e);

	void KeyUp(const SDL_Event& e);

	void KeyHeld(const SDL_Event& e);

	void MouseButtonDown(const SDL_Event& e);

	void MouseButtonUp(const SDL_Event& e);

	void MouseMotion(const SDL_Event& e);

	static void GetSDLMousePosition(int& x, int& y)
	{
		SDL_GetMouseState(&x, &y);
	};

	glm::vec2 GetMousePosition() const
	{
		return mousePosition;
	};

	glm::vec2 GetMouseDelta() const
	{
		return mousePosition - prevMousePosition;
	};

	void KeyHoldChecker();

public:
	// Public method to access the singleton instance
	static InputHandler& Instance();

	bool KeyHeld(SDL_Keycode key);


	void RegisterKeyPressCallback(SDL_Keycode key, const std::function<void(const SDL_Event&)> callback);

	void RegisterKeyHoldCallback(SDL_Keycode key, std::function<void()> callback);

	void RegisterKeyReleaseCallback(SDL_Keycode key, std::function<void(const SDL_Event&)> callback);
};
