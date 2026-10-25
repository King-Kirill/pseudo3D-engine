#include "../headers/VectorToLines.h"

VectorToLines::VectorToLines(std::vector<LineMap*>& newVectorLines)
{
	this->vectorLines = newVectorLines;
	this->dirX = NAN;
	this->dirY = NAN;
}

VectorToLines::VectorToLines()
{
	this->dirX = NAN;
	this->dirY = NAN;
}