#ifndef SectorMap_hpp
#define SectorMap_hpp

#include <iostream>
#include "LineMap.h"
#include <vector>
#include "VectorToLines.h"

class SectorMap
{
private:
	double ceilingHeight;
	double floorHeight;

public:
	SectorMap();

	std::vector<VectorToLines> sectorLines;

	SectorMap(std::vector<VectorToLines>& newVectorLines);

	void setCeilingHeight(double height);
	void setFloorHeight(double height);
};

#endif /* SectorMap_hpp */