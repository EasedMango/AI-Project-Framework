#pragma once
#include <functional>
#include <variant>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>


#include "Body.h"
#include "Common.h"
#include "Grid.h"
#include "SteeringOutput.h"
#include "Transform.h"


class Registry;

namespace AIBehaviors {

	enum class BehaviorType
	{
		Seek,
		Chase,
		Arrive,
		Patrol,
		Wander,
		Flee,
		AvoidCollision
	};

	struct SeekInfo
	{
		ID target;
		float maxAcceleration;
	};

	struct AvoidCollisionInfo
	{
		float distance;
	};

	struct ArriveInfo
	{
		ID target;
		float maxSpeed;
		float maxAcceleration;
		float targetRadius;
		float slowRadius;
		float timeToTarget;
	};

	struct ChaseInfo
	{
		ID target;
	};

	struct FleeInfo
	{
		ID target;
		float maxAcceleration;
	};


	struct WanderInfo
	{
		float range;
		glm::vec2 center;
		float speed;
		std::vector<Tile*> path;
		int currentPathIndex;

	};

	struct PatrolInfo
	{
		ID target;
		glm::vec3 patrolPointA;
		glm::vec3 patrolPointB;
		bool AtpointA;
	};
	using BehaviorInfo = std::variant<SeekInfo, ArriveInfo, ChaseInfo, FleeInfo, WanderInfo, PatrolInfo,AvoidCollisionInfo>;

	struct WeightedBehavior
	{
		AIBehaviors::BehaviorType behavior;
		float weight;
	};

	SteeringOutput AvoidCollision(const Body& characterBody, const Transform& characterTrans, Registry& registry, AvoidCollisionInfo& info);

	SteeringOutput Seek(const Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info);

	SteeringOutput Chase(Body& characterBody, const Transform& characterTrans, const Transform& target, ChaseInfo& info);

	SteeringOutput Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info);

	SteeringOutput Patrol(const Body& characterBody, const Transform& characterTrans, const Transform& target, PatrolInfo& info);

	SteeringOutput Wander(const Body& characterBody, const Transform& characterTrans, Grid& target, WanderInfo& info);


	SteeringOutput Flee(Body& characterBody, const Transform& characterTrans, const Transform& target, FleeInfo& info);

}
