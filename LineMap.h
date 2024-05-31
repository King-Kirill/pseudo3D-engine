#ifndef LineMap_hpp
#define LineMap_hpp

#include <iostream>
#include <cmath>
#include <string>

class LineMap
{
private:
	double floorHeight;
	double ceilHeight;

public:
	double startPosX;
	double startPosY;

	double endPosX;
	double endPosY;

	double dirX;
	double dirY;

	int lineColorR;
	int lineColorG;
	int lineColorB;

	LineMap(double startPosX, double starPosY, double endPosX, double endPosY, double dirX, double dirY);
	LineMap();

	bool operator==(const LineMap& other) const;
	bool operator!=(const LineMap& other) const;
	LineMap(const LineMap& other);

	void setFloorHeight(double height);
	void setCeilHeight(double height);
};

#endif /* LineMap_hpp */