#include "Collisions.hpp"

#include "Logger.hpp"
#include "Things.hpp"
#include "Camera.hpp"
#include "Collisions.hpp"

bool EntityCollide(Entity* entity, std::vector<Tile*>& nearby, int tileSize, CollisionInfo* info, olc::PixelGameEngine* engine)
{
    static bool ShowCollisionDebug = false;
    if (engine->GetKey(olc::P).bPressed)
        ShowCollisionDebug = !ShowCollisionDebug;
    
    if (!entity->HitBox) return false;
    
    static Logger& _Logger = Logger::getInstance();
    
    engine->SetDrawTarget(uint8_t(0));
    engine->Clear(olc::BLANK);
    
    float entityX = static_cast<float>(entity->Coords.x - entity->TrackingCamera->Coords.x);
    float entityY = static_cast<float>(entity->Coords.y - entity->TrackingCamera->Coords.y);
    float entityW = static_cast<float>(entity->HitBox->w);
    float entityH = static_cast<float>(entity->HitBox->h);
    
    int entityLeft   = static_cast<int>(entityX);
    int entityRight  = static_cast<int>(entityX + entityW);
    int entityTop    = static_cast<int>(entityY);
    int entityBottom = static_cast<int>(entityY + entityH);
    
    if (ShowDebug)
        engine->DrawRect(entityX, entityY, entityW, entityH, olc::RED); 
    
    for (auto tile : nearby)
    {
        if (ShowDebug)
            engine->DrawRect({ static_cast<int>(static_cast<float>((tile->Coords.x * tileSize) - entity->TrackingCamera->Coords.x)), static_cast<int>(static_cast<float>((tile->Coords.y * tileSize) - entity->TrackingCamera->Coords.y)) }, {tileSize, tileSize}, olc::BLUE);
        
        // return if not collidable
        if (!tile->IsSolid) continue;
        
        int tileLeft   = static_cast<int>(static_cast<float>(tile->Coords.x * tileSize) - entity->TrackingCamera->Coords.x);
        int tileRight  = static_cast<int>(static_cast<float>(tile->Coords.x * tileSize) - entity->TrackingCamera->Coords.x) + tileSize;
        int tileTop    = static_cast<int>(static_cast<float>(tile->Coords.y * tileSize) - entity->TrackingCamera->Coords.y);
        int tileBottom = static_cast<int>(static_cast<float>(tile->Coords.y * tileSize) - entity->TrackingCamera->Coords.y) + tileSize;
        
        // _Logger.Debug(entityLeft, " ", tileRight);
        
        bool xOverlaps = (entityLeft <= tileRight) && (entityRight >= tileLeft);
        bool yOverlaps = (entityTop <= tileBottom) && (entityBottom >= tileTop);
        
        bool collision = xOverlaps && yOverlaps;
        
        if (ShowDebug)
            engine->FillRect({static_cast<int>(static_cast<float>((tile->Coords.x * tileSize) - entity->TrackingCamera->Coords.x)), static_cast<int>(static_cast<float>((tile->Coords.y * tileSize) - entity->TrackingCamera->Coords.y))}, {tileSize, tileSize}, collision ? olc::RED : olc::BLUE);
        
        if (!collision) continue;
        
        info->TileCollided = tile;
        
        //info->CollidingX = xOverlaps;
        //info->CollidingY = yOverlaps;
        
        return true;
        
    }
    
    return false;
}

