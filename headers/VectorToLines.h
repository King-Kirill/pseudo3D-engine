#ifndef VectorToLines_hpp
#define VectorToLines_hpp

#include <iostream>
#include "LineMap.h"
#include <vector>

class VectorToLines
{
public:
	std::vector<LineMap*> vectorLines;

	double dirX;
	double dirY;

	VectorToLines(std::vector<LineMap*>& newVectorLines);
	VectorToLines();
};

#endif /* VectorToLines_hpp */