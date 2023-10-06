#pragma once
#include <unordered_set>

#include "CommonECS.h"


class EntityManager
{
private:



	std::unordered_set<ID> entities;

	std::vector<ID> freeIDs;

public:
	EntityManager();
	~EntityManager();

	static ID CreateEntity();

	void DestroyEntity(const ID id);
};

