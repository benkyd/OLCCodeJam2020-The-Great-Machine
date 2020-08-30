#ifndef GREATMACHINE_DUNGEON_H_
#define GREATMACHINE_DUNGEON_H_

#include <unordered_map>

#include "olcPixelGameEngine.hpp"

class Tile;
class Entity;
class FixedItem;

// Single level dungeon, no need for fancy levels
class Dungeon
{
public:

	void Generate();
	void SpawnEntity();

	void Update(float fTime);
	void Draw(olc::PixelGameEngine* engine);


	std::unordered_map<olc::vd2d, Tile*> Dungeon;
	std::unordered_map<olc::vd2d, Entity*> Entities;
	std::unordered_map<olc::vd2d, FixedItem*> FixedItems;
};

#endif
