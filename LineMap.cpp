#include "LineMap.h"
#include "TempParams.h"

LineMap::LineMap(double startPosX, double startPosY, double endPosX, double endPosY, double dirX, double dirY)
{
	this->startPosX = startPosX;
	this->startPosY = startPosY;

	this->endPosX = endPosX;
	this->endPosY = endPosY;

	this->dirX = dirX;
	this->dirY = dirY;

	lineColorR = currLineColorR;
	lineColorG = currLineColorG;
	lineColorB = currLineColorB;

	ceilHeight = 0.00;
	floorHeight = 0.00;
}

LineMap::LineMap()
{
	this->startPosX = 0;
	this->startPosY = 0;

	this->endPosX = 0;
	this->endPosY = 0;

	this->dirX = 0;
	this->dirY = 0;

	ceilHeight = 0.00;
	floorHeight = 0.00;

	lineColorR = currLineColorR;
	lineColorG = currLineColorG;
	lineColorB = currLineColorB;
}

bool LineMap::operator==(const LineMap& other) const
{
	if (startPosX == other.startPosX
		&& startPosY == other.startPosY
		&& endPosX == other.endPosX
		&& endPosY == other.endPosY
		&& dirX == other.dirX
		&& dirY == other.dirY)
	{
		return true;
	}
	return false;
}

bool LineMap::operator!=(const LineMap& other) const
{
	if (startPosX != other.startPosX
		|| startPosY != other.startPosY
		|| endPosX != other.endPosX
		|| endPosY != other.endPosY
		|| dirX != other.dirX
		|| dirY != other.dirY)
	{
		return true;
	}
	return false;
}

LineMap::LineMap(const LineMap& other)
{
	startPosX = other.startPosX;
	startPosY = other.startPosY;

	endPosX = other.endPosX;
	endPosY = other.endPosY;

	dirX = other.dirX;
	dirY = other.dirY;

	lineColorR = other.lineColorR;
	lineColorG = other.lineColorG;
	lineColorB = other.lineColorB;

	ceilHeight = other.ceilHeight;
	floorHeight = other.floorHeight;
}

void LineMap::setFloorHeight(double height)
{
	floorHeight = height;
}

void LineMap::setCeilHeight(double height)
{
	ceilHeight = height;
}