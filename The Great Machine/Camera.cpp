#include "Camera.hpp"

#include "Things.hpp"

#include "Logger.hpp"

void Camera::Update(float fTime)
{
	if (_Track == nullptr) return;
	Coords.x = _Track->Coords.x - (ViewPort.x / 2);
	Coords.y = _Track->Coords.y - (ViewPort.y / 2);
}

void Camera::Input(olc::PixelGameEngine* engine)
{

}

void Camera::TrackEntity(Entity* entity)
{
	_Track = entity;
}
