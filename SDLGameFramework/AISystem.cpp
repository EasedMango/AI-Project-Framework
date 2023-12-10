#include "AISystem.h"

#include "AI.h"
#include "Body.h"
#include "Player.h"
#include "TileMap.h"
#include "Transform.h"

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


			auto&& stateMachine = ai.stateMachine;
			stateMachine.Check();
			SteeringOutput temp;
 			for(auto& [type, info, weight] : stateMachine.GetCurrentBehaviors())
			{
				switch (type)
				{
				case AIBehaviors::BehaviorType::Seek:
				//	printf("Seeking\n");
					ai.steering += AIBehaviors::Seek(body, transform, targetTransform, std::get<AIBehaviors::SeekInfo>(info)) *= weight;
					break;
				case AIBehaviors::BehaviorType::Arrive:
					ai.steering += weight * AIBehaviors::Arrive(body, transform, targetTransform, std::get<AIBehaviors::ArriveInfo>(info));
					break;
				case AIBehaviors::BehaviorType::Chase:
					ai.steering += weight * AIBehaviors::Chase(body, transform, targetTransform, std::get<AIBehaviors::ChaseInfo>(info));
					break;
				case AIBehaviors::BehaviorType::Wander:
					//printf("Wandering\n");
					ai.steering += AIBehaviors::Wander(body, transform, TileMap::Instance().GetGrid(), std::get<AIBehaviors::WanderInfo>(info)) *= weight;
					break;
				case AIBehaviors::BehaviorType::Patrol:
					ai.steering += weight * AIBehaviors::Patrol(body, transform, targetTransform, std::get<AIBehaviors::PatrolInfo>(info));
					break;
				case AIBehaviors::BehaviorType::Flee:
					ai.steering += weight * AIBehaviors::Flee(body, transform, targetTransform, std::get<AIBehaviors::FleeInfo>(info));
					break;
				case AIBehaviors::BehaviorType::AvoidCollision:
					//printf("Avoiding\n");
					ai.steering += weight * AIBehaviors::AvoidCollision(body, transform, registry, std::get<AIBehaviors::AvoidCollisionInfo>(info));
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
