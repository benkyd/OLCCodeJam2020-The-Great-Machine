#ifndef GREATMACHINE_DUNGEON_H_
#define GREATMACHINE_DUNGEON_H_

#include <unordered_map>
#include <vector>

#include <SFML/Audio.hpp>
#include "olcPixelGameEngine.hpp"
#include "olcPGEX_AnimatedSprite.hpp"

#include "Logger.hpp"

class Camera;

class Tile;
class Entity;
class Enemy;
class Playable;
class FixedItem;
class TileDictionary;

// Single level dungeon, no need for fancy levels
class Dungeon
{
    public:
	Dungeon();
    
	void Generate();
    
	void Input(olc::PixelGameEngine* engine, float fTime);
	void Update(olc::PixelGameEngine* engine, float fTime);
	void Draw(olc::PixelGameEngine* engine, float fTime);
    
    bool HasBegun = false;
    bool IsAlive = true;
    bool HasWon = false;
    
    Playable* Player;
	Camera* ActiveCamera;
    
    bool StartedSpawning = false;
    olc::Renderable* EnemyRenderable;
    olc::AnimatedSprite* EnemyAnimator;
    std::vector<Enemy*> Enemies;
    
    int TileSize = 64;
    
	int DungeonWidth;
	int DungeonHeight;
	std::unordered_map<olc::vi2d, Tile*> DungeonTiles;
    
	TileDictionary* TileSetDictionary;
	olc::Renderable* TileSet;
    olc::Renderable* FireOverlay;
    
    olc::Renderable* Lives;
    
    bool IsFireLit = false;
    bool IsLightOn = true;
    sf::SoundBuffer SoundBufferFireLighting;
    sf::Sound SoundFireLighting;
    sf::SoundBuffer SoundBufferFire;
    sf::Sound SoundFire;
    
    sf::SoundBuffer SoundBufferAmbient;
    sf::Sound SoundAmbient;
    sf::SoundBuffer SoundBufferFootsteps;
    sf::Sound SoundFootsteps;
    
    sf::SoundBuffer SoundBufferHit;
    sf::Sound SoundHit;
    
	~Dungeon();
    
    private:
	Logger& _Logger;
};

#endif
