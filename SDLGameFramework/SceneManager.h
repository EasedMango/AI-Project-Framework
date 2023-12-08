#pragma once
#include "Scene.h"
#include "TestScene.h"
class SceneManager
{
private:
	ScopePtr<Scene> currentScene;

	Renderer* renderer;



	bool gui = true;
	bool pause = false;
protected:
	friend class Core;





	void OnCreate() const;

	void Update(const float& deltaTime) const;

	void Render() const;

	void OnDestroy() const;

public:
	SceneManager(Renderer* renderer);
	~SceneManager();
	void Pause();

	template<typename T>
	void SetScene()
	{
		if (currentScene) {
			currentScene->OnDestroy();
			currentScene = ScopePtr<T>(new T(renderer));
			currentScene->OnCreate();
		}
		currentScene = ScopePtr<T>(new T(renderer));
	}
};
