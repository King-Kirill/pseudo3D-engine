#ifndef GridDots_hpp
#define GridDots_hpp

#include <iostream>

class GridDots
{
public:
	GridDots(double posX, double posY, int squarePosX, int squarePosY, int squareLength);
	GridDots(double posX, double posY);
	GridDots();

	double posX;
	double posY;

	int squarePosX;
	int squarePosY;
	int squareLength;
};

#endif /* GridDots_hpp */