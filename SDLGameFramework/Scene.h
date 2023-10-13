#pragma once
#include "Renderer.h"
#include "ECS/ECS.h"
class Renderer;
class Camera;

class Scene
{
protected:
	Ref<Camera> camera;
	 Renderer& renderer;
public:
	virtual ~Scene() {}
	Scene(Renderer& renderer) : renderer(renderer)
	{
		
		camera = renderer.CreateCamera();
		renderer.GiveCamera(camera);

	}
	virtual bool OnCreate(ECS& ecs) = 0;
	virtual void OnDestroy(ECS& ecs) = 0;
	virtual void Update(const float& deltaTime, ECS& ecs) = 0;
	virtual void Render(  Registry& ecs) const = 0;



};
