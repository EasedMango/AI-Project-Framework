#pragma once
#include <cassert>
#include <vector>
#include <iostream>
#include <set>

struct OneComp
{
    int x;

};

struct TwoComp
{
    int x;
    int y;
};

struct BoolComp
{
    bool comp;
};
template<typename T>
class SparseSet {
private:
    //sparse is sized to the largest entity that has been added to the set
    std::vector<unsigned> sparse;

    std::vector<T> dense;
    unsigned size = 0;

public:
    SparseSet(size_t initialSize = 0) {
        sparse.reserve(initialSize);
        dense.reserve(initialSize);
    }

    // Adds a new entity and its data
    void Add(unsigned entity, T data) {
        //if (typeid(T) == typeid(OneComp)) {
        //    static int totalInt = 0;
        //    static std::set<unsigned> intEntities;
        //    totalInt++;
        //    intEntities.insert(entity);
        //    std::cout << "Adding entity with ID: " << entity << std::endl;
        //    std::cout << "Total " << typeid(T).name() << ": " << totalInt << std::endl;
        //    std::cout << "Entities with " << typeid(T).name() << ": ";
        //    for (auto e : intEntities) {
        //        std::cout << e << ' ';
        //    }
        //    std::cout << '\n' << std::endl;
        //}
        //if (typeid(T) == typeid(TwoComp)) {
        //    static int totalInt = 0;
        //    static std::set<unsigned> intEntities;
        //    totalInt++;
        //    intEntities.insert(entity);
        //    std::cout << "Adding entity with ID: " << entity << std::endl;
        //    std::cout << "Total " << typeid(T).name()<<": " << totalInt << std::endl;
        //    std::cout << "Entities with " << typeid(T).name()<<": ";
        //    for (auto e : intEntities) {
        //        std::cout << e << ' ';
        //    }
        //    std::cout << '\n' << std::endl;
        //}
        //if (typeid(T) == typeid(BoolComp)) {
        //    static int totalInt = 0;
        //    static std::set<unsigned> intEntities;
        //    totalInt++;
        //    intEntities.insert(entity);
        //    std::cout << "Adding entity with ID: " << entity << std::endl;
        //    std::cout << "Total " << typeid(T).name() << ": " << totalInt << std::endl;
        //    std::cout << "Entities with " << typeid(T).name() << ": ";
        //    for (auto e : intEntities) {
        //        std::cout << e << ' ';
        //    }
        //    std::cout << '\n' << std::endl;
        //}
        if (sparse.size() <= entity) 
            sparse.resize(entity + 1, std::numeric_limits<unsigned>::max());  // Resize sparse array if necessary
        sparse[entity] = size;  // Update sparse array to point into dense array
        dense.push_back(data);  // Add data to dense array
        size++;  // Increase size
    }

    // Removes an entity
    void Remove(unsigned entity) {
        if (!Contains(entity)) return;  // Entity not in the set
        unsigned index = sparse[entity];  // Get index from sparse array
        T back = std::move(dense.back());  // Get the last data in the dense array
        dense[index] = std::move(back);  // Overwrite the removed data
        unsigned backEntity = dense.size() - 1;  // Get the entity of the last data
        sparse[backEntity] = index;  // Update the sparse array
        dense.pop_back();  // Remove last data from dense array
        size--;  // Decrease size
    }

    // Checks if an entity is in the set
    bool Contains(unsigned entity) const {
        assert(entity < sparse.size() && "Entity out of bounds in sparse array");

        return entity < sparse.size() && sparse[entity] < size;
    }
    const std::vector<T>& GetAllDense() const {
        return dense;
    }
    T& operator[](unsigned index) {
        //assert(index < sparse.size() && "Entity out of bounds in sparse array");

        return dense[sparse[index]];
    }

    const T& operator[](unsigned index) const {
        assert(index < sparse.size() && "Entity out of bounds in sparse array");
        return dense[sparse[index]];
    }

    unsigned getSize() const {
        return size;
    }
};
