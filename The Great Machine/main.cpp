#include "Logger.hpp"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.hpp"
#define OLC_PGEX_ANIMSPR
#include "olcPGEX_AnimatedSprite.hpp"

#include "Dungeon.hpp"

class Game : public olc::PixelGameEngine
{
    private:
    
	Logger& _Logger;
	Dungeon* _Dungeon;
    
    public:
	Game()
		: _Logger(Logger::getInstance())
	{
		sAppName = "The Great Machine";
	}
    
	bool OnUserCreate() override
	{
		_Dungeon = new Dungeon();
		_Dungeon->Generate();
        
        for (int i = 0; i < 5; i++)
            CreateLayer();
        
        SetDrawTarget(uint8_t(0));
        Clear(olc::BLANK);
		
        return true;
	}
    
	void DisplayTitle(float fTime)
	{
		DrawString((ScreenWidth() / 2) - (7 * 7) * (std::string("The Great Machine").length() / 2), ScreenHeight() / 2, "The Great Machine", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade)), 6);
		DrawString(5, ScreenHeight() - 15, "Powered by the OLC Pixel Game Engine", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade)));
		DrawString(ScreenWidth() - (8 * (std::string("Copyright Benjamin Kyd 2020").length())) - 5, ScreenHeight() - 15, "Copyright Benjamin Kyd 2020", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade)));
	}
    
	bool OnUserUpdate(float fTime) override
	{
		_TimeAccumilator += fTime;
		
        Clear({38, 36, 40});
        // _Logger.Debug(m_TimeAccumilator);
        
        SetDrawTarget(4);
        Clear({38, 36, 40});
        SetDrawTarget(1);
        Clear(olc::BLANK);
        SetDrawTarget(2);
        Clear(olc::BLANK);
        SetDrawTarget(3);
        Clear(olc::BLANK);
        SetDrawTarget(uint8_t(0));
        Clear(olc::BLANK);
        
        goto bruh;
        
        if (_TimeAccumilator < 6.0f)
        {
            Clear({38, 36, 40});
            if (_TimeAccumilator > 4.0f)
            {
                _DeltaFade -= fTime * 200.0f;
                if (_DeltaFade < 0.1f) _DeltaFade = 0.0f;
            }
            DisplayTitle(fTime);
            return true;
        }
        
        
        static bool IsFlicker = false;
        if (_TimeAccumilator > 10.0f && _TimeAccumilator < 27.0f)
        {
            if (rand() % 60 < 1)
                IsFlicker = true;
            if (rand() % 20 < 1)
                IsFlicker = false;
            
            if (IsFlicker)
                _Dungeon->IsLightOn = true;
            else
                _Dungeon->IsLightOn = false;
            
            if (_TimeAccumilator > 13.0f)
            {
                _Dungeon->IsLightOn = false;
                
                if (_TimeAccumilator > 26.0f)
                {
                    _DeltaFade1 -= fTime * 200.0f;
                    if (_DeltaFade1 < 0.1f) _DeltaFade1 = 0.0f;
                }
                // TODO: Center these omd
                SetPixelMode(olc::Pixel::ALPHA);
                if (_TimeAccumilator > 16.0f)
                    DrawString(15, 40, "the machine has done nothing for me", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade1)), 3);
                if (_TimeAccumilator > 20.0f)
                    DrawString(15, 120, "the machine has left me with nothing", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade1)), 3);
                if (_TimeAccumilator > 23.0f)
                    
                    DrawString((ScreenWidth() / 2) - (7 * 7) * (std::string("i am nothing").length() / 2), 300, "i am nothing", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade1)), 6);
                if (_TimeAccumilator > 24.0f)
                    DrawString(15, 650, "your demons can't survive the dark", olc::Pixel(255, 255, 255, static_cast<int>(_DeltaFade1)), 2);
            }
            
        }
        
        if (_TimeAccumilator > 26.0f)
        {
            bruh:
            _Dungeon->IsFireLit = true;
        }
        
        
        _Dungeon->Input(this, fTime);
        
        _Dungeon->Update(this, fTime);
        
        _Dungeon->Draw(this, fTime);
        
        for (int i = 0; i < 5; i++)
            EnableLayer(i, true);
        
        return true;
    }
    
    bool OnUserDestroy() override
    {
        delete _Dungeon;
        return true;
    }
    
    private:
    long float _TimeAccumilator = 0.0f;
    
    float _DeltaFade = 255.0f;
    float _DeltaFade1 = 255.0f;
};

#include <SFML/Audio.hpp>

int main()
{
	Logger& _Logger = Logger::getInstance();
	_Logger.InitializeFileLogging("./logs.log");
	_Logger.InitializeLoggingThread();
    _Logger.Debug("Initializing");
    
    Game _Game;
    _Game.Construct(1280, 720, 1, 1, false, false);
    _Logger.Info("Game Constructed");
    
    if (!_Game.Start())
    {
        _Logger.Panic("PGE Cannot start");
    }
    
    _Logger.~Logger();
}
