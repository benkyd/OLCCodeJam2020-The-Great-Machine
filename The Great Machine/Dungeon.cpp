#include "Dungeon.hpp"

#include "Things.hpp"
#include "Camera.hpp"

Dungeon::Dungeon()
{
	ActiveCamera = new Camera();
	ActiveCamera->Coords = { 0.0f, 0.0f };
	ActiveCamera->ViewPort = { 1280, 720 };
}

void Dungeon::Generate()
{
	TileSet = new olc::Sprite("./res/dungeon_tileset.png");

}

void Dungeon::SpawnEntity(Entity* entity)
{
	Entities[entity->Coords] = entity;
}

void Dungeon::Input(olc::PixelGameEngine* engine)
{
	// engine->GetKey(olc::W);
}

void Dungeon::Update(float fTime)
{

}

void Dungeon::Draw(olc::PixelGameEngine* engine)
{
    int ActiveTile = MapVector[y * MAPWIDTH + x];

    // sky
    if (ActiveTile == -1) continue;

    // find tile in map
    auto DrawFrom = [&](int tile) {
        // mod w for x int div y - JavidX9
        int w = tile % (TileSet->width / 16);
        int h = tile / (TileSet->width / 16);
        return olc::vi2d(w * 16, h * 16);
    };

	//engine->DrawPartialSprite(
	//	{ static_cast<int>(Coords.x + camera->Coords.x), static_cast<int>(Coords.y + camera->Coords.y) },
	//	SpriteMap, SpriteTextureMask, { 16, 16 });
}

Dungeon::~Dungeon()
{

}
