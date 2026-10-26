#ifndef LineMap_hpp
#define LineMap_hpp

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

class SectorMap;

class LineMap
{
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

	bool shdCheckSector = true;

	//отдельные параметры для основной не расторизованной линии

	//направляющие по перепендикуляру нормали
	double pDirX = 0;
	double pDirY = 0;

	//указатели на смежные сектора для определения принадлежности линии тому или иному сектору
	//для луча игрока в пределах pDirX и pDirY
	SectorMap* inside_sectorPtr;
	//для луча противоположному в направлении pDirX и pDirY
	SectorMap* outside_sectorPtr;

	//массив указателей unique_ptr на расторизованные прямые для текущей линии
	std::vector<LineMap*> vectorToLines;

	//массив текстур на стену
	//аналогично два массива - один для внутренней стороны стены, другой для внешней
	//если pDirX и pDirY == 0, то пользуется и заполняется первый массив


	LineMap(double startPosX, double starPosY, double endPosX, double endPosY, double dirX, double dirY);
	LineMap();

	//если линия используется одним сектором (если pDirX и pDirY == 0)
	//указатель на сектор которому принадлежит линия (остается т.к на случай переопределения в приоритете будут смежные указатели)
	SectorMap* sectorObj;

	//указатель расторизованной прямой на не расторизованную (оригинальную)
	LineMap* linePtr;

	bool operator==(const LineMap& other) const;
	bool operator!=(const LineMap& other) const;
	LineMap(const LineMap& other);
};

#endif /* LineMap_hpp */