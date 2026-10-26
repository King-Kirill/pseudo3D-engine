#ifndef SquareMap_hpp
#define SquareMap_hpp
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <stdio.h>
#include "LineMap.h"
#include "SectorMap.h"
#include <vector>

class SquareMap
{
public:
	bool isEmpty;
	std::vector<LineMap*> linesList;
	SquareMap();
};

#endif /* SquareMap_hpp */