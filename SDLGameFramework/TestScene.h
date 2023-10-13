#pragma once
#include "Renderer.h"
#include "Scene.h"
class TestScene :
    public Scene
{
private:
    bool gui=true;
public:
    TestScene(const ScopePtr<Renderer>& renderer) : Scene(*renderer) {}
    bool OnCreate(ECS& ecs) override;
    void OnDestroy(ECS& ecs) override;
    void Update(const float& deltaTime, ECS& ecs) override;
    void Render( Registry& registry) const override;
};

