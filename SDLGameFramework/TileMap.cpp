#include "TileMap.h"

#include "Renderer.h"
#include "Transform.h"

TileMap::TileMap(const char* path) : grid(ReadMap(path))
{



}

TileMap::~TileMap()
{
}

void TileMap::Render(const Registry& grid) const
{
}
