#ifndef GREATMACHINE_THINGS_H_
#define GREATMACHINE_THINGS_H_

#include <optional>
#include <array>

#include "olcPixelGameEngine.hpp"

class Camera;

namespace ETile
{
	enum Type
	{
		Backdrop,
		Floor,
		Wall
	};

	enum State
	{
		Default,
		Exploded
	};
}

namespace EEntity
{
	enum Type
	{
		Item,
		FixedItem,
		Player,
		Enemy,
		Boss
	};

	namespace EItem
	{
		enum Item
		{
			None,
			Sword
		};

		// Fixed items can have states #door open / closed
		enum FixedItem
		{
			Torch,
			Door
		};
	}
}

class Entity
{
public:
	olc::vi2d Coords;
	EEntity::Type Type;
	olc::vf2d SpriteTextureMask;
	olc::Sprite* SpriteMap;
};

class Item : public Entity
{
public: 
	EEntity::EItem::Item Item;
};

class FixedItem : public Entity
{
public:
	EEntity::EItem::FixedItem Item;
};


class Playable : public Entity
{
public:
	std::array<Item*, 6> Inventory;
};


class TileDictionary
{

};

class Tile
{
public:
	olc::vi2d Coords;
	ETile::Type Type;
	ETile::State State;
	
	olc::vf2d SpriteTextureMask;
	olc::Sprite* SpriteMap;

	virtual void Update(float fTime);
};

#endif
