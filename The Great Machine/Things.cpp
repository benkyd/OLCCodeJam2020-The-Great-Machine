#include "Things.hpp"

#include "Camera.hpp"

void Tile::Update(float fTime)
{

}

void Tile::Draw(olc::PixelGameEngine* engine, Camera* camera)
{
	engine->DrawPartialSprite(
		{ Coords.x + camera->Coords.x, Coords.y + camera->Coords.y }, 
		SpriteMap, SpriteTextureMask, { 16, 16 });
}
