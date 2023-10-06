#include "Core.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Debug.h"
#include "Common.h"
#include "Window.h"
#include <memory>
#include "EventHandler.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Scene.h"
#include "Timer.h"
#include "ECS/ECS.h"
Core::Core()
{
	Debug::Info("Starting Core");
}




bool Core::Initialize(const char* name_, int width_, int height_)
{
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
	// Create Window
	window = std::make_unique<Window>();
	if (!window->OnCreate(name_, width_, height_))
	{
		Debug::FatalError("Failed to create window");
		return false;
	}

	// Create Renderer
	Renderer::GetInstance().Init(window);

	// Create Event Handler
	eventHandler = std::make_shared<EventHandler>();

	// Inject Event Handler Ref into Input Handler singleton
	InputHandler::Instance().InjectHandler(eventHandler);
	InputHandler::Instance().Start();
	timer = std::make_shared<Timer>();



	eventHandler->RegisterCallback(SDL_QUIT, [&](const SDL_Event&)
		{
			isRunning = false;
		});

	InputHandler::Instance().RegisterKeyPressCallback(SDLK_ESCAPE, [&](const SDL_Event&) {
		isRunning = false;
		});
	InputHandler::Instance().RegisterKeyPressCallback(SDLK_p, [&](const SDL_Event&) {
		pause = !pause;
		});
	return true;
}



bool Core::Run()
{
	isRunning = true;
	pause = false;
	fps = 60;
	auto& ecs = ECS::GetInstance();
	currentScene->OnCreate(ecs, window);
	timer->Start();
	while (isRunning) {
		timer->UpdateFrameTicks();
		eventHandler->HandleEvents();
		InputHandler::Instance().KeyHoldChecker();
		if (!pause)
			currentScene->Update(timer->GetDeltaTime(), ecs);
		currentScene->Render(ecs);
		SDL_GL_SwapWindow(window->GetWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
	currentScene->OnDestroy(ecs);
	return false;
}
