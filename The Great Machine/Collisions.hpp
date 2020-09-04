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
    
};

bool EntityCollide(Entity* entity, std::vector<Tile*>& nearby, int tileSize, CollisionInfo* info, olc::PixelGameEngine* engine);


#endif
