#include "Logger.hpp"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.hpp"

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

		return true;
	}

	void DisplayTitle(float fTime)
	{
		if (m_TimeAccumilator > 2.0f)
		{
			m_DeltaFade -= fTime * 200.0f;
			if (m_DeltaFade < 0.1f) m_DeltaFade = 0.0f;
		}
		DrawString((ScreenWidth() / 2) - (7 * 7) * (std::string("The Great Machine").length() / 2), ScreenHeight() / 2, "The Great Machine", olc::Pixel(255, 255, 255, static_cast<int>(m_DeltaFade)), 6);
		DrawString(5, ScreenHeight() - 15, "Powered by the OLC Pixel Game Engine", olc::Pixel(255, 255, 255, static_cast<int>(m_DeltaFade)));
		DrawString(ScreenWidth() - (8 * (std::string("Copyright Benjamin Kyd 2020").length())) - 5, ScreenHeight() - 15, "Copyright Benjamin Kyd 2020", olc::Pixel(255, 255, 255, static_cast<int>(m_DeltaFade)));
	}

	bool OnUserUpdate(float fTime) override
	{
		SetPixelMode(olc::Pixel::ALPHA);
		m_TimeAccumilator += fTime;

		Clear({ 38, 36, 40 });
		
		// _Logger.Debug(m_TimeAccumilator);

		//if (m_TimeAccumilator < 4.0f)
		//{
		//	DisplayTitle(fTime);
		//	return true;
		//}

		_Dungeon->Input(this, fTime);

		_Dungeon->Update(fTime);

		_Dungeon->Draw(this);

		return true;
	}

private:
	long float m_TimeAccumilator = 0;

	float m_DeltaFade = 255.0f;

};


int main()
{
	Logger& _Logger = Logger::getInstance();
	_Logger.InitializeFileLogging("./logs.log");
	_Logger.InitializeLoggingThread();

	Game _Game;
	_Game.Construct(1280, 720, 1, 1, false, false);
	_Logger.Info("Game Constructed");
	
	if (!_Game.Start())
	{
		_Logger.Panic("PGE Cannot start");
	}
}
