#ifndef GREATMACHINE_CAMERA_H_
#define GREATMACHINE_CAMERA_H_

#include "olcPixelGameEngine.hpp"

class Entity;

class Camera
{
public:
	olc::vd2d Coords;
	olc::vd2d ViewPort;

	void Update(float fTime);

	void Input(olc::PixelGameEngine* engine);

	void TrackEntity(Entity* e);

private: 

	Entity* _Track = nullptr;

};

#endif
