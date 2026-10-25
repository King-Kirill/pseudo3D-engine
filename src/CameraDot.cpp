#include "../headers/CameraDot.h"

CameraDot::CameraDot()
{
	this->posX = 0.0;
	this->posY = 0.0;
	this->wallHeight = 0.0;
	this->hit = false;
}

CameraDot::CameraDot(double posX, double posY, double wallHeight, bool hit)
{
	this->posX = posX;
	this->posY = posY;
	this->wallHeight = wallHeight;
	this->hit = hit;
}