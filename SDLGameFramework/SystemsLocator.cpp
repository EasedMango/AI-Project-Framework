#include "SystemsLocator.h"
#include "AudioSystem.h"
#include "Timer.h"
#include "EventHandler.h"
#include "Window.h"
#include "Scene.h"
void SystemsLocator::ProvideWindow(const Ref<Window>& win)
{ window = win; }

void SystemsLocator::ProvideTimer(const Ref<Timer>& tim)
{ timer = tim; }

void SystemsLocator::ProvideEventHandler(const Ref<EventHandler>& event)
{ eventHandler = event; }

void SystemsLocator::ProvideCurrentScene(const Ref<Scene>& scene)
{ currentScene = scene; }

void SystemsLocator::ProvideAudioSystem(const Ref<AudioSystem>& audio)
{ audioSystem = audio; }

void SystemsLocator::ProvideRenderer(const Ref<Renderer>& renderer_)
{
	renderer = renderer_;
}

Ref<Window> SystemsLocator::GetWindow()
{ return window; }

Ref<Timer> SystemsLocator::GetTimer()
{ return timer; }

Ref<EventHandler> SystemsLocator::GetEventHandler()
{ return eventHandler; }

Ref<Scene> SystemsLocator::GetCurrentScene()
{ return currentScene; }

Ref<AudioSystem> SystemsLocator::GetAudioSystem()
{ return audioSystem; }

Ref<Renderer> SystemsLocator::GetRenderer()
{ return renderer;
}
