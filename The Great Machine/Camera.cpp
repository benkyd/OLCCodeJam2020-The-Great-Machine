#include "Camera.hpp"

#include <cmath>

#include "Things.hpp"
#include "Logger.hpp"

void Camera::Update(float fTime)
{
	if (_Track == nullptr) return;
	_DesiredCoords = _Track->Coords - (ViewPort / 2);
    
	float lerpX = std::lerp(Coords.x, _DesiredCoords.x, _SmoothSpeed);
	float lerpY = std::lerp(Coords.y, _DesiredCoords.y, _SmoothSpeed);
    
	Coords.x = lerpX;
	Coords.y = lerpY;
}

void Camera::Input(olc::PixelGameEngine* engine)
{
    
}

void Camera::TrackEntity(Entity* entity)
{
	_Track = entity;
	entity->TrackingCamera = this;
    Coords.x = _Track->Coords.x - (ViewPort.x / 2);
	Coords.y = _Track->Coords.y - (ViewPort.y / 2);
}
