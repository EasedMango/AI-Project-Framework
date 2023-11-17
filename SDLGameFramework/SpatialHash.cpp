#include "SpatialHash.h"
#include "Transform.h"

SpatialHash::SpatialHash(int bucketSize) : bucketSize(bucketSize)
{

}

int SpatialHash::hash(int bucketX, int bucketY)
{
	// Cantor pairing function:
	return ((bucketX + bucketY) * (bucketX + bucketY + 1) / 2) + bucketY;
}

void SpatialHash::insert(ID entity, const Transform& pos)
{
	int bucketX = pos.pos.x / bucketSize;
	int bucketY = pos.pos.y / bucketSize;
	int hashValue = hash(bucketX, bucketY);
	if (buckets[hashValue].empty()) {
		++totalBuckets;
	}
	buckets[hashValue].push_back(entity);
	++totalEntities;
}

std::vector<ID> SpatialHash::query(const Transform& pos)
{
	const int averageEntitiesPerBucket = totalEntities / std::max(1, totalBuckets);
	const int bucketX = pos.pos.x / bucketSize;
	const int bucketY = pos.pos.y / bucketSize;
	std::vector<ID> potentialCollisions;

	// Reserve an estimated size
	potentialCollisions.reserve(9 * averageEntitiesPerBucket); // Adjust based on your expectations

	// Check adjacent buckets
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			int hashValue = hash(bucketX + i, bucketY + j);
			if (auto it = buckets.find(hashValue); it != buckets.end()) {
				potentialCollisions.insert(potentialCollisions.end(), it->second.begin(), it->second.end());
			}
		}
	}

	return potentialCollisions;
}

void SpatialHash::clear()
{
	buckets.clear();
	totalEntities = 0;
	totalBuckets = 0;
}
