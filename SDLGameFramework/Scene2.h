#pragma once
#include "Scene.h"

class Scene2 :
	public Scene
{
private:
	bool gui = true;
public:
	Scene2(Renderer* renderer) : Scene(renderer) {}
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float& deltaTime) override;
	void Render() const override;
};
