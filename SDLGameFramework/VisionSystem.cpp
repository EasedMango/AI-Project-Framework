#include "VisionSystem.h"

#include <map>
#include <glm/trigonometric.hpp>
#include <glm/detail/func_geometric.inl>

#include "Collider.h"
#include "Player.h"
#include "SpatialHash.h"
#include "Transform.h"
#include "Vision.h"

void VisionSystem::OnCreate(Registry& registry)
{}

void VisionSystem::Update(const float& deltaTime, Registry& registry)
{
	auto&& que = registry.CreateQuery().Include<Vision>();
	auto&& spatialHash = registry.GetComponent<SpatialHash>(registry.CreateQuery().Include<SpatialHash>().Find()[0]);

	auto&& player = registry.CreateQuery().Include<Player>().Find()[0];

	auto&& playerTransform = registry.GetComponent<Transform>(player);



	std::map<float, ID> entities;

	for (const auto& entity : que.Find())
	{
		const auto& vision = registry.GetComponent<Vision>(entity);
		const auto& transform = registry.GetComponent<Transform>(entity);
		glm::vec3 direction = glm::normalize(playerTransform.pos - transform.pos);

		for (const auto& id : spatialHash.query(transform, vision.radius))
		{
			if (id == entity)
				continue;
			if (const auto& collider = registry.GetComponent<Collider>(id); collider.shape == ColliderShape::BoxCollider)
			{
				auto obb = registry.GetComponent<BoxCollider>(id);
				auto obbTransform = registry.GetComponent<Transform>(id);
				float tMin = 0;
				if (RayIntersectsOBB({ transform.pos, direction }, obbTransform.pos, { obb.halfWidth,obb.halfHeight }, glm::mat2(glm::cos(obbTransform.rot), glm::sin(obbTransform.rot), -glm::sin(obbTransform.rot), glm::cos(obbTransform.rot)), tMin))
				{
					entities[tMin] = id;
				}
			}
			else if (const auto& collider = registry.GetComponent<Collider>(id); collider.shape == ColliderShape::CircleCollider)
			{
				const auto circle = registry.GetComponent<CircleCollider>(id);
				auto circleTransform = registry.GetComponent<Transform>(id);
				float tMin = 0;
				if (RayIntersectsCircle({ transform.pos, direction }, circleTransform.pos, circle.radius, tMin))
				{
					entities[tMin] = id;
				}
			}
		}
		if (!entities.empty() && entities.begin()->second == player)
		{
			registry.GetComponent<Vision>(entity).target = entities.begin()->second;
		}
		else
		{
			registry.GetComponent<Vision>(entity).target = -1;
		}


	}





}

void VisionSystem::OnDestroy(Registry& registry)
{}
