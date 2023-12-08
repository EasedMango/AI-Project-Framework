#include "Core.h"

#include <SDL.h>
#include <SDL_image.h>

#include "AudioSystem.h"
#include "Debug.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "SystemsLocator.h"
#include "Timer.h"
#include "Timing.h"
#include "Window.h"


class TestScene;

Core::Core(): fps(0), isRunning(false)
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
		window = std::make_shared<Window>();
		if (!window->OnCreate(name_, width_, height_))
		{
			Debug::FatalError("Failed to create window");
			return false;
		}
	}
	SystemsLocator::ProvideWindow(window.get());

	// Create Renderer
	{
		TIMING("Create Renderer");
		renderer = Renderer::Create(window.get());
	}
	SystemsLocator::ProvideRenderer(renderer.get());
	{
		TIMING("Create Audio System");
		audioSystem = std::make_shared<AudioSystem>();

		if (!audioSystem->Initialize())
		{
			Debug::FatalError("Failed to initialize audio system");
			return false;
		}
	}
	SystemsLocator::ProvideAudioSystem(audioSystem.get());


	// Create Event Handler
	{
		TIMING("Create Event Handler and start thread");
		eventHandler = std::make_shared<EventHandler>();
		// Inject Event Handler Ref into Input Handler singleton
		InputHandler::Instance().InjectHandler(eventHandler.get());
		InputHandler::Instance().Start();
	}
	SystemsLocator::ProvideEventHandler(eventHandler.get());

	{
		TIMING("Create Timer");
		timer = std::make_shared<Timer>();
	}
	SystemsLocator::ProvideTimer(timer.get());


	{
		TIMING("Create SceneManager");
		sceneManager = std::make_shared<SceneManager>(renderer.get());
	}

	SystemsLocator::ProvideSceneManager(sceneManager.get());

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
	InputHandler::Instance().RegisterKeyPressCallback(SDLK_0, [](const SDL_Event& event)
		{
			SystemsLocator::GetSceneManager()->SetScene<TestScene>();
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
			sceneManager->OnCreate();
	}
	

	timer->Start();
	while (isRunning) {
	//	TIMING("Run");
		timer->UpdateFrameTicks();
		eventHandler->HandleEvents();
		InputHandler::Instance().KeyHoldChecker();
		float dt = timer->GetDeltaTime();
		if (!pause)
			sceneManager->Update(dt);
		
		sceneManager->Render();
		SDL_GL_SwapWindow(window->GetWindow());
		SDL_Delay(timer->GetSleepTime(fps));
		//MemoryManager::CountAllocations();
	}
	//cleanup sdl


	audioSystem->CleanUp();
	sceneManager->OnDestroy();
	SDL_Quit();
	return false;
}
