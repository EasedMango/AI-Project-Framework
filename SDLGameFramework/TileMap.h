#pragma once
#include <vector>
#include "Grid.h"
#include "MapReader.h"
#include "ECS/Registry.h"

class TileMap
{
private:
	Grid grid;

	TileMap( const char* path);

public:
	Grid& GetGrid() { return grid; }


	static TileMap& Instance(const char* path = nullptr)
	{
		static TileMap instance( path);
		return instance;
	}

	//delete copy and move constructors and assign operators
	TileMap(TileMap const&) = delete;             // Copy construct
	TileMap(TileMap&&) = delete;                  // Move construct
	TileMap& operator=(TileMap const&) = delete;  // Copy assign
	TileMap& operator=(TileMap&&) = delete;       // Move assign

	~TileMap();
	void Render(const Registry& grid) const;

};
