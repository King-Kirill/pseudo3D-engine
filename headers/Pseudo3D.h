#ifndef Pseudo3D_hpp
#define Pseudo3D_hpp
#include <iostream>
#include <map>
#include <vector>
#include "PathEndPosition.h"
#include "LineMap.h"
#include "ConstParams.h"
#include "TempParams.h"
#include <string>
#include "VectorToLines.h"
#include <cmath>
#include "SectorMap.h"

class Pseudo3D
{
private:
	//методы для деления на сектора
	int checkPath(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths, LineMap& tempVector, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors);
	int checkPathPos(std::vector<PathEndPosition>& pathEndPositions, PathEndPosition& path);
	int checkAmountOfPaths(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths, std::vector<std::shared_ptr<LineMap>>& lineArr, std::vector<int>& tempNotToCheckPaths, int currPathId, std::vector<int>& recentPaths, std::vector<int>& notCheckPaths, std::vector<std::vector<int>>& sectorsArray, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors);
	bool shouldCheckSectorLine(int sectorLineId, std::vector<int> linesArr);
	bool shouldCheck(int pathId, std::vector<int>& notCheckPaths);
	void createSector(int currentPathId, int foundPathId, std::vector<int>& notCheckPaths, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths, std::vector<std::vector<int>>& sectorsArray);

	//методы поиска угла и сортировки
	int partitionCosign(std::vector<std::shared_ptr<LineMap>>& arr, int low, int high, LineMap& currentLine);
	void sortCosign(std::vector<std::shared_ptr<LineMap>>& arr, int low, int high, LineMap& line);
	double countCosign(LineMap& lineFirst, LineMap& lineSecond);

	//методы сортировки path'ов внутри линий сектора
	void sortPaths(std::vector<std::vector<int>>& pathsArr, int low, int high, int aimPath, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths);
	int partitionPaths(std::vector<std::vector<int>>& pathsArr, int low, int high, int aimPath, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths);
	double countPathDist(int pathStart, int pathEnd, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths);

	int getCellIndex(double coord, double cellSize);
	void addNewLine(double startPosX, double startPosY, double endPosX, double endPosY, double dirX, double dirY, std::shared_ptr<SectorMap>& ptr, std::shared_ptr<LineMap>& vectorPtr);
	void getArrForRepeat(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors, std::vector<std::shared_ptr<LineMap>>& repeatingVectors);
	void checkForRepeat(std::vector<std::shared_ptr<LineMap>>& repeatingVectors, LineMap line, std::vector<std::shared_ptr<LineMap>>& sectorPaths, std::shared_ptr<SectorMap> sectorPtr);

    //метод отрисовки камеры 
	void createCamera();
	~Pseudo3D();

public:
	void rastorization(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors, std::vector<std::shared_ptr<SectorMap>>& sectorsArray);
	void rastorLine(std::shared_ptr<LineMap>& linePtr, std::weak_ptr<SectorMap>& sectorPtr);
	void performBSP(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors);
};

#endif /* Pseudo3D_hpp */