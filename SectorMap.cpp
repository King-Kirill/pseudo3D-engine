#include "SectorMap.h"

SectorMap::SectorMap()
{
    ceilingHeight = 0.00;
    floorHeight = 0.00;
}

SectorMap::SectorMap(std::vector<VectorToLines>& newVectorLines)
{
    sectorLines = newVectorLines;

    ceilingHeight = 0.00;
    floorHeight = 0.00;
}

void SectorMap::setCeilingHeight(double height)
{
    for (int i = 0; i < sectorLines.size(); i++)
    {
        std::vector<LineMap*>& tempVectorsArr = sectorLines[i].vectorLines;

        for (int j = 0; j < tempVectorsArr.size(); j++)
        {
            tempVectorsArr[j]->setCeilHeight(height);
        }
    }
}

void SectorMap::setFloorHeight(double height)
{
    for (int i = 0; i < sectorLines.size(); i++)
    {
        std::vector<LineMap*>& tempVectorsArr = sectorLines[i].vectorLines;

        for (int j = 0; j < tempVectorsArr.size(); j++)
        {
            tempVectorsArr[j]->setFloorHeight(height);
        }
    }
}