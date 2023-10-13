#include "EntityManager.h"

#include "Registry.h"


EntityManager::EntityManager()
= default;



ID EntityManager::CreateEntity() {
	static unsigned id = 0;
	return id++;
}

void EntityManager::DestroyEntity(const ID id)
{
	//delete entityID from entities, nameToID, idToName
	entities.erase(id);

}

