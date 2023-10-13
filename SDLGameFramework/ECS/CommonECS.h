#pragma once
#include <bitset>
#include <iostream>
#include <typeindex>
#include <unordered_map>
using ID = unsigned int;
constexpr size_t MAX_COMPONENTS = 64;


class ComponentTypeRegistry {
private:
    std::unordered_map<std::type_index, size_t> typeMap;
    size_t nextTypeId = 0;

    ComponentTypeRegistry() {}

public:
    // Deleted copy constructors
    ComponentTypeRegistry(const ComponentTypeRegistry&) = delete;
    ComponentTypeRegistry& operator=(const ComponentTypeRegistry&) = delete;

    // Access singleton instance
    static ComponentTypeRegistry& GetInstance() {
        static ComponentTypeRegistry instance;
        return instance;
    }

    template<typename T>
    size_t GetTypeId() {
        std::type_index typeIndex(typeid(T));

        // If type is already registered, return existing ID
        if (typeMap.find(typeIndex) != typeMap.end()) {
            return typeMap[typeIndex];
        }

        // Otherwise, register the type and return new ID
        size_t newId = nextTypeId++;
        typeMap[typeIndex] = newId;
        std::cout << typeid(T).name() << " ID: " << newId << std::endl;
        return newId;
    }
};

template<typename... ComponentTypes>
inline static std::bitset<MAX_COMPONENTS> GetArchetypeID() {
    std::bitset<MAX_COMPONENTS> archetype;
    (..., archetype.set(ComponentTypeRegistry::GetInstance().GetTypeId<ComponentTypes>()));
    return archetype;
}