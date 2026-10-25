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

	std::vector<std::weak_ptr<LineMap>> sectorLines;
	SectorMap(std::vector<std::shared_ptr<LineMap>>& newVectorLines);
};

#endif /* SectorMap_hpp */