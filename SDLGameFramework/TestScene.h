#pragma once
#include "Renderer.h"
#include "Scene.h"
class TestScene :
    public Scene
{
private:

public:
    bool OnCreate(ECS& ecs, Ref<Window>& window) override;
    void OnDestroy(ECS& ecs) override;
    void Update(const float deltaTime, ECS& ecs) override;
    void Render(ECS& ecs) const override;
};

