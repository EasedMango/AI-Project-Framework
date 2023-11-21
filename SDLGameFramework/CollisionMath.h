////#pragma once
//#include <array>
//#include <vector>
//#include <glm/geometric.hpp>
//#include <glm/mat2x2.hpp>
//#include <glm/vec2.hpp>
//#include <glm/ext/matrix_transform.hpp>
//
//#include "Collider.h"
//#include "Transform.h"
//namespace Collisions {
//	struct CollisionResult {
//		bool isColliding;         // Flag indicating if a collision occurred
//		glm::vec2 collisionNormal; // The normal of the collision
//		float overlap;            // The amount of overlap between the two OBBs
//		glm::vec2 mtv;            // The Minimum Translation Vector to resolve the collision
//
//		// Constructor for ease of use
//		CollisionResult(bool isColliding, glm::vec2 collisionNormal, float overlap, glm::vec2 mtv)
//			: isColliding(isColliding), collisionNormal(collisionNormal), overlap(overlap), mtv(mtv) {}
//
//		// Default constructor for no collision
//		CollisionResult() : isColliding(false), collisionNormal(), overlap(0.0f), mtv()
//		{
//		}
//	};
//
//	inline std::array<glm::vec2, 4> calculateOBBCorners(const Transform& transform, const BoxCollider& collider)
//	{
//		const auto rot = glm::radians(transform.rot +collider.rotation);
//		const auto pos = pos;
//		const auto rotCos = glm::cos(rot);
//		const auto rotSin = glm::sin(rot);
//		/*if (rot >= FLT_EPSILON && rot <=)*/
//		const auto rotationMatrix =
//			glm::mat2x2(rotCos, rotSin,
//					   -rotSin, rotCos);
//
//
//		const std::array<glm::vec2, 4> vertices = {
//			pos + rotationMatrix * glm::vec2(collider.halfWidth, collider.halfHeight)  ,
//			pos + rotationMatrix * glm::vec2(-collider.halfWidth, collider.halfHeight),
//			pos + rotationMatrix * glm::vec2(-collider.halfWidth, -collider.halfHeight),
//			pos + rotationMatrix * glm::vec2(collider.halfWidth, -collider.halfHeight)
//		};
//
//		return vertices;
//	};
//
//	inline std::pair<float, float> ProjectOntoAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& axis)
//	{
//		float min = std::numeric_limits<float>::max();
//		float max = std::numeric_limits<float>::min();
//		for (const auto& corner : corners) {
//			const float projection = glm::dot(corner, axis);
//			min = std::min(min, projection);
//			max = std::max(max, projection);
//		}
//		return std::make_pair(min, max);
//	};
//
//	inline bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis, float& overlapOut)
//	{
//		const auto [min1, max1] = ProjectOntoAxis(corners1, axis);
//		const auto [min2, max2] = ProjectOntoAxis(corners2, axis);
//		const bool isOverlapping = (min2 <= max1) && (min1 <= max2);
//		if (isOverlapping) {
//			const float overlap1 = max1 - min2;
//			const float overlap2 = max2 - min1;
//			overlapOut = std::min(overlap1, overlap2);
//		}
//		return isOverlapping;
//	};
//	inline glm::vec2 CalculateMTV(const glm::vec2& axis, float overlap, const glm::vec2& relativePosition)
//	{
//		glm::vec2 mtv = axis * overlap;
//		// Ensure the MTV points from OBB2 to OBB1
//		if (glm::dot(mtv, relativePosition) < 0) {
//			mtv = -mtv;
//		}
//		return mtv;
//	};
//
//	inline CollisionResult CheckOBBOBBCollision(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2)
//	{
//		CollisionResult result;
//		result.overlap = std::numeric_limits<float>::infinity();
//		std::array<glm::vec2, 4> axes = {
//			glm::normalize(corners1[0] - corners1[1]),
//			glm::normalize(corners1[0] - corners1[3]),
//			glm::normalize(corners2[1] - corners2[0]),
//			glm::normalize(corners2[3] - corners2[0])
//		};
//
//		const glm::vec2 relativePosition = corners2[0] - corners1[0]; // Vector from OBB1 to OBB2
//		for (auto& axis : axes) {
//			const float length = glm::length(axis);
//			if (length > std::numeric_limits<float>::epsilon()) {
//				axis = glm::normalize(axis);
//			}
//			else {
//				continue;
//			}
//			float overlap;
//			if (!CheckOverlapOnAxis(corners1, corners2, axis, overlap)) {
//				return {}; // No collision
//			}
//			if (overlap < result.overlap) {
//				result.isColliding = true;
//				result.overlap = overlap;
//				result.collisionNormal = axis;
//				result.mtv = CalculateMTV(axis, overlap, relativePosition);
//			}
//		}
//
//		return result;
//	}
//	CollisionResult CheckCircleCircleCollision(const CircleCollider& circle1, const Transform& transform1, const CircleCollider& circle2, const Transform& transform2) {
//		const glm::vec2 distanceVec = (glm::vec2(transform1.pos)+ circle2.offset) - (glm::vec2(transform2.pos)+ circle1.offset);
//		const float distance = glm::length(distanceVec);
//		const float radiusSum = circle1.radius + circle2.radius;
//
//		if (distance < radiusSum) {
//			float overlap = radiusSum - distance;
//			glm::vec2 collisionNormal = glm::normalize(distanceVec);
//			glm::vec2 mtv = collisionNormal * overlap;
//			return {true, collisionNormal, overlap, mtv};
//		}
//
//		return {}; // No collision
//	}
//	CollisionResult CheckCircleOBBCollision(const CircleCollider& circle1, const Transform& transform1, const BoxCollider& boxCollider, const Transform& transform2) {
//		// Calculate world space position of the circle center
//		glm::vec2 circleCenter = glm::vec2(transform1.pos) + circle1.offset;
//
//		// Transform the circle center to the OBB's local space
//		glm::mat2 rotationMatrix = glm::mat2(
//			glm::cos(glm::radians(-transform2.rot)), glm::sin(glm::radians(-transform2.rot)),
//			-glm::sin(glm::radians(-transform2.rot)), glm::cos(glm::radians(-transform2.rot))
//		);
//		glm::vec2 localCircleCenter = rotationMatrix * (circleCenter - glm::vec2(transform2.pos));
//
//
//		glm::vec2 halfExtents = { boxCollider.halfWidth, boxCollider.halfHeight };
//		// Find the closest point on the OBB to the local circle center
//		glm::vec2 closestPoint = glm::clamp(localCircleCenter, -halfExtents, halfExtents);
//
//		// Transform the closest point back to world space
//		glm::vec2 worldClosestPoint = glm::inverse(rotationMatrix) * closestPoint + glm::vec2(transform2.pos);
//
//		// Calculate the vector from the circle's center to the closest point and its magnitude
//		glm::vec2 toCircle = circleCenter - worldClosestPoint;
//		float distance = glm::length(toCircle);
//
//		// Check for collision
//		if (distance < circle1.radius) {
//			float overlap = circle1.radius - distance;
//			glm::vec2 collisionNormal = glm::normalize(toCircle);
//			glm::vec2 mtv = collisionNormal * overlap;
//			return CollisionResult(true, collisionNormal, overlap, mtv);
//		}
//
//		return CollisionResult(); // No collision
//	}
//
//
//
//}
////
////
////	inline auto calculateOBBCorners = [](const Transform& transform, const BoxCollider& collider) {
////		const auto rotationMatrix =
////			glm::mat2x2(glm::cos(glm::radians(collider.rotation + transform.rot)), glm::sin(glm::radians(collider.rotation + transform.rot)),
////				-glm::sin(glm::radians(collider.rotation + transform.rot)), glm::cos(glm::radians(collider.rotation + transform.rot)));
////
////
////		std::array<glm::vec2, 4> vertices = {
////			pos + rotationMatrix * glm::vec2(collider.halfWidth, collider.halfHeight)  ,
////			pos + rotationMatrix * glm::vec2(-collider.halfWidth, collider.halfHeight),
////			pos + rotationMatrix * glm::vec2(-collider.halfWidth, -collider.halfHeight),
////			pos + rotationMatrix * glm::vec2(collider.halfWidth, -collider.halfHeight)
////		};
////
////		return vertices;
////		};
////
////
////	inline auto ProjectOntoAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& axis) {
////		float min = std::numeric_limits<float>::max();
////		float max = std::numeric_limits<float>::min();
////		for (const auto& corner : corners) {
////			const float projection = glm::dot(corner, axis);
////			min = std::min(min, projection);
////			max = std::max(max, projection);
////		}
////		return std::make_pair(min, max);
////	}
////
////	bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis) {
////		const auto [min1, max1] = ProjectOntoAxis(corners1, axis);
////		const auto [min2, max2] = ProjectOntoAxis(corners2, axis);
////		return (min2 <= max1) && (min1 <= max2);
////	}
////
////	inline bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2, const glm::vec2& axis, float& overlapOut) {
////		const auto [min1, max1] = ProjectOntoAxis(corners1, axis);
////		const auto [min2, max2] = ProjectOntoAxis(corners2, axis);
////		bool isOverlapping = (min2 <= max1) && (min1 <= max2);
////		if (isOverlapping) {
////			float overlap1 = max1 - min2;
////			float overlap2 = max2 - min1;
////			overlapOut = std::min(overlap1, overlap2);
////		}
////		return isOverlapping;
////	}
////
////	inline bool CheckOverlapOnAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& circleCenter, const glm::vec2& axis, float& overlapOut)
////	{
////		const auto [min, max] = ProjectOntoAxis(corners, axis);
////		const float circleMin = glm::dot(circleCenter, axis) - glm::length(axis);
////		const float circleMax = glm::dot(circleCenter, axis) + glm::length(axis);
////		const bool isOverlapping = (circleMin <= max) && (min <= circleMax);
////		if (isOverlapping)
////		{
////			float overlap1 = max - circleMin;
////			float overlap2 = circleMax - min;
////			overlapOut = std::min(overlap1, overlap2);
////		}
////		return isOverlapping;
////	}
////
////	inline glm::vec2 CalculateMTV(const glm::vec2& axis, const float& overlap, const glm::vec2& relativePosition) {
////		glm::vec2 mtv = axis * overlap;
////		// Ensure the MTV points from OBB2 to OBB1
////		if (glm::dot(mtv, relativePosition) < 0) {
////			mtv = -mtv;
////		}
////		return mtv;
////	}
////
////	inline CollisionResult CheckCircleOBBColision(const Transform& transform1, const CircleCollider& circleCollider, const Transform& transform2, const BoxCollider& boxCollider)
////	{
////		CollisionResult result;
////		result.overlap = std::numeric_limits<float>::infinity();
////		const auto corners = calculateOBBCorners(transform2, boxCollider);
////		const auto relativePosition = transform1.pos - transform2.pos;
////		const auto rotationMatrix =
////			glm::mat2x2(glm::cos(glm::radians(boxCollider.rotation + transform2.rot)), glm::sin(glm::radians(boxCollider.rotation + transform2.rot)),
////				-glm::sin(glm::radians(boxCollider.rotation + transform2.rot)), glm::cos(glm::radians(boxCollider.rotation + transform2.rot)));
////
////		const auto circleCenter = glm::vec2(transform1.pos) + circleCollider.offset;
////		const auto closestPoint = glm::clamp(circleCenter, glm::vec2(transform2.pos) + rotationMatrix * glm::vec2(-boxCollider.halfWidth, -boxCollider.halfHeight), glm::vec2(transform2.pos) + rotationMatrix * glm::vec2(boxCollider.halfWidth, boxCollider.halfHeight));
////		const auto circleToClosestPoint = closestPoint - circleCenter;
////		const float distance = glm::length(circleToClosestPoint);
////		if (distance > circleCollider.radius)
////		{
////			return CollisionResult();
////		}
////		const glm::vec2 axis = glm::normalize(circleToClosestPoint);
////		float overlap;
////		if (!CheckOverlapOnAxis(corners, circleCenter, axis, overlap))
////		{
////			return CollisionResult();
////		}
////		if (overlap < result.overlap)
////		{
////			result.isColliding = true;
////			result.overlap = overlap;
////			result.collisionNormal = axis;
////			result.mtv = CalculateMTV(axis, overlap, relativePosition);
////		}
////		return result;
////	}
////
////	inline CollisionResult CheckOBBOBBCollision(const std::array<glm::vec2, 4>& corners1, const std::array<glm::vec2, 4>& corners2)
////	{
////		CollisionResult result;
////		result.overlap = std::numeric_limits<float>::infinity();
////		std::array<glm::vec2, 4> axes = {
////			glm::normalize(corners1[0] - corners1[1]),
////			glm::normalize(corners1[0] - corners1[3]),
////			glm::normalize(corners2[1] - corners2[0]),
////			glm::normalize(corners2[3] - corners2[0])
////		};
////
////		const glm::vec2 relativePosition = corners2[0] - corners1[0]; // Vector from OBB1 to OBB2
////		for (auto& axis : axes) {
////			float length = glm::length(axis);
////			if (length > std::numeric_limits<float>::epsilon()) {
////				axis = glm::normalize(axis);
////			}
////			else {
////				continue;
////			}
////			float overlap;
////			if (!CheckOverlapOnAxis(corners1, corners2, axis, overlap)) {
////				return CollisionResult(); // No collision
////			}
////			if (overlap < result.overlap) {
////				result.isColliding = true;
////				result.overlap = overlap;
////				result.collisionNormal = axis;
////				result.mtv = CalculateMTV(axis, overlap, relativePosition);
////			}
////		}
////
////		return result;
////	}
////}