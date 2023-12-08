#pragma once
#include "Renderer.h"
#include "Scene.h"
class TestScene :
    public Scene
{
private:
    bool gui=true;
public:
    TestScene(Renderer* renderer) : Scene(renderer) {}
    bool OnCreate() override;
    void OnDestroy() override;
    void Update(const float& deltaTime) override;
    void Render() const override;

};

