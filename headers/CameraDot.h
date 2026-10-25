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

	std::weak_ptr<LineMap> line_ptr;

	CameraDot();
	CameraDot(double posX, double posY, double wallHeight, bool hit);
};
