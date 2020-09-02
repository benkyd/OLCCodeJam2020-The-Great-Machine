#include "Things.hpp"

#include "Camera.hpp"

void Tile::Update(float fTime)
{

}

void TileDictionary::Register()
{
	Dictionary[ETile::Type::Floor] = { 27, 37 };
	Dictionary[ETile::Type::FloorV1] = { 44, 16 };
	Dictionary[ETile::Type::WallL] = { 0, 16 };
	Dictionary[ETile::Type::WallR] = { 64, 16 };
	Dictionary[ETile::Type::WallU] = { 16, 0 };
	Dictionary[ETile::Type::WallD] = { 16, 64 };
	Dictionary[ETile::Type::WallTRCorner] = { 64, 0 };
	Dictionary[ETile::Type::WallTLCorner] = { 0, 0 };
	Dictionary[ETile::Type::WallBRCorner] = { 64, 64 };
	Dictionary[ETile::Type::WallBLCorner] = { 0, 64 };
	Dictionary[ETile::Type::PathUp] = { 84, 16 };
	Dictionary[ETile::Type::PathAcross] = { 100, 0 };
	Dictionary[ETile::Type::PathTop] = { 84, 64 };
	Dictionary[ETile::Type::PathBottom] = { 84, 80 };
	Dictionary[ETile::Type::PathLeft] = { 0, 96 };
	Dictionary[ETile::Type::PathRight] = { 16, 96 };
	Dictionary[ETile::Type::PathTRCorner] = { 88, 128 };
	Dictionary[ETile::Type::PathTLCorner] = { 56, 128 };
	Dictionary[ETile::Type::PathBRCorner] = { 88, 176 };
	Dictionary[ETile::Type::PathBLCorner] = { 56, 176 };
	Dictionary[ETile::Type::OneByOne] = { 36, 96 };
	Dictionary[ETile::Type::DoorOpen] = { 108, 180 };
	Dictionary[ETile::Type::DoorClosed] = { 124, 180 };
	Dictionary[ETile::Type::DoorPortcullis] = { 208, 108 };
	Dictionary[ETile::Type::DoorOpenTop] = { 208, 108 };
	Dictionary[ETile::Type::DoorClosedTop] = { 224, 108 };
	Dictionary[ETile::Type::DoorPortcullisTop] = { 240, 108 };
	Dictionary[ETile::Type::ThreeDStandard] = { 0, 80 };
	Dictionary[ETile::Type::ThreeDSolid] = { 0, 160 };
	Dictionary[ETile::Type::ThreeDSpike] = { 0, 260 };
	Dictionary[ETile::Type::ThreeDSpikeBottom] = { 0, 276 };
}
