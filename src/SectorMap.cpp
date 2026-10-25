#include "../headers/SectorMap.h"

SectorMap::SectorMap()
{
    ceilingHeight = 0.00;
    floorHeight = 0.00;
}

SectorMap::~SectorMap()
{
    sectorLines.clear();
}

SectorMap::SectorMap(std::vector<std::shared_ptr<LineMap>>& newVectorLines)
{
    for (int i = 0; i < newVectorLines.size(); i++)
    {
        std::weak_ptr<LineMap> temp_ptr = newVectorLines[i];
        sectorLines.push_back(temp_ptr);
    }

    ceilingHeight = 0.00;
    floorHeight = 0.00;
}