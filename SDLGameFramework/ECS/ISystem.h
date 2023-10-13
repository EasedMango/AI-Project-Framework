#pragma once
#include "Registry.h";
class ISystem {
public:
	virtual ~ISystem() = default;
	virtual void OnCreate(Registry& registry) = 0;
	virtual void Update(const float& deltaTime,Registry& registry) = 0;
	virtual void OnDestroy(Registry& registry) = 0;
};
