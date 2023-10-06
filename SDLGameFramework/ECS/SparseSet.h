#pragma once
#include <vector>

template<typename T>
class SparseSet {
private:
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
        if (sparse.size() <= entity) sparse.resize(entity + 1, -1);  // Resize sparse array if necessary
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
        return entity < sparse.size() && sparse[entity] < size;
    }
    const std::vector<T>& GetAllDense() const {
        return dense;
    }
    T& operator[](unsigned index) {
        return dense[sparse[index]];
    }

    const T& operator[](unsigned index) const {
        return dense[sparse[index]];
    }

    unsigned getSize() const {
        return size;
    }
};
