#pragma once
#include "Common.h"

class SceneManager;
class Renderer;
class GUI;
class AudioSystem;
class Scene;
#include "InputHandler.h"
#include "Timer.h"
class Window;

class SystemsLocator {
private:
    inline static Window* window=nullptr;
    inline static Timer* timer = nullptr;
    inline static EventHandler* eventHandler = nullptr;
    inline static Scene* currentScene = nullptr;
    inline static AudioSystem* audioSystem = nullptr;
    inline static Renderer* renderer = nullptr;
    inline static SceneManager* sceneManager = nullptr;
    protected:
    friend class Core;
    // Setters to inject system dependencies
    static void ProvideWindow( Window*  win);
    static void ProvideTimer( Timer*  tim);
    static void ProvideEventHandler( EventHandler*  event);
    static void ProvideCurrentScene( Scene*  scene);
    static void ProvideAudioSystem( AudioSystem*  audio);
    static void ProvideRenderer( Renderer*  renderer_);
    static void ProvideSceneManager( SceneManager*  sceneManager_);
public:

    // Getters to access dependencies
    static Window* GetWindow();
    static Timer* GetTimer();
    static EventHandler* GetEventHandler();
    static Scene* GetCurrentScene();
    static AudioSystem* GetAudioSystem();
    static Renderer* GetRenderer();
    static SceneManager* GetSceneManager();
};
