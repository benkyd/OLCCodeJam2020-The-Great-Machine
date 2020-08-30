#ifndef GREATMACHINE_THINGS_H_
#define GREATMACHINE_THINGS_H_

#include <optional>

#include "olcPixelGameEngine.hpp"

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
	olc::vd2d Coords;
	EEntity::Type Type;
};

class Item : public Entity
{
public: 
	EEntity::EItem::Item Item;
};

class FixedItem : public Entity
{
	EEntity::EItem::FixedItem Item;
};


class Tile
{
public:
	olc::vd2d Coords;
	ETile::Type Type;
	ETile::State State;
	
	olc::vd2d SpriteTextureMask;
	olc::Sprite* SpriteMap;

	virtual void Update(float fTime);
	void Draw(olc::PixelGameEngine* engine);
};

#endif
