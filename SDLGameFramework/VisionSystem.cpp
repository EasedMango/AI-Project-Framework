#include "VisionSystem.h"

#include <map>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "AI.h"
#include "Collider.h"
#include "Player.h"
#include "SpatialHash.h"
#include "Vision.h"
#include "Transform.h"
CollisionResult GetClosestRayCollision(Ray ray, ID entityOrigin, Transform originTransform, Registry& registry, float radius) {
	auto& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Exact<SpatialHash>().Find()[0]);
	CollisionResult closestResult;
	closestResult = false;  // Assuming CollisionResult has a boolean 'intersection'
	float closestDistance = FLT_MAX;

	for (auto& entity : spatialHash.query(originTransform, radius)) {
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

		if (result() && result.overlap < closestDistance && result.overlap >= 0.f && result.overlap <= ray.distance) {
			closestDistance = result.overlap;
			closestResult = result;
		}
	}

	return closestResult;
}


void VisionSystem::OnCreate(Registry& registry)
{}
void VisionSystem::Update(const float& deltaTime, Registry& registry) {
    auto query = registry.CreateQuery().Include<Vision>();
    auto&& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Include<SpatialHash>().Find()[0]);

    // Assuming player ID is 1 for simplicity. Replace with actual player ID retrieval logic if needed.
    auto&& player = 1;
    auto&& playerTransform = registry.GetComponent<Transform>(player);

    // Iterate over entities with the Vision component
    for (const auto& entity : query.Find()) {
        auto&& vision = registry.GetComponent<Vision>(entity);
        const auto& transform = registry.GetComponent<Transform>(entity);

        // Cast a ray from the entity to the player
        Ray rayToPlayer;
        rayToPlayer.origin = transform.pos;
        rayToPlayer.direction = glm::normalize(playerTransform.pos - transform.pos);
        rayToPlayer.distance = glm::length(playerTransform.pos - transform.pos);

        // Check if there is any obstacle between the entity and the player
        auto collisionResult = GetClosestRayCollision(rayToPlayer, entity, transform, registry, vision.radius);

        // Determine if the player is visible
        if (!collisionResult() || collisionResult.overlap >= rayToPlayer.distance) {
            // Player is visible to this entity
            vision.targetFound = true;
            vision.timeSinceLastSeen = 0.f;

            // Update AI state if applicable
            if (registry.HasComponent<AI>(entity)) {
                auto&& aiComponent = registry.GetComponent<AI>(entity);
                if (!aiComponent.stateMachine.GetBool("Visible")) {
                    printf("Player is visible to AI\n");
                    aiComponent.stateMachine.SetBool("Visible", true);
                }
            }
        } else {
            // Player is not visible to this entity
            vision.timeSinceLastSeen += deltaTime;
            if (vision.timeSinceLastSeen > vision.maxTimeSinceLastSeen) {
                vision.targetFound = false;
                vision.timeSinceLastSeen = 0.f;
                if (registry.HasComponent<AI>(entity)) {
                    printf("Player is not visible to AI\n");
                    registry.GetComponent<AI>(entity).stateMachine.SetBool("Visible", false);
                }
            }
        }
    }
}







void VisionSystem::OnDestroy(Registry& registry)
{}
