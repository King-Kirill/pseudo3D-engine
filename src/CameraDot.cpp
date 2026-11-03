#include "../headers/CameraDot.h"

CameraDot::CameraDot()
{
	this->posX = 0.0;
	this->posY = 0.0;
	this->wallHeight = 0.0;
	this->hit = false;

	this->drawStart = 0.0;
	this->drawEnd = 0.0;
}

CameraDot::CameraDot(double posX, double posY, double wallHeight, bool hit, int drawStart, int drawEnd)
{
	this->posX = posX;
	this->posY = posY;
	this->wallHeight = wallHeight;
	this->hit = hit;

	this->drawStart = drawStart;
	this->drawEnd = drawEnd;
}