#ifndef VectorToLines_hpp
#define VectorToLines_hpp

#include <iostream>
#include "LineMap.h"
#include <vector>

class VectorToLines
{
public:
	LineMap line;
	std::vector<LineMap*> vectorLines;

	VectorToLines(LineMap& newLine, std::vector<LineMap*>& newVectorLines);
	VectorToLines();
};

#endif /* VectorToLines_hpp */