#include "AISystem.h"

#include "AI.h"
#include "Player.h"
#include "TileMap.h"

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
			for (const auto& [behavior, weight] : ai.behaviors)
			{
				

				switch (behavior)
				{
				case AIBehaviors::BehaviorType::Seek:
					ai.steering += weight* AIBehaviors::Seek(body, transform, targetTransform, registry.GetComponent<SeekInfo>(entity));
					break;
				case AIBehaviors::BehaviorType::Arrive:
					ai.steering += AIBehaviors::Arrive(body, transform, targetTransform, registry.GetComponent<ArriveInfo>(entity));
					break;
				case AIBehaviors::BehaviorType::Chase:
					ai.steering += AIBehaviors::Chase(body, transform, targetTransform, registry.GetComponent<ChaseInfo>(entity));
					break;
				case AIBehaviors::BehaviorType::Wander:
					ai.steering += AIBehaviors::Wander(body, transform, TileMap::Instance().GetGrid(), registry.GetComponent<WanderInfo>(entity));
					break;
				case AIBehaviors::BehaviorType::Patrol:
					ai.steering += AIBehaviors::Patrol(body, transform, targetTransform, registry.GetComponent<PatrolInfo>(entity));
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
}

void AISystem::OnDestroy(Registry& registry)
{
}
