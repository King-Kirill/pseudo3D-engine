#ifndef SectorMap_hpp
#define SectorMap_hpp

#include <iostream>
#include "LineMap.h"
#include <vector>

class SectorMap
{
private:
	double ceilingHeight;
	double floorHeight;

public:
	SectorMap();
	~SectorMap();

	std::vector<LineMap*> sectorLines;
	SectorMap(std::vector<LineMap*>& newVectorLines);
};

#endif /* SectorMap_hpp */