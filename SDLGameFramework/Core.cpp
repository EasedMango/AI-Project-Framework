#include "Core.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Debug.h"
#include "Common.h"
#include "Window.h"
#include <memory>

#include "AudioSystem.h"
#include "EventHandler.h"
#include "InputHandler.h"
#include "MemoryManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Timer.h"
#include "Timing.h"
#include "ECS/ECS.h"
#include "SystemAccessors.h"
Core::Core(): fps(0), isRunning(false), pause(false)
{
	Debug::Info("Starting Core");
}


bool Core::Initialize(const char* name_, int width_, int height_)
{
	{
		TIMING("Initialize SDL");
		// Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			Debug::FatalError("Failed to initialize SDL");
			return false;
		}
		// Initialize SDL Image
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
			SDL_Quit();
			return false;  // Changed from 'return 1;' to 'return false;'
		}
	}
	// Create Window
	{
		TIMING("Create Window");
		window = std::make_unique<Window>();
		if (!window->OnCreate(name_, width_, height_))
		{
			Debug::FatalError("Failed to create window");
			return false;
		}
	}
	SystemAccessors::ProvideWindow(window);

	// Create Renderer
	{
		TIMING("Create Renderer");
		renderer = Renderer::Create(window);
	}
	{
		TIMING("Create Audio System");
		audioSystem = std::make_shared<AudioSystem>();

		if (!audioSystem->Initialize())
		{
			Debug::FatalError("Failed to initialize audio system");
			return false;
		}
	}
	SystemAccessors::ProvideAudioSystem(audioSystem);


	// Create Event Handler
	{
		TIMING("Create Event Handler and start thread");
		eventHandler = std::make_shared<EventHandler>();
		// Inject Event Handler Ref into Input Handler singleton
		InputHandler::Instance().InjectHandler(eventHandler);
		InputHandler::Instance().Start();
	}
	SystemAccessors::ProvideEventHandler(eventHandler);

	{
		TIMING("Create Timer");
		timer = std::make_shared<Timer>();
	}
	SystemAccessors::ProvideTimer(timer);
	{
		TIMING("Create ECS");
		ecs = ECS::Create();
	}
	eventHandler->RegisterCallback(SDL_QUIT, [&](const SDL_Event&)
		{
			isRunning = false;
		});

	InputHandler::Instance().RegisterKeyPressCallback(SDLK_ESCAPE, [&](const SDL_Event&) {
		isRunning = false;
		});
	InputHandler::Instance().RegisterKeyPressCallback(SDLK_SPACE, [&](const SDL_Event&) {
		auto soundPath = std::string("woosh.wav");
		audioSystem->PlaySound(soundPath);
		});
	InputHandler::Instance().RegisterKeyPressCallback(SDLK_p, [&](const SDL_Event&) {
		pause = !pause;
		std::cout << "Pause: " << std::boolalpha << pause << std::endl;
		});
	return true;
}



bool Core::Run()
{
	isRunning = true;
	pause = false;
	fps = 144;
	{
		TIMING("Create Scene");
			currentScene->OnCreate(*ecs);
	}
	SystemAccessors::ProvideCurrentScene(currentScene);

	timer->Start();
	while (isRunning) {
	//	TIMING("Run");
		timer->UpdateFrameTicks();
		eventHandler->HandleEvents();
		InputHandler::Instance().KeyHoldChecker();
		float dt = timer->GetDeltaTime();
		if (!pause)
			currentScene->Update(dt, *ecs);
		
		currentScene->Render(ecs->GetRegistry());
		SDL_GL_SwapWindow(window->GetWindow());
		SDL_Delay(timer->GetSleepTime(fps));
		//MemoryManager::CountAllocations();
	}
	//cleanup sdl


	audioSystem->CleanUp();
	currentScene->OnDestroy(*ecs);
	SDL_Quit();
	return false;
}
