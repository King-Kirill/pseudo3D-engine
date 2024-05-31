#include "GridDots.h"

GridDots::GridDots(int posX, int posY, int squarePosX, int squarePosY, int squareLength)
{
	this->posX = posX;
	this->posY = posY;
	this->squarePosX = squarePosX;
	this->squarePosY = squarePosY;
	this->squareLength = squareLength;
}

GridDots::GridDots(double posX, double posY)
{
	this->posX = posX;
	this->posY = posY;
}

GridDots::GridDots()
{
	this->posX = 0;
	this->posY = 0;
}

