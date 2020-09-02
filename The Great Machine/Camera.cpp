#include "Camera.hpp"

#include "Things.hpp"

#include "Logger.hpp"

void Camera::Update(float fTime)
{
	if (_Track == nullptr) return;
	Coords.x = _Track->Coords.x;
	Coords.y = _Track->Coords.y;
}

void Camera::Input(olc::PixelGameEngine* engine)
{

}

void Camera::TrackEntity(Entity* entity)
{
	_Track = entity;
}
