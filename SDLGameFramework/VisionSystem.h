#pragma once
#include "ECS/ISystem.h"


class VisionSystem : public ISystem
{
private:


	public:
		void OnCreate(Registry& registry) override;;
		void Update(const float& deltaTime, Registry& registry) override;;
		void OnDestroy(Registry& registry) override;;



};