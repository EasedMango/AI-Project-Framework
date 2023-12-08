#pragma once
#include "Common.h"


class SceneManager;
class AudioSystem;
class Renderer;
#include "EventHandler.h"
class Timer;
class Window;
#include "SceneManager.h"
class Core
{
private:
	Ref<Window> window;
	Ref<Timer> timer;
	Ref<EventHandler> eventHandler;
	Ref<Renderer> renderer;
	Ref<AudioSystem> audioSystem;
	Ref<SceneManager> sceneManager;


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
		sceneManager->SetScene<T>();
	}


	bool Initialize(const char* name_, int width_, int height_);




	bool Run();
};

