#include <iostream>
#include <vector>
#include "LineMap.h"

class CameraDot
{
public:
	double posX;
	double posY;

	double wallHeight = 0;

	bool hit = false;

	// позиции начала отрисовки
	int drawStart = 0.0;
	int drawEnd = 0.0;

	LineMap* line_ptr;

	CameraDot();
	CameraDot(double posX, double posY, double wallHeight, bool hit, int drawStart = 0, int drawEnd = 0);
};
