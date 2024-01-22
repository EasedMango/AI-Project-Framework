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
			//printf("Entity: %i ",entity);
 			for(auto& [type, info, weight,OG] : stateMachine.GetCurrentBehaviors())
			{
			//	printf(" Behavior: %i ", type);
				switch (type)
				{
				case AIBehaviors::BehaviorType::Seek:
				//	printf("Seeking\n");
					ai.steering += AIBehaviors::Seek(body, transform, targetTransform, std::get<AIBehaviors::SeekInfo>(info)) *= weight;
					break;
				case AIBehaviors::BehaviorType::Arrive:
					ai.steering +=  AIBehaviors::Arrive(body, transform, targetTransform, std::get<AIBehaviors::ArriveInfo>(info))*= weight;
					break;
				case AIBehaviors::BehaviorType::Chase:
					ai.steering +=  AIBehaviors::Chase(body, transform, targetTransform, std::get<AIBehaviors::ChaseInfo>(info))*= weight;
					break;
				case AIBehaviors::BehaviorType::Wander:
					//printf("Wandering\n");
					ai.steering += AIBehaviors::Wander(body, transform, TileMap::Instance().GetGrid(), std::get<AIBehaviors::WanderInfo>(info)) *= weight;
					break;
				case AIBehaviors::BehaviorType::Patrol:
					ai.steering +=  AIBehaviors::Patrol(body, transform, std::get<AIBehaviors::PatrolInfo>(info), TileMap::Instance().GetGrid())*= weight;
					break;
				case AIBehaviors::BehaviorType::Flee:
					ai.steering +=  AIBehaviors::Flee(body, transform, targetTransform, std::get<AIBehaviors::FleeInfo>(info))*= weight;
					break;
				case AIBehaviors::BehaviorType::AvoidCollision:
				//	printf("Avoiding\n");
					ai.steering +=  AIBehaviors::AvoidCollision(entity, body, transform, registry, std::get<AIBehaviors::AvoidCollisionInfo>(info))*= weight;
					break;
				default:
					break;
				}
			}
			//printf("\n");
		}
	}
}

void AISystem::OnDestroy(Registry& registry)
{
}
