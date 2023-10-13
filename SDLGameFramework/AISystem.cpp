#include "AISystem.h"

#include "AI.h"
#include "Player.h"

void AISystem::OnCreate(Registry& registry)
{
}

void AISystem::Update(const float& deltaTime, Registry& registry)
{
	ID target;
	auto targets= registry.CreateQuery().Any<Player>();
	for(auto& entity: targets.Find())
	{
		target= entity;
	}
	for (auto que = registry.CreateQuery().Any<AI>(); const auto & entity : que.Find()) {
		{

			auto& ai = registry.GetComponent<AI>(entity);
			auto& body = registry.GetComponent<Body>(entity);
			auto& transform = registry.GetComponent<Transform>(entity);
			auto& targetTransform = registry.GetComponent<Transform>(target);
			switch (ai.currentBehavior)
			{
			case AIBehaviors::BehaviorType::Seek:
				AIBehaviors::Seek(body, transform, targetTransform, ai.seek);
				break;
			case AIBehaviors::BehaviorType::Arrive:
				AIBehaviors::Arrive(body, transform, targetTransform, ai.arrive);
				break;
			default:
				break;
			}

		}
	}
}

void AISystem::OnDestroy(Registry& registry)
{
}
