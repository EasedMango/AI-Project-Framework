#include "MapReader.h"

#include <iostream>

#include "Grid.h"

using namespace tinyxml2;


std::vector<std::vector<Tile*>>  ReadMap(const char* path)
{
	XMLDocument doc;
	doc.LoadFile(path);
	XMLElement* root = doc.FirstChildElement("tileset");

	if (root == nullptr)
	{
		std::cout << "No root element found" << std::endl;
		return {};
	}

	auto tileCount = root->IntAttribute("tilecount");
	auto columns = root->IntAttribute("columns");
	auto rows = tileCount / columns;

	std::vector<std::vector<Tile*>> tiles(columns, std::vector<Tile*>(rows, nullptr));

	;


	XMLElement* tile = root->FirstChildElement("tile");

	int currentColumn = 0;
	int currentRow = 0;
	std::cout << "Tiles: \n";
	while (tile != nullptr)
	{
		std::cout << tile->IntAttribute("id") << " ";
		tinyxml2::XMLElement* propertyElement = tile->FirstChildElement("properties")->FirstChildElement("property");
		float cost = 0;
		bool walkable = false;
		bool wall = false;
		while (propertyElement != nullptr) {
			if (std::string propertyName = propertyElement->Attribute("name"); propertyName == "cost") {
				cost = propertyElement->FloatAttribute("value");
			}
			else if (propertyName == "walkable") {
				walkable = propertyElement->BoolAttribute("value");
			}
			else if (propertyName == "wall") {
				wall = propertyElement->BoolAttribute("value");
			}

			propertyElement = propertyElement->NextSiblingElement("property");
		}
		int rowIndex = rows - 1 - currentRow;

		tiles[currentColumn][rowIndex] = new Tile(currentColumn, rowIndex, walkable, cost,wall);

		currentColumn++;
		if (currentColumn >= columns) {
			std::cout << std::endl;
			currentColumn = 0;
			currentRow++;
		}

		tile = tile->NextSiblingElement("tile");
	}

	//for (auto& row : tiles) {
	//	std::ranges::reverse(row);
	//}

	//std::ranges::reverse(tiles);
	return tiles;
}
