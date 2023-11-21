#pragma once
#include <vector>

#include "tinyxml2.h"
struct Tile;




[[nodiscard]] std::vector<std::vector<Tile*>> ReadMap(const char* path);

