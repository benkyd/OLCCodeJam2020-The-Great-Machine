#include "Dungeon.hpp"

#include <algorithm>
#include <sstream>

#include "olcPixelGameEngine.hpp"
#include "olcPGEX_AnimatedSprite.hpp"

#include "Collisions.hpp"
#include "Things.hpp"
#include "Camera.hpp"
#include "Logger.hpp"


void PerlinNoise1D(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
    // Used 1D Perlin Noise
    for (int x = 0; x < nCount; x++)
    {
        float fNoise = 0.0f;
        float fScaleAcc = 0.0f;
        float fScale = 1.0f / fBias;
        
        for (int o = 0; o < nOctaves; o++)
        {
            int nPitch = nCount >> o;
            int nSample1 = (x / nPitch) * nPitch;
            int nSample2 = (nSample1 + nPitch) % nCount;
            
            float fBlend = (float)(x - nSample1) / (float)nPitch;
            
            float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
            
            fScaleAcc += fScale;
            fNoise += fSample * fScale;
            // fScale = fScale / fBias;
        }
        
        // Scale to seed range
        fOutput[x] = fNoise / fScaleAcc;
    }
}

static int perlinSize = 10000;
static float* perlinSeed;
static float* perlinOutput;

void SetupPerlin()
{
    perlinSeed = (float*)malloc(sizeof(float) * perlinSize);
    perlinOutput = (float*)malloc(sizeof(float) * perlinSize);
    for (int i = 0; i < perlinSize; i++)
        perlinSeed[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    PerlinNoise1D(perlinSize, perlinSeed, 8, 2.0f, perlinOutput);
}

float GetNextPerlin()
{
    static int i = 0;
    i++;
    if (i > perlinSize)
        i = 0;
    return perlinOutput[i];
}


Dungeon::Dungeon()
: _Logger(Logger::getInstance())
{
    SetupPerlin();
    
	ActiveCamera = new Camera();
	ActiveCamera->Coords = { 0, 0 };
	ActiveCamera->ViewPort = { 1280, 720 };
    
    Player = new Playable();
    
    Player->Coords = { 2, 2 };
    Player->Type = EEntity::Type::Player;
    // Relative to player TL corner
    // not really used ? lol
    Player->HitBox = new Collider{ 0, 0, 28, 36 } ;
    
    Player->Renderable = new olc::Renderable();
    Player->Renderable->Load("res/player.png");
    Player->Animator = new olc::AnimatedSprite();
    Player->Animator->mode = olc::AnimatedSprite::SPRITE_MODE::SINGLE;
    Player->Animator->type = olc::AnimatedSprite::SPRITE_TYPE::DECAL;
    Player->Animator->spriteSheet = Player->Renderable;
    Player->Animator->SetSpriteSize({28, 36});
    
    Player->Animator->AddState("idle", std::vector<olc::vi2d>{
                                   {28, 0}
                               });
    
    Player->Animator->AddState("north", std::vector<olc::vi2d>{
                                   {0, 110},
                                   {28, 110},
                                   {56, 110},
                                   {84, 110}
                               });
    Player->Animator->AddState("east", std::vector<olc::vi2d>{
                                   {0, 36},
                                   {28, 36},
                                   {56, 36},
                                   {84, 36}
                               });
    Player->Animator->AddState("south", std::vector<olc::vi2d>{
                                   {0, 0},
                                   {28, 0},
                                   {56, 0},
                                   {84, 0}
                               });
    Player->Animator->AddState("west", std::vector<olc::vi2d>{
                                   {0, 72},
                                   {28, 72},
                                   {56, 72},
                                   {84, 72}
                               });
    Player->Animator->SetState("idle");
    
    ActiveCamera->TrackEntity(Player);
    ActiveCamera->Update(0.0f);
    
    TileSetDictionary = new TileDictionary();
    TileSetDictionary->Register();
    
    TileSet = new olc::Renderable();
    TileSet->Load("res/dungeon_tileset.png");
    _Logger.Debug("Texture Loaded: ", TileSet, " ", TileSet->Sprite()->width, " ", TileSet->Sprite()->height);
    
    FireOverlay = new olc::Renderable();
    FireOverlay->Load("res/torch.png");
    
}

void Dungeon::Generate()
{
    srand(time(NULL));
    
    DungeonWidth = 0;
    DungeonHeight = 0;
    
    // Generate a dungeon
    // Loosely follows the algorithm in section 3.3 "Agent Based Growing"
    // http://pcgbook.com/wp-content/uploads/chapter03.pdf
    
    struct Room
    {
        int x, y, w, h;
    };
    
    auto randomRoom = [](int x, int y) 
    { return new Room{ x, y, (rand() % 7) + 3, (rand() % 7) + 3 }; };
    
    std::vector<olc::vi2d> tiles;
    
    // Starting at 0,0
    
    int directionChance = 5;
    int roomChance = 5;
    int dungeonMinSize = 3000;
    
    struct Agent
    {
        // 0 up 1 right 2 down 3 left
        int x, y, direction;
    };
    
    Agent* agent = new Agent();
    agent->x = 0; agent->y = 0;
    agent->direction = rand() % 4;
    
    _Logger.Debug("Agent ", agent->x, " ", agent->y, " ", agent->direction);
    
    auto addTile = [&](int x, int y) { 
        for (auto i : tiles)
            if (i.x == x && i.y == y)
            return;
        tiles.push_back({ x, y });
    };
    
    addTile(0, 0);
    
    while (tiles.size() < dungeonMinSize)
    {
        switch (agent->direction)
        {
            case 0: agent->y -= 1; break;
            case 1: agent->x += 1; break;
            case 2: agent->y += 1; break;
            case 3: agent->x -= 1; break;
        }
        addTile(agent->x, agent->y);
        
        if (rand() % 200 < directionChance)
        {
            agent->direction = rand() % 4;
            directionChance = 0;
        }
        else
        {
            directionChance += 5;
        }
        
        if (rand() % 300 < roomChance)
        {
            Room* room = randomRoom(agent->x, agent->y);
            for (int x = room->x; x < room->w + room->x; x++)
                for (int y = room->y; y < room->h + room->y; y++)
                addTile(x, y);
            delete room;
            roomChance = 0;
        }
        else
        {
            roomChance += 5;
        }
    }
    
    for (auto& tile : tiles)
    {
        if (DungeonWidth <= tile.x) DungeonWidth = tile.x;
        if (DungeonHeight <= tile.y) DungeonHeight = tile.y;
        
        ETile::Type tileType = rand() % 100 > 1 ? ETile::Type::Floor : ETile::Type::FloorV1;
        
        auto DoesTileExist = [&](olc::vi2d tile)
        {
            return std::find(tiles.begin(), tiles.end(), tile) != tiles.end();
        };
        
        bool tileBelow = DoesTileExist({ tile.x, tile.y + 1 });
        bool tileAbove = DoesTileExist({ tile.x, tile.y - 1 });
        bool tileLeft  = DoesTileExist({ tile.x - 1, tile.y });
        bool tileRight = DoesTileExist({ tile.x + 1, tile.y });
        
        // Order here is very important! stop fucking it up
        
        if (!tileAbove)
            tileType = ETile::Type::WallU;
        if (!tileRight)
            tileType = ETile::Type::WallR;
        if (!tileBelow)
            tileType = ETile::Type::WallD;
        if (!tileLeft)
            tileType = ETile::Type::WallL;
        
        if (!tileAbove && !tileBelow)
            tileType = ETile::Type::PathAcross;
        if (!tileRight && !tileLeft)
            tileType = ETile::Type::PathUp;
        
        if (!tileAbove && !tileLeft)
            tileType = ETile::Type::WallTLCorner;
        if (!tileAbove && !tileRight)
            tileType = ETile::Type::WallTRCorner;
        if (!tileBelow && !tileLeft)
            tileType = ETile::Type::WallBLCorner;
        if (!tileBelow && !tileRight)
            tileType = ETile::Type::WallBRCorner;
        
        if (!tileAbove && !tileBelow && !tileRight)
            tileType = ETile::Type::PathRight;
        if (!tileAbove && !tileBelow && !tileLeft)
            tileType = ETile::Type::PathLeft;
        if (!tileRight && !tileLeft && !tileAbove)
            tileType = ETile::Type::PathTop;
        if (!tileRight && !tileLeft && !tileBelow)
            tileType = ETile::Type::PathBottom;
        
        // Is the tile below free? if so, make it 3D
        if (!tileBelow)
        {
            Tile* psuedo3DTile = new Tile({ tile.x, tile.y + 1 }, 
                                          ETile::Type::ThreeDStandard, ETile::State::Default);
            DungeonTiles[psuedo3DTile->Coords] = psuedo3DTile;
        }
        
        Tile* t = new Tile(tile, tileType, ETile::State::Default);
        DungeonTiles[t->Coords] = t;
    }
}

void Dungeon::SpawnEntity(Entity* entity)
{
    Entities[entity->Coords] = entity;
}

void Dungeon::Input(olc::PixelGameEngine* engine, float fTime)
{
    
    olc::vf2d oldCoords = Player->Coords;
    
    static std::string state = "idle";
    static std::string lastState = "idle";
    
    if (engine->GetKey(olc::W).bHeld)
    {
        Player->Coords.y -= static_cast<float>(TileSize) * (fTime * Player->Speed);
        if (state != "north")
            state = "north";
    }
    if (engine->GetKey(olc::A).bHeld)
    {
        Player->Coords.x -= static_cast<float>(TileSize) * (fTime * Player->Speed);
        if (state != "west")
            state = "west";
    }
    if (engine->GetKey(olc::S).bHeld)
    {
        Player->Coords.y += static_cast<float>(TileSize) * (fTime * Player->Speed);
        if (state != "south")
            state = "south";
    }
    if (engine->GetKey(olc::D).bHeld)
    {
        Player->Coords.x += static_cast<float>(TileSize) * (fTime * Player->Speed);
        if (state != "east")
            state = "east";
    }
    
    if (engine->GetKey(olc::W).bHeld && engine->GetKey(olc::A).bHeld)
    {
        Player->Coords.y += static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        Player->Coords.x += static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        if (state != "west")
            state = "west";
        
    }
    if (engine->GetKey(olc::W).bHeld && engine->GetKey(olc::D).bHeld)
    {
        Player->Coords.y += static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        Player->Coords.x -= static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        if (state != "east")
            state = "east";
        
    }
    if (engine->GetKey(olc::S).bHeld && engine->GetKey(olc::D).bHeld)
    {
        Player->Coords.y -= static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        Player->Coords.x -= static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        if (state != "east")
            state = "east";
    }
    if (engine->GetKey(olc::S).bHeld && engine->GetKey(olc::A).bHeld)
    {
        Player->Coords.y -= static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        Player->Coords.x += static_cast<float>(TileSize) * (fTime * (Player->Speed / 3.0f));
        if (state != "west")
            state = "west";
    }
    
    if (oldCoords == Player->Coords)
        state = "idle";
    if (state != lastState)
        Player->Animator->SetState(state);
    lastState = state;
    
    
    
    // Map collisions
    olc::vi2d currentTile = { static_cast<int>(Player->Coords.x / TileSize), static_cast<int>(Player->Coords.y / TileSize) };
    
    static olc::vi2d lastTile;
    
    if (lastTile != currentTile)
        _Logger.Debug("Player Tile: ", currentTile.x, " ", currentTile.y);
    
    auto IsMapMember = [&] (int x, int y) {
        std::unordered_map<olc::vi2d, Tile*>::const_iterator found = DungeonTiles.find({x, y});
        return found != DungeonTiles.end();
    };
    
    auto IsNeighbouringMapMember = [&] (int x, int y) {
        if (IsMapMember(x + 1, y)) return true;
        if (IsMapMember(x - 1, y)) return true;
        if (IsMapMember(x, y + 1)) return true;
        if (IsMapMember(x, y - 1)) return true;
        return false;
    };
    
    // get nearby collidables
    std::vector<Tile*> nearbyCollidables;
    
    for (int x = currentTile.x - 1; x <= currentTile.x + 1; x++)
        for (int y = currentTile.y - 1; y <= currentTile.y + 1; y++)
    {
        //_Logger.Debug("IsMapMember ", x, " ", y, " ", IsMapMember(x,y));
        //_Logger.Debug("IsNeighbouringMapMember ", x, " ", y, " ", IsNeighbouringMapMember(x,y));
        
        // If isnt in the map but borders a map tile, add a void type for collision
        if (IsMapMember(x,y))
        {
            nearbyCollidables.push_back(DungeonTiles[{x, y}]);
        } else
        {
            if (IsNeighbouringMapMember(x,y))
                nearbyCollidables.push_back(new Tile({x, y}, ETile::Type::Void, ETile::State::Default));
        }
    }
    
    // Passes ownership back
    CollisionInfo collisionInfo;
    bool colliding = EntityCollide(Player, nearbyCollidables, TileSize, &collisionInfo, engine);
    
    // collision response
    if (colliding)
        Player->Coords = oldCoords;
    
    //if (collisionInfo.CollidingY)
    //Player->Coords.y = oldCoords.y;
    //if (collisionInfo.CollidingX)
    //Player->Coords.x = oldCoords.x;
    
    // delete nearbyCollidables that are void
    for (int i = 0; i < nearbyCollidables.size(); i++)
    {
        if (nearbyCollidables[i]->Type == ETile::Type::Void)
            delete nearbyCollidables[i];
    }
    
    lastTile = currentTile;
}

void Dungeon::Update(olc::PixelGameEngine* engine, float fTime)
{
    ActiveCamera->Update(fTime);
}

olc::Pixel pixelMultiply(const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)
{
    olc::Pixel ret;
    ret.r = pDest.r * pSource.r;
    ret.g = pDest.g * pSource.g;
    ret.b = pDest.b * pSource.b;
    ret.a = pDest.a * pSource.a;
    return ret;
}

void Dungeon::Draw(olc::PixelGameEngine* engine, float fTime)
{
    // Maps not gonna be big enough for me to care about optimistaion
    // maybe i should care? i don't
    
    // Entities are always (tilesize / 3) * 2
    
    // Dungeon Layer
    engine->SetDrawTarget(3);
    engine->Clear({38, 36, 40});
    
    engine->SetPixelMode(olc::Pixel::ALPHA);
    for (std::pair<olc::vi2d, Tile*> tile : DungeonTiles)
        
        engine->DrawPartialDecal({ static_cast<float>((tile.first.x * TileSize) - ActiveCamera->Coords.x), static_cast<float>((tile.first.y * TileSize) - ActiveCamera->Coords.y) },
                                 { static_cast<float>(TileSize), static_cast<float>(TileSize) }, TileSet->Decal(), TileSetDictionary->Dictionary[tile.second->Type], { 16, 16 });
    
    
    // Entity Layer
    engine->SetDrawTarget(2);
    engine->Clear(olc::BLANK);
    
    // Draw character
    Player->Animator->Draw(fTime, {Player->Coords.x - ActiveCamera->Coords.x, Player->Coords.y - ActiveCamera->Coords.y});
    
    // Lighting layers
    engine->SetDrawTarget(1);
    engine->Clear(olc::BLANK);
    
    static std::function<olc::Pixel(const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)> fPixelMultiply = pixelMultiply;
    
    // loads to make it more chaotic
    float lightScale = 1.4f + GetNextPerlin(); GetNextPerlin(); GetNextPerlin(); GetNextPerlin(); GetNextPerlin();
    
    float lightX = static_cast<float>(Player->Coords.x - ActiveCamera->Coords.x) - ((FireOverlay->Sprite()->width * lightScale) / 2.0f);
    float lightY = static_cast<float>(Player->Coords.y - ActiveCamera->Coords.y) - ((FireOverlay->Sprite()->height * lightScale) / 2.0f);
    
    float lightLeft   = lightX + 1.0f;
    float lightRight  = lightX + FireOverlay->Sprite()->width * lightScale - 1.0f;
    float lightTop    = lightY + 1.0f;
    float lightBottom = lightY + FireOverlay->Sprite()->height * lightScale - 1.0f;
    
    // orange tint
    engine->FillRectDecal({0.0f, 0.0f}, {static_cast<float>(engine->ScreenWidth()), static_cast<float>(engine->ScreenHeight())}, olc::Pixel(255, 123, 0, 30));
    
    engine->SetPixelMode(fPixelMultiply);
    engine->DrawDecal({ lightX, lightY }, FireOverlay->Decal(), {lightScale, lightScale}, olc::RED);
    engine->SetPixelMode(olc::Pixel::ALPHA);
    
    // top
    engine->FillRectDecal({0.0f, 0.0f}, {static_cast<float>(engine->ScreenWidth()), lightTop}, olc::BLACK);  
    // right
    engine->FillRectDecal({lightRight, 0.0f}, {static_cast<float>(engine->ScreenWidth()) - lightRight, static_cast<float>(engine->ScreenHeight())}, olc::BLACK); 
    // bottom
    engine->FillRectDecal({0.0f, lightBottom}, {static_cast<float>(engine->ScreenWidth()), static_cast<float>(engine->ScreenHeight()) - lightBottom}, olc::BLACK);
    // left
    engine->FillRectDecal({0.0f, 0.0f}, {lightLeft, static_cast<float>(engine->ScreenHeight())}, olc::BLACK);
    
}

Dungeon::~Dungeon()
{
    delete Player->HitBox;
    delete Player->Renderable;
    delete Player->Animator;
    delete Player;
    
    delete ActiveCamera;
    delete TileSetDictionary;
    delete TileSet;
    for (std::pair<olc::vi2d, Tile*> tile : DungeonTiles)
        delete tile.second;
    for (std::pair<olc::vi2d, Entity*> entity : Entities)
        delete entity.second;
    for (std::pair<olc::vi2d, FixedItem*> entity : FixedItems)
        delete entity.second;
    
    free(perlinSeed);
    free(perlinOutput);
}
