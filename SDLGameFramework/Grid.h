#pragma once
#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

enum class TileType
{
	Fast = 1,
	Normal,
	Slow

};


struct Tile
{
	int x, y;
	bool walkable;
	float cost;
	float gCost;
	float hCost;

	Tile* parent;

public:
	Tile(int x = 0, int y = 0, bool walkable = true, float cost = 1.f)
	{
		this->x = x;
		this->y = y;
		this->walkable = walkable;
		this->cost = cost;
		parent = nullptr;
	}


	auto GetFCost() const { return gCost + hCost; }

	void Reset()
	{
		gCost = 0;
		hCost = 0;
		parent = nullptr;
	}

};
struct CompareTile {
	bool operator()(const Tile* a, const Tile* b) const {
		return a->GetFCost() > b->GetFCost();
	}
};

template <size_t Width, size_t Height = Width>
class Grid
{
private:
	const size_t width = Width, height = Height;
	std::array<std::array<Tile*, Height>, Width> tiles;
	float Heuristic(Tile* a, Tile* b) {
		// Manhattan distance as an example
		return std::abs(a->x - b->x) + std::abs(a->y - b->y);
	}



	void InitializeTiles()
	{

		for (int x = 0; x < Width; ++x) {
			for (int y = 0; y < Height; ++y) {
				bool walkable = (rand() % 100) > 10; // 80% chance of being walkable
				float cost = walkable ? (1.0f + (rand() % 3)) : FLT_MAX; // Cost between 1 and 3 for walkable tiles
				tiles[x][y] = new Tile(x, y, walkable, cost);
			}
		}
		//int xOffset = Width / 2;
		//int yOffset = Height / 2;
		//for (int x = 0; x < Width; x++)
		//{
		//	for (int y = 0; y < Height; ++y)
		//	{
		//		tiles[x][y] = new Tile(x - xOffset, y - yOffset, true);
		//	}
		//}
	}

public:
	auto GetWidth() const { return width; }
	auto GetHeight() const { return height; }

	static Grid<Width,Height>& GetInstance()
	{
		static Grid<Width, Height> instance;
		return instance;
	}

	void Print(Tile* startTile = nullptr, Tile* endTile = nullptr, const std::vector<Tile*>& path = {})
	{
		for (int y = Height - 1; y >= 0; --y)
		{
			for (int x = 0; x < Width; ++x)
			{
				Tile* currentTile = tiles[x][y];

				if (currentTile == startTile)
				{
					std::cout << "S"; // Start tile
				}
				else if (currentTile == endTile)
				{
					std::cout << "E"; // End tile
				}
				else if (std::ranges::find(path, currentTile) != path.end())
				{
					std::cout << "*"; // Path tile
				}
				else
				{
					std::cout << (currentTile->walkable ? " " : "X");
				}
			}
			std::cout << std::endl;
		}
	}

	Grid()
	{
		tiles = {};
		InitializeTiles();
	}


	~Grid()
	{
		for (int x = 0; x < Width; x++)
		{
			for (int y = 0; y < Height; ++y)
			{
				delete tiles[x][y];
			}
		}
	}



	std::vector<Tile*> GetPath(Tile* startTile, Tile* endTile)
	{
		//tiles that need to be evaluated
		std::priority_queue<Tile*, std::vector<Tile*>, CompareTile> openSet;
		//maps each tile to its cost so far to reach it
		std::unordered_map<Tile*, float> costSoFar;

		startTile->gCost = 0;
		startTile->hCost = Heuristic(startTile, endTile);
		openSet.push(startTile);
		startTile->parent = nullptr;
		costSoFar[startTile] = startTile->gCost;

		//while open set is not empty and there are still tiles to evaluate
		while (!openSet.empty()) {
			//get the tile with the lowest cost
			Tile* current = openSet.top();
			openSet.pop();
			//if we reached the end tile, we are done
			if (current == endTile) {
				break; // Path found
			}
			//check all neighbors of the current tile
			for (auto& next : GetNeighbours(current)) {
				if (next == nullptr)
					continue;
				if (!next || !next->walkable) continue; // Skip non-walkable or null neighbors
				//calculate the cost to get to the next tile
				const float newCost = costSoFar[current] + next->cost;

				//if costSoFar does not contain next then that means it has not been evaluated yet
				//if the new cost is less than the cost so far then that means we found a better path to get there
				if (!costSoFar.contains(next) || newCost < costSoFar[next]) {
					next->gCost = newCost;
					next->hCost = Heuristic(next, endTile);
					costSoFar[next] = next->gCost;
					openSet.push(next);
					next->parent = current;
				}
			}
		}

		if (endTile->parent == nullptr || endTile == nullptr)
		{
			std::cerr << "No path found!" << std::endl;
			return {};
		}
		// Reconstruct path
		std::vector<Tile*> path;
		for (Tile* tile = endTile; tile != nullptr; tile = tile->parent) {
			path.push_back(tile);
		}
		std::ranges::reverse(path);

		return path;
	}



	std::array <Tile*, 4> GetNeighbours(const Tile* tile)
	{
		std::array<Tile*, 4> neighbours;
		int x = tile->x;
		int y = tile->y;

		if (x > 0)
			neighbours[0] = GetTile(x - 1, y);
		else

			neighbours[0] = nullptr;

		if (x < Width - 1)
			neighbours[1] = GetTile(x + 1, y);
		else
			neighbours[1] = nullptr;
		if (y > 0)
			neighbours[2] = GetTile(x, y - 1);
		else
			neighbours[2] = nullptr;
		if (y < Height - 1)
			neighbours[3] = GetTile(x, y + 1);
		else
			neighbours[3] = nullptr;

		return neighbours;
	}

	Tile* GetTile(float x, float y) {
		// Round the values
		x = round(x);
		y = round(y);

		// Clamp the values within the valid range
		x = std::clamp(x, 0.0f, static_cast<float>(Width - 1));
		y = std::clamp(y, 0.0f, static_cast<float>(Height - 1));

		return tiles[static_cast<int>(x)][static_cast<int>(y)];
	}

};
