#ifndef GREATMACHINE_DUNGEON_H_
#define GREATMACHINE_DUNGEON_H_

#include <unordered_map>

#include "olcPixelGameEngine.hpp"

class Camera;

class Tile;
class Entity;
class Playable;
class FixedItem;
class TileDictionary;

// Single level dungeon, no need for fancy levels
class Dungeon
{
public:
	Dungeon();

	void Generate();
	void SpawnEntity(Entity* entity);

	void Input(olc::PixelGameEngine* engine);
	void Update(float fTime);
	void Draw(olc::PixelGameEngine* engine);

	Playable* Player;
	Camera* ActiveCamera;

	int DungeonWidth;
	int DungeonHeight;
	std::vector<Tile*> DungeonTiles;
	std::unordered_map<olc::vf2d, Entity*> Entities; // key here could be room?
	std::unordered_map<olc::vf2d, FixedItem*> FixedItems;

	TileDictionary* TileSetDictionary;
	olc::Sprite* TileSet;

	~Dungeon();
};

#endif
