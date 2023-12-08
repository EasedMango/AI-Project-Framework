#include "SceneManager.h"

SceneManager::SceneManager(Renderer* renderer): renderer(renderer)
{}

SceneManager::~SceneManager()
= default;

void SceneManager::OnCreate() const
{
	currentScene->OnCreate();
}

void SceneManager::Update(const float& deltaTime) const
{
	if (!pause)
		currentScene->Update( deltaTime);
}

void SceneManager::Render() const
{
	currentScene->Render();
}

void SceneManager::OnDestroy() const
{
	currentScene->OnDestroy();
}

void SceneManager::Pause()
{
	pause = !pause;
}


