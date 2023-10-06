#pragma once
class Camera;
class ECS;
class Scene
{
protected:
	Ref<Camera> camera;
public:
	virtual ~Scene() {}
	virtual bool OnCreate(ECS& ecs, Ref<Window>& window) = 0;
	virtual void OnDestroy(ECS& ecs) = 0;
	virtual void Update(const float deltaTime, ECS& ecs) = 0;
	virtual void Render(ECS& ecs) const = 0;



};
