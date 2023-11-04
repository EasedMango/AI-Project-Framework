#include "SystemAccessors.h"
#include "AudioSystem.h"
#include "Timer.h"
#include "EventHandler.h"
#include "Window.h"
#include "Scene.h"
void SystemAccessors::ProvideWindow(const Ref<Window>& win)
{ window = win; }

void SystemAccessors::ProvideTimer(const Ref<Timer>& tim)
{ timer = tim; }

void SystemAccessors::ProvideEventHandler(const Ref<EventHandler>& event)
{ eventHandler = event; }

void SystemAccessors::ProvideCurrentScene(const Ref<Scene>& scene)
{ currentScene = scene; }

void SystemAccessors::ProvideAudioSystem(const Ref<AudioSystem>& audio)
{ audioSystem = audio; }

Ref<Window> SystemAccessors::GetWindow()
{ return window; }

Ref<Timer> SystemAccessors::GetTimer()
{ return timer; }

Ref<EventHandler> SystemAccessors::GetEventHandler()
{ return eventHandler; }

Ref<Scene> SystemAccessors::GetCurrentScene()
{ return currentScene; }

Ref<AudioSystem> SystemAccessors::GetAudioSystem()
{ return audioSystem; }
