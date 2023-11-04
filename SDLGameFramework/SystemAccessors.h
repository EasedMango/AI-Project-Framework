#pragma once
#include "Common.h"

class GUI;
class AudioSystem;
class Scene;
class EventHandler;
class Timer;
class Window;

class SystemAccessors {
private:
    inline static Ref<Window> window=nullptr;
    inline static Ref<Timer> timer = nullptr;
    inline static Ref<EventHandler> eventHandler = nullptr;
    inline static Ref<Scene> currentScene = nullptr;
    inline static Ref<AudioSystem> audioSystem = nullptr;

    protected:
    friend class Core;
    // Setters to inject dependencies
    static void ProvideWindow(const Ref<Window>& win);
    static void ProvideTimer(const Ref<Timer>& tim);
    static void ProvideEventHandler(const Ref<EventHandler>& event);
    static void ProvideCurrentScene(const Ref<Scene>& scene);
    static void ProvideAudioSystem(const Ref<AudioSystem>& audio);

public:

    // Getters to access dependencies
    static Ref<Window> GetWindow();
    static Ref<Timer> GetTimer();
    static Ref<EventHandler> GetEventHandler();
    static Ref<Scene> GetCurrentScene();
    static Ref<AudioSystem> GetAudioSystem();
};
