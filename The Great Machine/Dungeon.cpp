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

}

Dungeon::~Dungeon()
{

}
