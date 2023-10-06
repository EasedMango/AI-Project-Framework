#pragma once
#include <memory>
using ID = unsigned int;

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T>
using ScopePtr = std::unique_ptr<T>;
template <typename T>
using WeakRef = std::weak_ptr<T>;