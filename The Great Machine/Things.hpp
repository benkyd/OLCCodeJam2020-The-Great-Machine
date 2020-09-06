#ifndef GREATMACHINE_THINGS_H_
#define GREATMACHINE_THINGS_H_

#include <optional>
#include <array>

#include "olcPixelGameEngine.hpp"
#include "olcPGEX_AnimatedSprite.hpp"

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
        Void,
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

class Collider;
class Camera;

class Entity
{
    public:
	olc::vf2d Coords;
	EEntity::Type Type;
	Collider* HitBox;
    // Does not own
    Camera* TrackingCamera;
	olc::Renderable* Renderable;
    olc::AnimatedSprite* Animator; 
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
    int Life = 7;
	float Speed = 4.0f;
    int SelectedInventoryItem = 0;
	std::array<Item*, 6> Inventory;
};

class Enemy : public Entity
{
    public:
    olc::vf2d dxdy = { 0.0f, 0.0f };
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
	{ 
        if (Type == ETile::Type::None || Type == ETile::Type::Void || Type == ETile::Type::ThreeDStandard)
            IsSolid = true;
        else
            IsSolid = false;
    }
    
	olc::vi2d Coords;
	ETile::Type Type;
	ETile::State State;
    bool IsSolid;
    
    // CBA, just gonna use coords and size
	// Collider* HitBox;
	
	virtual void Update(float fTime);
};

#endif
