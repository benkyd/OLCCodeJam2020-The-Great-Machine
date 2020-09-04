#include "Collisions.hpp"

#include "Camera.hpp"
#include "Things.hpp"
#include "Logger.hpp"

bool EntityCollide(Entity* entity, std::vector<Tile*>& nearby, int tileSize, CollisionInfo* info, olc::PixelGameEngine* engine)
{
    if (!entity->HitBox) return false;
    
    for (auto tile : nearby)
    {
        Logger::getInstance().Debug(tile->Coords.x, " ", tile->Coords.y);
        engine->DrawRect({ static_cast<int>((tile->Coords.x + tileSize) - entity->TrackingCamera->Coords.x), static_cast<int>((tile->Coords.y + tileSize) - entity->TrackingCamera->Coords.y) }, {tileSize, tileSize}, olc::RED);
    }
    
}

