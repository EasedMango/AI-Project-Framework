#include "AISystem.h"

#include "AI.h"
#include "Player.h"

void AISystem::OnCreate(Registry& registry)
{
}

void AISystem::Update(const float& deltaTime, Registry& registry)
{
	static auto target = registry.CreateQuery().Any<Player>().Find()[0];

	for (auto que = registry.CreateQuery().Any<AI>(); const auto & entity : que.Find()) {
		{

			auto& ai = registry.GetComponent<AI>(entity);
			auto& body = registry.GetComponent<Body>(entity);
			auto& transform = registry.GetComponent<Transform>(entity);
			auto& targetTransform = registry.GetComponent<Transform>(target);
			switch (ai.currentBehavior)
			{
			case AIBehaviors::BehaviorType::Seek:
				ai.steering += AIBehaviors::Seek(body, transform, targetTransform, registry.GetComponent<SeekInfo>(entity));
				break;
			case AIBehaviors::BehaviorType::Arrive:
				ai.steering += AIBehaviors::Arrive(body, transform, targetTransform, registry.GetComponent<ArriveInfo>(entity));
				break;
			case AIBehaviors::BehaviorType::Flee:
				ai.steering += AIBehaviors::Flee(body, transform, targetTransform, registry.GetComponent<FleeInfo>(entity));
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
