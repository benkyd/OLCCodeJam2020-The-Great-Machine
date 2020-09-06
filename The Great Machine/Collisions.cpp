#include "Collisions.hpp"

#include "Logger.hpp"
#include "Things.hpp"
#include "Camera.hpp"
#include "Collisions.hpp"

static bool ShowCollisionDebug = false;

void CollisionTick(olc::PixelGameEngine* engine)
{
    if (engine->GetKey(olc::P).bPressed)
        ShowCollisionDebug = !ShowCollisionDebug;
}

bool EntityCollideDungeon(Entity* entity, std::vector<Tile*>& nearby, int tileSize, CollisionInfo* info, olc::PixelGameEngine* engine)
{
    if (!entity->HitBox) return false;
    
    static Logger& _Logger = Logger::getInstance();
    
    engine->SetDrawTarget(1);
    
    float entityX = static_cast<float>(entity->Coords.x - entity->TrackingCamera->Coords.x);
    float entityY = static_cast<float>(entity->Coords.y - entity->TrackingCamera->Coords.y);
    float entityW = static_cast<float>(entity->HitBox->w);
    float entityH = static_cast<float>(entity->HitBox->h);
    
    int entityLeft   = static_cast<int>(entityX);
    int entityRight  = static_cast<int>(entityX + entityW);
    int entityTop    = static_cast<int>(entityY);
    int entityBottom = static_cast<int>(entityY + entityH);
    
    if (ShowCollisionDebug)
        engine->DrawRect(entityX, entityY, entityW, entityH, olc::RED); 
    
    for (auto tile : nearby)
    {
        if (ShowCollisionDebug)
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
        
        if (ShowCollisionDebug)
            engine->FillRect({static_cast<int>(static_cast<float>((tile->Coords.x * tileSize) - entity->TrackingCamera->Coords.x)), static_cast<int>(static_cast<float>((tile->Coords.y * tileSize) - entity->TrackingCamera->Coords.y))}, {tileSize, tileSize}, collision ? olc::RED : olc::BLUE);
        
        if (!collision) continue;
        
        info->TileCollided = tile;
        
        //info->CollidingX = xOverlaps;
        //info->CollidingY = yOverlaps;
        
        return true;
        
    }
    
    return false;
}

bool EntityCollide(Entity* e1, Entity* e2, olc::PixelGameEngine* engine)
{
    if (!e1->HitBox || !e2->HitBox) return false;
    
    static Logger& _Logger = Logger::getInstance();
    
    engine->SetDrawTarget(1);
    
    float e1X = static_cast<float>((e1->Coords.x - e1->TrackingCamera->Coords.x) + static_cast<float>(e1->HitBox->x));
    float e1Y = static_cast<float>((e1->Coords.y - e1->TrackingCamera->Coords.y) + static_cast<float>(e1->HitBox->y));
    float e1W = static_cast<float>(e1->HitBox->w);
    float e1H = static_cast<float>(e1->HitBox->h);
    
    float e2X = static_cast<float>((e2->Coords.x - e1->TrackingCamera->Coords.x) + static_cast<float>(e2->HitBox->x));
    float e2Y = static_cast<float>((e2->Coords.y - e1->TrackingCamera->Coords.y) + static_cast<float>(e2->HitBox->y));
    float e2W = static_cast<float>(e2->HitBox->w);
    float e2H = static_cast<float>(e2->HitBox->h);
    
    bool xOverlaps = (e1X <= e2X + e2W) && (e1X + e1W >= e2X);
    bool yOverlaps = (e1Y <= e2Y + e2H) && (e1Y + e1H >= e2Y);
    
    bool collision = xOverlaps && yOverlaps;
    if (ShowCollisionDebug)
    {
        engine->DrawRect(e1X, e1Y, e1W, e1H, collision ? olc::BLUE : olc::RED); 
        engine->DrawRect(e2X, e2Y, e2W, e2H, collision ? olc::BLUE : olc::RED);
    }
    
    return collision;
}

