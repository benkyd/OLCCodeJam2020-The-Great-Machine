#include "Dungeon.hpp"

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
	TileSet = new olc::Sprite("res/dungeon_tileset.png");
    _Logger.Debug("Texture Loaded: ", TileSet, " ", TileSet->width, " ", TileSet->height);

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
    int dungeonMinSize = 2000;

    struct Agent
    {
        // 0 up 1 right 2 down 3 left
        int x, y, direction;
    };

    Agent* agent = new Agent();
    agent->x = 20; agent->y = 10;
    agent->direction = rand() % 4;

    _Logger.Debug("Agent ", agent->x, " ", agent->y, " ", agent->direction);

    auto addTile = [&](int x, int y) { 
        for (auto i : tiles)
            if (i.x == x && i.y == y)
                return;
        tiles.push_back({ x, y });
    };

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
        
        if (rand() % 100 < directionChance)
        {
            agent->direction = rand() % 4;
            directionChance = 0;
        }
        else
        {
            directionChance += 5;
        }

        if (rand() % 200 < roomChance)
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
        Tile* t = new Tile(tile, ETile::Type::OneByOne, ETile::State::Default);
        _Logger.Debug(t->Coords.x, " ", t->Coords.y);
        DungeonTiles[t->Coords] = t;
    }

    // Empty rooms 

}

void Dungeon::SpawnEntity(Entity* entity)
{
	Entities[entity->Coords] = entity;
}

void Dungeon::Input(olc::PixelGameEngine* engine, float fTime)
{
    if (engine->GetKey(olc::W).bHeld)
        Player->Coords.y += 1 / fTime;
    if (engine->GetKey(olc::A).bHeld)
        Player->Coords.x += 1 / fTime;
    if (engine->GetKey(olc::S).bHeld)
        Player->Coords.y -= 1 / fTime;
    if (engine->GetKey(olc::D).bHeld)
        Player->Coords.x -= 1 / fTime;
}

void Dungeon::Update(float fTime)
{
    ActiveCamera->Update(fTime);
}

void Dungeon::Draw(olc::PixelGameEngine* engine)
{
    // find tile in map
    auto DrawFrom = [&](int tile) {
        // mod w for x int div y - JavidX9
        int w = tile % (TileSet->width / 16);
        int h = tile / (TileSet->width / 16);
        return olc::vi2d(w * 16, h * 16);
    };
    
    auto index = [](int x, int y, int depth) -> int { return x + depth * y; };

    for (std::pair<olc::vi2d, Tile*> tile : DungeonTiles)
    {
        engine->DrawPartialSprite({ (tile.first.x * 16) + ActiveCamera->Coords.x, (tile.first.y * 16) + ActiveCamera->Coords.y }, TileSet,
            TileSetDictionary->Dictionary[tile.second->Type], { 16, 16 }, 1);
    }
}

Dungeon::~Dungeon()
{
    for (std::pair<olc::vi2d, Tile*> tile : DungeonTiles)
        delete tile.second;
}
