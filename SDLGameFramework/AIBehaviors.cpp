
#include "AIBehaviors.h"

#include <map>
#include <random>

#include "SteeringOutput.h"
#include "AI.h"
#include "Collider.h"
#include "SpatialHash.h"
#include "ECS/Registry.h"

//vec2 to float



CollisionResult GetClosestRayCollision(Ray ray, ID entityOrigin, Transform originTransform, Registry& registry) {
	auto& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Exact<SpatialHash>().Find()[0]);
	CollisionResult closestResult;
	closestResult = false;  // Assuming CollisionResult has a boolean 'intersection'
	float closestDistance = FLT_MAX;

	for (auto& entity : spatialHash.query(originTransform, 2)) {
		if (entity == entityOrigin) continue;

		auto& collider = registry.GetComponent<Collider>(entity);
		CollisionResult result;

		if (collider.shape == ColliderShape::BoxCollider) {
			auto obb = registry.GetComponent<BoxCollider>(entity);
			auto& obbTransform = registry.GetComponent<Transform>(entity);
			result = RayIntersectsOBB(ray, obbTransform.pos, { obb.halfWidth, obb.halfHeight }, glm::mat2(glm::cos(obbTransform.rot), glm::sin(obbTransform.rot), -glm::sin(obbTransform.rot), glm::cos(obbTransform.rot)));
		}
		else if (collider.shape == ColliderShape::CircleCollider) {
			const auto circle = registry.GetComponent<CircleCollider>(entity);
			auto& circleTransform = registry.GetComponent<Transform>(entity);
			result = RayIntersectsCircle(ray, circleTransform.pos, circle.radius);
		}

		if (result() && result.overlap < closestDistance && result.overlap >= 0.f) {
			closestDistance = result.overlap;
			closestResult = result;
		}
	}

	return closestResult;
}




SteeringOutput AIBehaviors::AvoidCollision(ID id, const Body& characterBody, const Transform& characterTrans, Registry& registry, AvoidCollisionInfo& info) {
	// Check if the character is effectively stationary
	if (glm::length(characterBody.vel) < FLT_EPSILON) {
		return SteeringOutput{}; // No movement, no need to steer
	}

	// Create a ray for collision detection
	glm::vec2 normalizedVelocity = glm::normalize(characterBody.vel);
	const Ray ray = { characterTrans.pos, normalizedVelocity, characterBody.maxAcceleration / 2.f };

	// Get the closest collision result
	const auto result = GetClosestRayCollision(ray, id, characterTrans, registry);

	SteeringOutput steering;
	steering.linear = glm::vec2(0.0f);

	// Check if a valid collision occurred and is within avoidance distance
	if (result.Valid() && result() && result.overlap < info.distance) {
		// Calculate avoidance direction, perpendicular to the wall normal
		glm::vec2 avoidanceDirection = glm::vec2(result.collisionNormal.y, -result.collisionNormal.x);

		// Apply steering force
		steering.linear = avoidanceDirection * (characterBody.maxAcceleration);
	}

	return steering;
}







SteeringOutput AIBehaviors::Seek(const Body& characterBody, const Transform& characterTrans, const Transform& target, SeekInfo& info)
{
	SteeringOutput steering ={};
	steering.angular = 0;
	const glm::vec2 direction = target.pos - characterTrans.pos;

	if (const float distance = glm::length(direction); distance > 0) {
		float targetSpeed;

		// If the target is close enough, then slow down


		targetSpeed = characterBody.maxSpeed;


		glm::vec2 targetVelocity = glm::normalize(direction) * targetSpeed;

		// Accelerate towards the target
		steering.linear = targetVelocity - characterBody.vel;
		// Optionally limit the acceleration
		if (glm::length(steering.linear) > characterBody.maxAcceleration) {
			steering.linear = glm::normalize(steering.linear) * characterBody.maxAcceleration;
		}
	}
	else {
		steering.linear = glm::vec2(0.0f);
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
	//print wander info



	if (info.path.empty())
	{
		const auto currentTile = target.GetTile(characterTrans.pos.x, characterTrans.pos.y);
		const auto randomTile = target.GetTile(dist(rng), dist(rng));
		//printf("current tile: %d, %d\n", currentTile->x, currentTile->y);

		const auto path = target.GetPath(currentTile, randomTile);

		//while (path.empty())
		//{
		//	randomTile = target.GetTile(dist(rng), dist(rng));
		//	path = target.GetPath(currentTile, randomTile);
		//}
		//printf("Pathing: %i",info.self);
		if (path.empty()) {
			//printf("no path found\n");
			return steering;
		}

		info.path = path;
		info.currentPathIndex = 0;
	}
	auto targetTile = info.path[info.currentPathIndex];
	const auto dirToNextTile = glm::vec2{ targetTile.x,targetTile.y } - characterTrans.pos;
	const auto distance = glm::length(dirToNextTile);
	if (distance < 0.5f)
	{

		info.currentPathIndex++;
		if (info.currentPathIndex >= info.path.size())
		{

			info.path.clear();
		}
		else
		{

		}
	}

	const Transform targetTrans = Transform{ glm::vec2{targetTile.x,targetTile.y},0 };
	//	auto d = ArriveInfo{ info.speed,characterBody.maxAcceleration,0.1f,0.5f,0.1f };
	auto d = SeekInfo{ 0,1.f };
	auto s = Seek(characterBody, characterTrans, targetTrans, d);


	return s;
}
SteeringOutput AIBehaviors::Chase(Body& characterBody, const Transform& characterTrans, const Transform& target, ChaseInfo& info)
{
	SteeringOutput steering;
	steering.angular = 0;

	glm::vec2 direction = target.pos - characterTrans.pos;
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
	const glm::vec2 targetVelocity = glm::normalize(direction) * targetSpeed;

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

	glm::vec2 direction = target.pos - characterTrans.pos;
	float distance = glm::length(direction);


	SeekInfo si = SeekInfo{ 0,1.0f };

	if (distance < 2) {

		return Seek(characterBody, characterTrans, target, si);
	}


	//sets patrol point
	info.patrolPointA = glm::vec2(-6.0f, 4.0f);
	info.patrolPointB = glm::vec2(6.0f, 4.0f);

	//sets the starting posistion to be one of the points
	glm::vec2 currentPosition;

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




SteeringOutput AIBehaviors::Flee(Body& characterBody, const Transform& characterTrans, const Transform& target, FleeInfo& info)
{
    SteeringOutput steering;
    steering.angular = 0;

    const glm::vec2 direction = characterTrans.pos - target.pos;

    // Normalize the direction and scale it by a fleeing distance
    const glm::vec2 fleeTargetPos = characterTrans.pos + glm::normalize(direction);

    // Create a Transform for the flee target
    const Transform fleeTarget = { fleeTargetPos, 0.0f };

    // Use SeekInfo with maxSpeed and maxAcceleration for fleeing
    SeekInfo seekInfo = { 0,1.f };

    // Use the Seek behavior to move towards the flee target
    return Seek(characterBody, characterTrans, fleeTarget, seekInfo);
}


