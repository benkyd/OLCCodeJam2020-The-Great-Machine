#ifndef GREATMACHINE_CAMERA_H_
#define GREATMACHINE_CAMERA_H_

#include "olcPixelGameEngine.hpp"

class Entity;

class Camera
{
    public:
	olc::vf2d Coords;
	olc::vi2d ViewPort;
    
	void Update(float fTime);
    
	void Input(olc::PixelGameEngine* engine);
    
	void TrackEntity(Entity* e);
    
    private: 
    
	Entity* _Track = nullptr;
    
	olc::vi2d _DesiredCoords;
	float _SmoothSpeed = 0.01f;
    
};

#endif
