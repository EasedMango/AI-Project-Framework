#pragma once
#include "Renderer.h"
#include "ECS/ECS.h"
class Renderer;
class Camera;

class Scene
{
protected:
	Ref<Camera> mainCamera;//camera is non owning to a specific object
	Renderer* renderer;

	ScopePtr<ECS> ecs;//

public:
	virtual ~Scene() {}
	Scene(Renderer* renderer) : renderer(renderer)
	{
		mainCamera = renderer->CreateCamera();
		renderer->GiveCamera(mainCamera);
	}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual void Render() const = 0;



};
