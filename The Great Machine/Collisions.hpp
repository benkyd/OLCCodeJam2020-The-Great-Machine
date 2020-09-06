#ifndef GREATMACHINE_COLLISIONS_H_
#define GREATMACHINE_COLLISIONS_H_

#include <unordered_map>
#include <vector>

#include "olcPixelGameEngine.hpp"

class Entity;
class Tile; 

class Collider
{
    public:
	int x, y, w, h;
};

class CollisionInfo
{
    public:
    Tile* TileCollided;
    Entity* EntityCollided;
    bool CollidingX;
    bool CollidingY;
};

void CollisionTick(olc::PixelGameEngine* engine);

bool EntityCollideDungeon(Entity* entity, std::vector<Tile*>& nearby, int tileSize, CollisionInfo* info, olc::PixelGameEngine* engine);

// e1 needs to have a camera
bool EntityCollide(Entity* e1, Entity* e2, olc::PixelGameEngine* engine);

#endif

