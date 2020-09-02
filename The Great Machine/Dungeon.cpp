#include "Dungeon.hpp"

#include "Things.hpp"
#include "Camera.hpp"
#include "Logger.hpp"

Dungeon::Dungeon()
{
	ActiveCamera = new Camera();
	ActiveCamera->Coords = { 0.0f, 0.0f };
	ActiveCamera->ViewPort = { 1280, 720 };
}

void Dungeon::Generate()
{
    TileSetDictionary = new TileDictionary();
    TileSetDictionary->Register();
	TileSet = new olc::Sprite("res/dungeon_tileset.png");
    Logger::getInstance().Debug("Texture Loaded: ", TileSet, " ", TileSet->width, " ", TileSet->height);

    auto index = [](int x, int y, int depth) -> int { return x + depth * y; };

    // Generate a dungeon
    // Loosely follows the algorithm in section 3.3 "Agent Based Growing"
    // http://pcgbook.com/wp-content/uploads/chapter03.pdf

    struct Room
    {
        int x, y, w, h;
    };

    auto randomRoom = [](int x, int y) 
        { return new Room{ x, y, (rand() % 15) + 5, (rand() % 15) + 5 }; };

    std::vector<Room*> rooms;
    std::map<olc::vi2d, bool> tiles;

    // Starting at 0,0

    olc::vi2d agentPos;



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
    
    // find tile in map
    auto DrawFrom = [&](int tile) {
        // mod w for x int div y - JavidX9
        int w = tile % (TileSet->width / 16);
        int h = tile / (TileSet->width / 16);
        return olc::vi2d(w * 16, h * 16);
    };
    
    auto index = [](int x, int y, int depth) -> int { return x + depth * y; };

    for (int x = 0; x < DungeonWidth; x++)
    for (int y = 0; y < DungeonHeight; y++)
    {
        static Logger& _Logger = Logger::getInstance();
        // _Logger.Debug(x, " ", y);
        Tile* tile = DungeonTiles[index(x, y, DungeonWidth)];
        
        engine->DrawPartialSprite({ x * 32, y * 32 }, TileSet, 
                                  TileSetDictionary->Dictionary[tile->Type], { 16, 16 }, 2);
    }
}

Dungeon::~Dungeon()
{
    for (int i = 0; i < DungeonTiles.size(); i++)
        delete DungeonTiles[i];
}
