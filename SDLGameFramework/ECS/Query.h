#pragma once


#include <bitset>
#include <unordered_map>
#include <vector>


#include "CommonECS.h"


class Query {
private:
    std::vector<std::bitset<MAX_COMPONENTS>> matchingArchetypes;
    const std::unordered_map<std::bitset<MAX_COMPONENTS>, std::vector<ID>>& archetypeEntities;

    template<typename... ComponentTypes>
    std::bitset<MAX_COMPONENTS> GetArchetypeID() const
    {
        std::bitset<MAX_COMPONENTS> archetype;
        auto setAndPrint = [&]<typename T>(T componentType) {
            const std::size_t id = GetComponentTypeId<T>();
            archetype.set(id);
          //  std::cout << id << " ";
            };
        (..., setAndPrint(ComponentTypes{}));
     //   std::cout << std::endl;
        return archetype;
    }

public:
    Query(const std::unordered_map<std::bitset<MAX_COMPONENTS>, std::vector<ID>>& archetypes)
        : archetypeEntities(archetypes)
    {
        for (const auto& archetype : archetypeEntities) {
            matchingArchetypes.push_back(archetype.first);
        }
    }

    template <typename... ComponentTypes>
    Query& Include() {
        std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
        matchingArchetypes.erase(std::remove_if(matchingArchetypes.begin(), matchingArchetypes.end(),
            [conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
                return (archetype & conditions) != conditions;
            }), matchingArchetypes.end());
        return *this;
    }

    template <typename... ComponentTypes>
    Query& Exclude() {
        std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
        matchingArchetypes.erase(std::remove_if(matchingArchetypes.begin(), matchingArchetypes.end(),
            [conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
                return (archetype & conditions).any();
            }), matchingArchetypes.end());
        return *this;
    }

    template <typename... ComponentTypes>
    Query& Any() {
        std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
        matchingArchetypes.erase(std::remove_if(matchingArchetypes.begin(), matchingArchetypes.end(),
            [conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
                return !(archetype & conditions).any(); // Remove if no bits are set
            }), matchingArchetypes.end());
        return *this;
    }


    template <typename... ComponentTypes>
    Query& exact() {
        std::bitset<MAX_COMPONENTS> conditions = GetArchetypeID<ComponentTypes...>();
        matchingArchetypes.erase(std::remove_if(matchingArchetypes.begin(), matchingArchetypes.end(),
            [conditions](const std::bitset<MAX_COMPONENTS>& archetype) {
                return archetype != conditions;
            }), matchingArchetypes.end());
        return *this;
    }
    std::vector<ID> Find() const {
        std::vector<ID> result;
        for (const auto& archetype : matchingArchetypes) {
            result.insert(result.end(), archetypeEntities.at(archetype).begin(), archetypeEntities.at(archetype).end());
        }
        return result;
    }





};
