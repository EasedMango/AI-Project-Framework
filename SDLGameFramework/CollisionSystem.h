#pragma once
#include "ECS/ISystem.h"

class CollisionSystem : public ISystem
{

private:
	static void CheckCollision(Registry& registry, const ID& entity1, const ID& entity2, const float& deltaTime);
	
public:
	void OnCreate(Registry& registry) override;
	void Update(const float& deltaTime, Registry& registry) override;
	void OnDestroy(Registry& registry) override;
};
