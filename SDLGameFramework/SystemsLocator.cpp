#include "SystemsLocator.h"
#include "AudioSystem.h"
#include "Timer.h"
#include "EventHandler.h"
#include "Window.h"
#include "Scene.h"
void SystemsLocator::ProvideWindow(Window* win)
{
	window = win;
}

void SystemsLocator::ProvideTimer(Timer* tim)
{
	timer = tim;
}

void SystemsLocator::ProvideEventHandler(EventHandler* event)
{
	eventHandler = event;
}

void SystemsLocator::ProvideCurrentScene(Scene* scene)
{
	currentScene = scene;
}

void SystemsLocator::ProvideAudioSystem(AudioSystem* audio)
{
	audioSystem = audio;
}

void SystemsLocator::ProvideRenderer(Renderer* renderer_)
{
	renderer = renderer_;
}

void SystemsLocator::ProvideSceneManager(SceneManager* sceneManager_)
{
	sceneManager = sceneManager_;
}

SceneManager* SystemsLocator::GetSceneManager()
{
	return sceneManager;
}

Window* SystemsLocator::GetWindow()
{
	return window;
}

Timer* SystemsLocator::GetTimer()
{
	return timer;
}

EventHandler* SystemsLocator::GetEventHandler()
{
	return eventHandler;
}

Scene* SystemsLocator::GetCurrentScene()
{
	return currentScene;
}

AudioSystem* SystemsLocator::GetAudioSystem()
{
	return audioSystem;
}

Renderer* SystemsLocator::GetRenderer()
{
	return renderer;
}
