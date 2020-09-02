#include "Dungeon.hpp"

#include <algorithm>

#include "Things.hpp"
#include "Camera.hpp"
#include "Logger.hpp"

Dungeon::Dungeon()
    : _Logger(Logger::getInstance())
{
	ActiveCamera = new Camera();
	ActiveCamera->Coords = { 0, 0 };
	ActiveCamera->ViewPort = { 1280, 720 };

    Player = new Playable();

    Player->Coords = { 0, 0 };
    Player->Type = EEntity::Type::Player;

    ActiveCamera->TrackEntity(Player);
    ActiveCamera->Update(0.0f);
}

void Dungeon::Generate()
{
    srand(time(NULL));

    TileSetDictionary = new TileDictionary();
    TileSetDictionary->Register();

    TileSet = new olc::Renderable();
    TileSet->Load("res/dungeon_tileset.png");
    _Logger.Debug("Texture Loaded: ", TileSet, " ", TileSet->Sprite()->width, " ", TileSet->Sprite()->height);

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
    int dungeonMinSize = 5000;

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

    // DungeonRenderTarget = new olc::Renderable();
    // DungeonRenderTarget->Create(DungeonWidth * 32, DungeonHeight * 32);
}

void Dungeon::SpawnEntity(Entity* entity)
{
	Entities[entity->Coords] = entity;
}

void Dungeon::Input(olc::PixelGameEngine* engine, float fTime)
{
    if (engine->GetKey(olc::W).bHeld)
        Player->Coords.y -= 1000 * fTime;
    if (engine->GetKey(olc::A).bHeld)
        Player->Coords.x -= 1000 * fTime;
    if (engine->GetKey(olc::S).bHeld)
        Player->Coords.y += 1000 * fTime;
    if (engine->GetKey(olc::D).bHeld)
        Player->Coords.x += 1000 * fTime;
}

void Dungeon::Update(float fTime)
{
    ActiveCamera->Update(fTime);
}

void Dungeon::Draw(olc::PixelGameEngine* engine)
{
    for (std::pair<olc::vi2d, Tile*> tile : DungeonTiles)
    {
        // TODO: Perform culling
        engine->DrawPartialDecal({ static_cast<float>((tile.first.x * 64) - ActiveCamera->Coords.x), static_cast<float>((tile.first.y * 64) - ActiveCamera->Coords.y) },
            { 64, 64 }, TileSet->Decal(), TileSetDictionary->Dictionary[tile.second->Type], { 16, 16 });
    }


    // Draw character
    engine->DrawPartialDecal({ static_cast<float>(Player->Coords.x - ActiveCamera->Coords.x), static_cast<float>(Player->Coords.y - ActiveCamera->Coords.y) },
        { 64, 64 }, TileSet->Decal(), { 143, 130 }, { 16, 16 });

}

Dungeon::~Dungeon()
{
    for (std::pair<olc::vi2d, Tile*> tile : DungeonTiles)
        delete tile.second;
}
