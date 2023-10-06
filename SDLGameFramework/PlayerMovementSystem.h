#pragma once
#include <glm/vec2.hpp>

#include "ECS/ISystem.h"

class PlayerMovementSystem : public ISystem
{
private:
	glm::vec2 moveDir = {};
public:
	void OnCreate(Registry& registry) override;
	void Update(const float& deltaTime, Registry& registry) override;
	void OnDestroy(Registry& registry) override;
};
