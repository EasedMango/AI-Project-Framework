#pragma once
#include <functional>
#include <unordered_map>
#include <glm/vec2.hpp>

#include "ecs/CommonECS.h"

struct Transform;

class SpatialHash {
	std::unordered_map<int, std::vector<ID>> buckets;

	// New members to track counts
	int totalEntities = 0;
	int totalBuckets = 0;
public:
	int bucketSize = 4;  // Depending on your needs, you may want a larger or smaller bucket size
	//Atleast slightly bigger than the biggest entity size
	explicit SpatialHash(int bucketSize = 4);;


	int hash(int bucketX, int bucketY);

	void insert(ID entity, const Transform& pos);

	std::vector<ID> query(const Transform& pos);
	std::vector<ID> query(const Transform& pos, int radius);

	void clear();
};

