#pragma once
#include "Common.h"

class Window;
class Scene;
class Timer;
class EventHandler;
#include "Scene.h"
#include "TestScene.h"
class Core
{
private:
	Ref<Window> window;
	Ref<Timer> timer;
	Ref<EventHandler> eventHandler;
	Ref<Scene> currentScene;

	unsigned int fps;
	bool isRunning;
	bool pause;
public:
	Core();

	template<typename T>
	void BuildNewScene()
	{
		currentScene = std::make_unique<T>();
	}


	bool Initialize(const char* name_, int width_, int height_);
	template<typename T>
	void SetInitalScene();



	bool Run();
};

template <typename T>
void Core::SetInitalScene()
{
	BuildNewScene<T>();
}