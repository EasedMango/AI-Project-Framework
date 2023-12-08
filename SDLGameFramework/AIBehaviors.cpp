
#include "AIBehaviors.h"

#include <random>

#include "SteeringOutput.h"
#include "AI.h"


SteeringOutput AIBehaviors::Seek(const Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	const glm::vec3 direction = target.pos - characterTrans.pos;

	if (const float distance = glm::length(direction); distance > 0) {
		float targetSpeed;

		// If the target is close enough, then slow down


		targetSpeed = characterBody.maxSpeed;


		glm::vec3 targetVelocity = glm::normalize(direction) * targetSpeed;

		// Accelerate towards the target
		steering.linear = targetVelocity - characterBody.vel;

		// Optionally limit the acceleration
		if (glm::length(steering.linear) > characterBody.maxAcceleration) {
			steering.linear = glm::normalize(steering.linear) * characterBody.maxAcceleration;
		}
	}
	else {
		steering.linear = glm::vec3(0.0f);
	}

	return steering;
}

SteeringOutput AIBehaviors::Chase(Body& characterBody, const Transform& characterTrans, const Transform& target, ChaseInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);

	// Work out current speed
	float targetSpeed = glm::length(characterBody.vel);

	// Decide on a reasonable prediction time
	float maxPrediction = 1.0f;
	float prediction;

	if (targetSpeed <= (distance / maxPrediction))
	{
		prediction = maxPrediction;
	}
	else
	{
		prediction = distance / targetSpeed;
	}

	//float targetSpeed;

	// If the target is close enough, then slow down

	targetSpeed = characterBody.maxSpeed;
	const glm::vec3 targetVelocity = glm::normalize(direction) * targetSpeed;

	// Put together the target for Chase
	Transform ChaseTarget = target;
	ChaseTarget.pos += targetVelocity * prediction;

	// Call the Seek function with the new target; 

	SeekInfo si = SeekInfo(1.0f);

	return Seek(characterBody, characterTrans, ChaseTarget, si);

}

SteeringOutput AIBehaviors::Arrive(const Body& characterBody, const Transform& characterTrans, const Transform& target, ArriveInfo& info)
{
	auto result = SteeringOutput();


	auto direction = target.pos - characterTrans.pos;

	auto distance = glm::length(direction);
	float targetSpeed;
	if (distance < info.targetRadius)
	{
		return result;
	}

	if (distance > info.slowRadius)
	{
		targetSpeed = characterBody.maxSpeed;
	}
	else
		targetSpeed = characterBody.maxSpeed * distance / info.slowRadius;

	auto targetVelocity = glm::normalize(direction) * targetSpeed;
	result.linear = (targetVelocity - characterBody.vel) / info.timeToTarget;
	if (glm::length(result.linear) > info.maxAcceleration)
	{
		result.linear = glm::normalize(result.linear) * info.maxAcceleration;
	}

	result.angular = 0;
	return result;
}

SteeringOutput AIBehaviors::Patrol(const Body& characterBody, const Transform& characterTrans, const Transform& target, PatrolInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);


	SeekInfo si = SeekInfo{ 0,1.0f };

	if (distance < 2) {

		return Seek(characterBody, characterTrans, target, si);
	}


	//sets patrol point
	info.patrolPointA = glm::vec3(-6.0f, 4.0f, 0.0f);
	info.patrolPointB = glm::vec3(6.0f, 4.0f, 0.0f);

	//sets the starting posistion to be one of the points
	glm::vec3 currentPosition;

	if (info.AtpointA == true) {
		steering.linear = info.patrolPointB - characterTrans.pos;
		currentPosition = info.patrolPointB;
	}

	if (info.AtpointA == false) {
		steering.linear = info.patrolPointA - characterTrans.pos;
		currentPosition = info.patrolPointA;
	}

	direction = currentPosition - characterTrans.pos;
	distance = glm::length(direction);

	if (distance <= 1.0f) {
		info.AtpointA = !info.AtpointA;
	}

	return steering;
}

SteeringOutput AIBehaviors::Wander(const Body& characterBody, const Transform& characterTrans, Grid& target,
	WanderInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;
	static std::random_device random;
	static std::mt19937 rng(random());
	static std::uniform_int_distribution<std::mt19937::result_type> dist(0, 24);

	if (info.path.empty())
	{
		const auto currentTile = target.GetTile(characterTrans.pos.x, characterTrans.pos.y);
		auto randomTile = target.GetTile(dist(rng), dist(rng));


		auto path = target.GetPath(currentTile, randomTile);

		//while (path.empty())
		//{
		//	randomTile = target.GetTile(dist(rng), dist(rng));
		//	path = target.GetPath(currentTile, randomTile);
		//}
		if(path.empty())
			return steering;

		info.path = path;
		info.currentPathIndex = 0;
	}
	auto targetTile = info.path[info.currentPathIndex];
	auto dirToNextTile = glm::vec3{ targetTile->x,targetTile->y,0 } - characterTrans.pos;
	auto distance = glm::length(dirToNextTile);
	if (distance < 0.5f)
	{
		info.currentPathIndex++;
		if (info.currentPathIndex >= info.path.size())
		{
			info.path.clear();
		}
	}

	Transform targetTrans = Transform{ glm::vec3{targetTile->x,targetTile->y,0},0 };
//	auto d = ArriveInfo{ info.speed,characterBody.maxAcceleration,0.1f,0.5f,0.1f };
	auto d = SeekInfo{ 0,1.f };
	return Seek(characterBody, characterTrans, targetTrans, d);
}


SteeringOutput AIBehaviors::Flee(Body& characterBody, const Transform& characterTrans, const Transform& target, FleeInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec3 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);

	if (distance > 0) {
		steering.linear = characterTrans.pos - target.pos;

		steering.linear = glm::normalize(steering.linear) * characterBody.maxAcceleration;
	}
	return steering;
}
