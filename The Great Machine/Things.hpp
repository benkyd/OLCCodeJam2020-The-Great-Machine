#ifndef GREATMACHINE_THINGS_H_
#define GREATMACHINE_THINGS_H_

#include <optional>
#include <array>

#include "olcPixelGameEngine.hpp"

class Camera;

namespace ETexture
{
	enum Type
	{
		EightByEight,
		SixteenBySixteen
	};
}

namespace ETile
{
	enum Type
	{
		None,
		Floor,
		FloorV1,
		WallL,
		WallR,
		WallU,
		WallD,
		WallTRCorner,
		WallTLCorner,
		WallBRCorner,
		WallBLCorner,
		PathUp,
		PathAcross,
		PathTop,
		PathBottom,
		PathLeft,
		PathRight,
		PathTRCorner,
		PathTLCorner,
		PathBRCorner,
		PathBLCorner,
		OneByOne,
		DoorOpen,
		DoorClosed,
		DoorPortcullis,
		DoorOpenTop,
		DoorClosedTop,
		DoorPortcullisTop,

		ThreeDStandard,
		ThreeDSolid,
		ThreeDSpike,
		ThreeDSpikeBottom
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

// AABB
class HitBox
{
public:
	int x, y, w, h;
};

class Entity
{
public:
	olc::vi2d Coords;
	EEntity::Type Type;
	HitBox* AABBHitBox;
	olc::vf2d SpriteTextureMask;
	olc::Renderable* SpriteMap;
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
public:
	void Register();
	std::map<ETile::Type, olc::vi2d> Dictionary;
};

class Tile
{
public:
	Tile(olc::vi2d coords, ETile::Type type, ETile::State state)
		: Coords(coords)
		, Type(type)
		, State(state)
	{ }

	olc::vi2d Coords;
	ETile::Type Type;
	ETile::State State;
	
	//olc::vf2d SpriteTextureMask;
	//olc::Renderable* SpriteMap;

	virtual void Update(float fTime);
};

#endif
