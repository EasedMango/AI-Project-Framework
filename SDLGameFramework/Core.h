#pragma once
#include "Common.h"

class AudioSystem;
class Window;
class Scene;
class Timer;
class EventHandler;
#include "Scene.h"
#include "TestScene.h"
#include "GUI.h"



class Core
{
private:
	Ref<Window> window;
	Ref<Timer> timer;
	Ref<EventHandler> eventHandler;
	Ref<Scene> currentScene;
	Ref<Renderer> renderer;
	ScopePtr<ECS> ecs;
	Ref<AudioSystem> audioSystem;


	unsigned int fps;
	bool isRunning;
	inline static bool pause = false;
public:
	Core();


	static void Pause()
	{
				pause = !pause;
	}
	template<typename T>
	void BuildNewScene()
	{
		currentScene = std::make_unique<T>(renderer);
	}


	bool Initialize(const char* name_, int width_, int height_);




	bool Run();
};

