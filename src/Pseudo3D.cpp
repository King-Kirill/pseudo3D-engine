#include "../headers/Pseudo3D.h"
#include <thread>

void Pseudo3D::rastorization(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors, std::vector<std::shared_ptr<SectorMap>>& sectorsArray)
{
	std::vector<std::shared_ptr<LineMap>> repeatingLines;
	/*getArrForRepeat(sectors, repeatingLines);*/

	for (int k = 0; k < sectors.size(); k++)
	{
		std::vector<std::shared_ptr<LineMap>>& sectorPaths = sectors[k];
		SectorMap* newSector = new SectorMap(sectorPaths);
		std::shared_ptr<SectorMap> ptr(newSector);
		
		sectorsArray.push_back(ptr);

		for (int i = 0; i < sectorPaths.size(); i++)
		{
			checkForRepeat(repeatingLines, *sectorPaths[i], sectorPaths, ptr);

			LineMap& tempSectorPath = *sectorPaths[i];
			std::shared_ptr<LineMap> vectorPtr(sectorPaths[i]);

			double startPosX = tempSectorPath.startPosX;
			double startPosY = tempSectorPath.startPosY;
			double endPosX = tempSectorPath.endPosX;
			double endPosY = tempSectorPath.endPosY;

			double dirX = tempSectorPath.dirX;
			double dirY = tempSectorPath.dirY;

			double deltaDist_X = (dirX == 0)
				? std::numeric_limits<double>::infinity()
				: fabs(squareWidth / dirX);

			double deltaDist_Y = (dirY == 0)
				? std::numeric_limits<double>::infinity()
				: fabs(squareHeight / dirY);

			double sideDist_X = 0.0;
			double sideDist_Y = 0.0;

			double totalLineLength = sqrt(pow((startPosX - endPosX), 2) + pow((startPosY - endPosY), 2));

			int stepX = 0;
			int stepY = 0;

			int mapX = 0;
			int mapY = 0;

			int map_endX = 0;
			int map_endY = 0;

			int steps_x = 0;
			int steps_y = 0;

			double prevEndX = startPosX;
			double prevEndY = startPosY;

			double prevStartX = 0;
			double prevStartY = 0;

			double main_dist = 0;
			double implement_dist = 0;

			if (dirX != 0 && dirY != 0)
			{
				if (dirX < 0)
				{
					stepX = -1;
					sideDist_X += deltaDist_X;
					//LETS CONTINUE!!!
					mapX = getCellIndex((tempSectorPath.startPosX - (zoomEpsW * squareWidth)), squareWidth);
					map_endX = getCellIndex((tempSectorPath.endPosX - (zoomEpsW * squareWidth)), squareWidth);

					steps_x = mapX - map_endX;
				}
				else
				{
					stepX = 1;
					sideDist_X += deltaDist_X;

					mapX = getCellIndex((tempSectorPath.startPosX - (zoomEpsW * squareWidth)), squareWidth);
					map_endX = getCellIndex((tempSectorPath.endPosX - (zoomEpsW * squareWidth)), squareWidth);

					steps_x = map_endX - mapX;
				}

				if (dirY < 0)
				{
					stepY = -1;
					sideDist_Y += deltaDist_Y;

					mapY = getCellIndex((tempSectorPath.startPosY - (zoomEpsH * squareHeight)), squareHeight);
					map_endY = getCellIndex((tempSectorPath.endPosY - (zoomEpsH * squareHeight)), squareHeight);

					steps_y = mapY - map_endY;
				}
				else
				{
					stepY = 1;
					sideDist_Y += deltaDist_Y;

					mapY = getCellIndex((tempSectorPath.startPosY - (zoomEpsH * squareHeight)), squareHeight);
					map_endY = getCellIndex((tempSectorPath.endPosY - (zoomEpsH * squareHeight)), squareHeight);

					steps_y = map_endY - mapY;
				}

				double tempStartX;
				double tempStartY;

				double temp_prevEndX = 0;
				double temp_prevEndY = 0;

				bool shdChg = true;

				while (steps_x > 0 || steps_y > 0)
				{
					bool moveX = (steps_x > 0);
					bool moveY = (steps_y > 0);

					// Двигаемся по X
					if (moveX && sideDist_X < sideDist_Y)
					{
						double newX = startPosX + sideDist_X * dirX;
						double newY = startPosY + sideDist_X * dirY;

						if (newX != prevEndX || newY != prevEndY)
							addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, vectorPtr);

						prevEndX = newX;
						prevEndY = newY;

						steps_x -= abs(stepX);
						sideDist_X += deltaDist_X;
					}
					// Двигаемся по Y
					else if (moveY && sideDist_Y < sideDist_X)
					{
						double newX = startPosX + sideDist_Y * dirX;
						double newY = startPosY + sideDist_Y * dirY;

						if (newX != prevEndX || newY != prevEndY)
							addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, vectorPtr);

						prevEndX = newX;
						prevEndY = newY;

						steps_y -= abs(stepY);
						sideDist_Y += deltaDist_Y;
					}
					// Угловой случай
					else if (moveX && moveY && fabs(sideDist_X - sideDist_Y) < epsilon)
					{
						double newX = startPosX + sideDist_X * dirX;
						double newY = startPosY + sideDist_X * dirY;

						if (newX != prevEndX || newY != prevEndY)
							addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, vectorPtr);

						prevEndX = newX;
						prevEndY = newY;

						steps_x -= abs(stepX);
						steps_y -= abs(stepY);

						sideDist_X += deltaDist_X;
						sideDist_Y += deltaDist_Y;
					}
					else
					{
						// Если осталось двигаться только по одной оси
						if (moveX)
						{
							double newX = startPosX + sideDist_X * dirX;
							double newY = startPosY + sideDist_X * dirY;

							if (newX != prevEndX || newY != prevEndY)
								addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, vectorPtr);

							prevEndX = newX;
							prevEndY = newY;

							steps_x -= abs(stepX);
							sideDist_X += deltaDist_X;
						}
						else if (moveY)
						{
							double newX = startPosX + sideDist_Y * dirX;
							double newY = startPosY + sideDist_Y * dirY;

							if (newX != prevEndX || newY != prevEndY)
								addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, vectorPtr);

							prevEndX = newX;
							prevEndY = newY;

							steps_y -= abs(stepY);
							sideDist_Y += deltaDist_Y;
						}
					}
				}

				if (prevEndX != endPosX || prevEndY != endPosY)
				{
					// если вдруг в момент выхода из цикла sideDist_X == sideDist_Y и
					// последний угол уже был добавлен, проверка выше не даст дубль-сегмент
					addNewLine(prevEndX, prevEndY, endPosX, endPosY, dirX, dirY, ptr, vectorPtr);

					prevEndX = endPosX;
					prevEndY = endPosY;
				}
			}
			else
			{
				int step = 0;
				int side = 0;
				int steps = 0;

				if (dirX == 0)
				{
					if (dirY > 0)
					{
						step = 1;

						mapY = getCellIndex((tempSectorPath.startPosY - (zoomEpsH * squareHeight)), squareHeight);
						map_endY = getCellIndex((tempSectorPath.endPosY - (zoomEpsH * squareHeight)), squareHeight);

						steps = map_endY - mapY;
					}
					else
					{
						step = -1;

						mapY = getCellIndex((tempSectorPath.startPosY - (zoomEpsH * squareHeight)), squareHeight);
						map_endY = getCellIndex((tempSectorPath.endPosY - (zoomEpsH * squareHeight)), squareHeight);

						steps = mapY - map_endY;
					}

					side = 1;
					addNewLine(startPosX, startPosY, startPosX, startPosY + (step * squareHeight), dirX, dirY, ptr, vectorPtr);
					steps -= 1;

					endPosY = startPosY + (step * squareHeight);
				}
				else if (dirY == 0)
				{
					if (dirX > 0)
					{
						step = 1;

						mapX = getCellIndex((tempSectorPath.startPosX - (zoomEpsW * squareWidth)), squareWidth);
						map_endX = getCellIndex((tempSectorPath.endPosX - (zoomEpsW * squareWidth)), squareWidth);

						steps = map_endX - mapX;
					}
					else
					{
						step = -1;

						mapX = getCellIndex((tempSectorPath.startPosX - (zoomEpsW * squareWidth)), squareWidth);
						map_endX = getCellIndex((tempSectorPath.endPosX - (zoomEpsW * squareWidth)), squareWidth);

						steps = mapX - map_endX;
					}

					side = 0;
					addNewLine(startPosX, startPosY, startPosX + (step * squareWidth), startPosY, dirX, dirY, ptr, vectorPtr);
					steps -= 1;

					endPosX = startPosX + (step * squareWidth);
				} 
				while (steps != 0)
				{
					if (side == 1)
					{
						startPosY = endPosY;
						endPosY += step * squareHeight;

						steps -= 1;
					}
					else
					{
						startPosX = endPosX;
						endPosX += step * squareWidth;

						steps -= 1;
					}

					addNewLine(startPosX, startPosY, endPosX, endPosY, dirX, dirY, ptr, vectorPtr);
				}
			}
		}
	}
}

void Pseudo3D::rastorLine(std::shared_ptr<LineMap>& linePtr, std::weak_ptr<SectorMap>& sectorPtr)
{
	auto ptr = sectorPtr.lock();

	double startPosX = linePtr->startPosX;
	double startPosY = linePtr->startPosY;
	double endPosX = linePtr->endPosX;
	double endPosY = linePtr->endPosY;

	double dirX = linePtr->dirX;
	double dirY = linePtr->dirY;

	double deltaDist_X = (dirX == 0)
		? std::numeric_limits<double>::infinity()
		: fabs(squareWidth / dirX);

	double deltaDist_Y = (dirY == 0)
		? std::numeric_limits<double>::infinity()
		: fabs(squareHeight / dirY);

	double sideDist_X = 0.0;
	double sideDist_Y = 0.0;

	double totalLineLength = sqrt(pow((startPosX - endPosX), 2) + pow((startPosY - endPosY), 2));

	int stepX = 0;
	int stepY = 0;

	int mapX = 0;
	int mapY = 0;

	int map_endX = 0;
	int map_endY = 0;

	int steps_x = 0;
	int steps_y = 0;

	double prevEndX = startPosX;
	double prevEndY = startPosY;

	double prevStartX = 0;
	double prevStartY = 0;

	double main_dist = 0;
	double implement_dist = 0;

	if (dirX != 0 && dirY != 0)
	{
		if (dirX < 0)
		{
			stepX = -1;
			sideDist_X += deltaDist_X;
			//LETS CONTINUE!!!
			mapX = getCellIndex((linePtr->startPosX - (zoomEpsW * squareWidth)), squareWidth);
			map_endX = getCellIndex((linePtr->endPosX - (zoomEpsW * squareWidth)), squareWidth);

			steps_x = mapX - map_endX;
		}
		else
		{
			stepX = 1;
			sideDist_X += deltaDist_X;

			mapX = getCellIndex((linePtr->startPosX - (zoomEpsW * squareWidth)), squareWidth);
			map_endX = getCellIndex((linePtr->endPosX - (zoomEpsW * squareWidth)), squareWidth);

			steps_x = map_endX - mapX;
		}

		if (dirY < 0)
		{
			stepY = -1;
			sideDist_Y += deltaDist_Y;

			mapY = getCellIndex((linePtr->startPosY - (zoomEpsH * squareHeight)), squareHeight);
			map_endY = getCellIndex((linePtr->endPosY - (zoomEpsH * squareHeight)), squareHeight);

			steps_y = mapY - map_endY;
		}
		else
		{
			stepY = 1;
			sideDist_Y += deltaDist_Y;

			mapY = getCellIndex((linePtr->startPosY - (zoomEpsH * squareHeight)), squareHeight);
			map_endY = getCellIndex((linePtr->endPosY - (zoomEpsH * squareHeight)), squareHeight);

			steps_y = map_endY - mapY;
		}

		double tempStartX;
		double tempStartY;

		double temp_prevEndX = 0;
		double temp_prevEndY = 0;

		bool shdChg = true;

		while (steps_x > 0 || steps_y > 0)
		{
			bool moveX = (steps_x > 0);
			bool moveY = (steps_y > 0);

			// Двигаемся по X
			if (moveX && sideDist_X < sideDist_Y)
			{
				double newX = startPosX + sideDist_X * dirX;
				double newY = startPosY + sideDist_X * dirY;

				if (newX != prevEndX || newY != prevEndY)
					addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, linePtr);

				prevEndX = newX;
				prevEndY = newY;

				steps_x -= abs(stepX);
				sideDist_X += deltaDist_X;
			}
			// Двигаемся по Y
			else if (moveY && sideDist_Y < sideDist_X)
			{
				double newX = startPosX + sideDist_Y * dirX;
				double newY = startPosY + sideDist_Y * dirY;

				if (newX != prevEndX || newY != prevEndY)
					addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, linePtr);

				prevEndX = newX;
				prevEndY = newY;

				steps_y -= abs(stepY);
				sideDist_Y += deltaDist_Y;
			}
			// Угловой случай
			else if (moveX && moveY && fabs(sideDist_X - sideDist_Y) < epsilon)
			{
				double newX = startPosX + sideDist_X * dirX;
				double newY = startPosY + sideDist_X * dirY;

				if (newX != prevEndX || newY != prevEndY)
					addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, linePtr);

				prevEndX = newX;
				prevEndY = newY;

				steps_x -= abs(stepX);
				steps_y -= abs(stepY);

				sideDist_X += deltaDist_X;
				sideDist_Y += deltaDist_Y;
			}
			else
			{
				// Если осталось двигаться только по одной оси
				if (moveX)
				{
					double newX = startPosX + sideDist_X * dirX;
					double newY = startPosY + sideDist_X * dirY;

					if (newX != prevEndX || newY != prevEndY)
						addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, linePtr);

					prevEndX = newX;
					prevEndY = newY;

					steps_x -= abs(stepX);
					sideDist_X += deltaDist_X;
				}
				else if (moveY)
				{
					double newX = startPosX + sideDist_Y * dirX;
					double newY = startPosY + sideDist_Y * dirY;

					if (newX != prevEndX || newY != prevEndY)
						addNewLine(prevEndX, prevEndY, newX, newY, dirX, dirY, ptr, linePtr);

					prevEndX = newX;
					prevEndY = newY;

					steps_y -= abs(stepY);
					sideDist_Y += deltaDist_Y;
				}
			}
		}

		if (prevEndX != endPosX || prevEndY != endPosY)
		{
			// если вдруг в момент выхода из цикла sideDist_X == sideDist_Y и
			// последний угол уже был добавлен, проверка выше не даст дубль-сегмент
			addNewLine(prevEndX, prevEndY, endPosX, endPosY, dirX, dirY, ptr, linePtr);

			prevEndX = endPosX;
			prevEndY = endPosY;
		}
	}
	else
	{
		int step = 0;
		int side = 0;
		int steps = 0;

		if (dirX == 0)
		{
			if (dirY > 0)
			{
				step = 1;

				mapY = getCellIndex((linePtr->startPosY - (zoomEpsH * squareHeight)), squareHeight);
				map_endY = getCellIndex((linePtr->endPosY - (zoomEpsH * squareHeight)), squareHeight);

				steps = map_endY - mapY;
			}
			else
			{
				step = -1;

				mapY = getCellIndex((linePtr->startPosY - (zoomEpsH * squareHeight)), squareHeight);
				map_endY = getCellIndex((linePtr->endPosY - (zoomEpsH * squareHeight)), squareHeight);

				steps = mapY - map_endY;
			}

			side = 1;
			addNewLine(startPosX, startPosY, startPosX, startPosY + (step * squareHeight), dirX, dirY, ptr, linePtr);
			steps -= 1;

			endPosY = startPosY + (step * squareHeight);
		}
		else if (dirY == 0)
		{
			if (dirX > 0)
			{
				step = 1;

				mapX = getCellIndex((linePtr->startPosX - (zoomEpsW * squareWidth)), squareWidth);
				map_endX = getCellIndex((linePtr->endPosX - (zoomEpsW * squareWidth)), squareWidth);

				steps = map_endX - mapX;
			}
			else
			{
				step = -1;

				mapX = getCellIndex((linePtr->startPosX - (zoomEpsW * squareWidth)), squareWidth);
				map_endX = getCellIndex((linePtr->endPosX - (zoomEpsW * squareWidth)), squareWidth);

				steps = mapX - map_endX;
			}

			side = 0;
			addNewLine(startPosX, startPosY, startPosX + (step * squareWidth), startPosY, dirX, dirY, ptr, linePtr);
			steps -= 1;

			endPosX = startPosX + (step * squareWidth);
		}
		while (steps != 0)
		{
			if (side == 1)
			{
				startPosY = endPosY;
				endPosY += step * squareHeight;

				steps -= 1;
			}
			else
			{
				startPosX = endPosX;
				endPosX += step * squareWidth;

				steps -= 1;
			}

			addNewLine(startPosX, startPosY, endPosX, endPosY, dirX, dirY, ptr, linePtr);
		}
	}

	clearExposedSquareMapArray(); // очистка просроченых указателей
}


void Pseudo3D::performBSP(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors)
{
	//копируем текущий массив векторов в прошлый
	previousVectorsMapArray = vectorsMapArray;

	//временный массив для сохранения секторов
	std::vector<std::vector<int>> sectorsArray;
	std::vector<std::vector<std::shared_ptr<LineMap>>> sectorPaths;

	//заводим массив содержащий path ID, где будут содержаться ID которые не будут проверяться
	std::vector<int> notCheckPaths;
	
	int currentPathId = 0;

	//массив для сохранения найденных векторров выходящих из одной общей точки
	std::vector<std::shared_ptr<LineMap>> foundLines;
	std::vector<std::shared_ptr<LineMap>> lines;

	//каждый раз после добавления вектора удаляем его из прошлого массива, сокращение итераций цикла
	vectorsMapArray[0]->shdCheckSector = false;
	lines.push_back(vectorsMapArray[0]); 
	vectorsMapArray.erase(vectorsMapArray.begin() + 0);

	//инициализируем первый путь массивом lines
	sectorPaths.push_back(lines);

	//пока не кончатся вектора выполняем
	int j = 0;
	while (vectorsMapArray.size() >= 0)
	{
		//проверка чтобы итератор не был больше указателей внутри массива путей
		if (j < sectorPaths.size())
		{
			if (shouldCheck(j, notCheckPaths))
			{
				//каждый раз берется новый путь для заполнения его линиями
				std::vector<std::shared_ptr<LineMap>>& tempLineMapComponent = sectorPaths[j];

				//проверяем все дотупные вектора, имеют ли они общее начало с выбранным вектором
				for (int i = 0; i < vectorsMapArray.size(); i++)
				{
					if (vectorsMapArray.size() > 0)
					{
						LineMap& vectorMap = *vectorsMapArray[i];
						LineMap& tempLineMap = *tempLineMapComponent[tempLineMapComponent.size() - 1];

						//проверка если вектора совпадают в концах
						if (((fabs(vectorMap.endPosX - tempLineMap.endPosX) < epsilon) &&
							(fabs(vectorMap.endPosY - tempLineMap.endPosY) < epsilon)))
						{
							//разворачиваем вектор, чтобы в пути каждый вектор следовал из другого
							double tempEndPosX = vectorMap.startPosX;
							double tempEndPosY = vectorMap.startPosY;

							vectorMap.startPosX = vectorMap.endPosX;
							vectorMap.startPosY = vectorMap.endPosY;
							vectorMap.endPosX = tempEndPosX;
							vectorMap.endPosY = tempEndPosY;
							vectorMap.dirX = -(vectorMap.dirX);
							vectorMap.dirY = -(vectorMap.dirY);

							//добавляем в массив foundLines найденный вектор, и удаляем из прошлого массива
							vectorsMapArray[i]->shdCheckSector = false;
							foundLines.push_back(vectorsMapArray[i]);
							vectorsMapArray.erase(vectorsMapArray.begin() + i);
							i = -1;
						}
						else if (((fabs(vectorMap.startPosX - tempLineMap.endPosX) < epsilon)
							&& (fabs(vectorMap.startPosY - tempLineMap.endPosY) < epsilon)))
						{
							//добавляем в массив foundLines найденный вектор, и удаляем из прошлого массива
							vectorsMapArray[i]->shdCheckSector = false;
							foundLines.push_back(vectorsMapArray[i]);
							vectorsMapArray.erase(vectorsMapArray.begin() + i);
							i = -1;
						}
					}
				}

				//если кол-во найденных векторов, чье начало
				if (foundLines.size() > 1)
				{
					//вызов метода для сортировки массива
					LineMap& tempLineMap = *tempLineMapComponent[tempLineMapComponent.size() - 1];
					sortCosign(foundLines, 0, foundLines.size() - 1, tempLineMap);

					//заведем временный массив для сохранения считанных path
					//необходимо для того чтобы потом добавить в массив notCheckPaths path'ы которые не надо проверять т.к они содержат только один вектор 
					std::vector<int> tempNotToCheckPaths;
					std::vector<int> createdPaths;

					//добавляем найденные вектора
					for (int i = 0; i < foundLines.size(); i++)
					{
						//увеличиваем на 1 при создании нового path
						currentPathId += 1;

						std::string pathName = "path" + std::to_string(currentPathId);

						std::vector<std::shared_ptr<LineMap>> newPath;
						newPath.push_back(foundLines[i]);

						sectorPaths.push_back(newPath);

						tempNotToCheckPaths.push_back(currentPathId);
						createdPaths.push_back(currentPathId);
					}
					//делаем проверку, являяется ли конечная точка добавленного вектора начальной точкой для более одного вектора
					currentPathId = checkAmountOfPaths(sectorPaths, foundLines, tempNotToCheckPaths, currentPathId, createdPaths, notCheckPaths, sectorsArray, sectors);

					foundLines.clear();
					j++;
				}
				else
				{
					//делаем проверку пути, является ли путь вхождением в другой путь
					LineMap& tempVector = *tempLineMapComponent[tempLineMapComponent.size() - 1];
					int pathId = checkPath(sectorPaths, tempVector, sectors);

					//-1 - значит нет такого пути в который входил бы текущий путь
					if (pathId == -1)
					{
						//проверка является ли массив найденных векторов пустым
						if (foundLines.size() == 1)
						{
							sectorPaths[j].push_back(foundLines[0]);
							foundLines.clear();
						}
						else
						{
							//переход на следующий путь, для текущего вектора нету линий
							j++;
						}
					}
					else
					{
						int foundPathId = pathId;
						createSector(j, pathId, notCheckPaths, sectorPaths, sectorsArray);
						j++;
					}

					if (foundLines.size() != 0)
					{
						for (int k = 0; k < foundLines.size(); k++)
						{
							vectorsMapArray.push_back(foundLines[k]);
						}
						foundLines.clear();
					}
				}
			}
			else
			{
				j++;
			}
		}
		else
		{
			if (vectorsMapArray.size() > 0)
			{
				//инициализируем новый путь массивом lines
				currentPathId += 1;

				std::vector<std::shared_ptr<LineMap>> lines;
				vectorsMapArray[0]->shdCheckSector = false;
				lines.push_back(vectorsMapArray[0]);
				vectorsMapArray.erase(vectorsMapArray.begin() + 0);
				sectorPaths.push_back(lines);
			}
			else
			{
				break;
			}
		}
	}

	for (int i = 0; i < sectorsArray.size(); i++)
	{
		std::vector<int> pathIndexes = sectorsArray[i];
		std::vector<std::shared_ptr<LineMap>> newSector;

		for (int j = 0; j < pathIndexes.size(); j++)
		{
			newSector.insert(newSector.end(), sectorPaths[pathIndexes[j]].begin(), sectorPaths[pathIndexes[j]].end());
		}
		clearUnusedMemmory_simpleArr(newSector);
		sectors.push_back(newSector);
	}

	vectorsMapArray = previousVectorsMapArray;
}

int Pseudo3D::checkPath(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths, LineMap& tempVector, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors)
{
	for (int i = 0; i < sectorPaths.size(); i++)
	{
		std::vector<std::shared_ptr<LineMap>>& pathFirst = sectorPaths[i];
		LineMap& otherPath = *pathFirst[pathFirst.size() - 1];
		LineMap& otherPathZero = *pathFirst[0];

		if ((otherPath != tempVector) && (otherPathZero != tempVector))
		{
			if ((fabs(otherPathZero.startPosX - tempVector.endPosX) < epsilon) && (fabs(otherPathZero.startPosY - tempVector.endPosY) < epsilon))
			{
				return i;
			}
			else if (((fabs(otherPath.endPosX - tempVector.endPosX) < epsilon) && (fabs(otherPath.endPosY - tempVector.endPosY) < epsilon)))
			{
				return i;
			}
		}
		else if ((otherPath == tempVector) && (otherPathZero != tempVector))
		{
			if ((fabs(otherPathZero.startPosX - tempVector.endPosX) < epsilon) && (fabs(otherPathZero.startPosY - tempVector.endPosY) < epsilon))
			{
				return i;
			}
		}
	}
	return -1;
}

int Pseudo3D::checkPathPos(std::vector<PathEndPosition>& pathEndPositions, PathEndPosition& path)
{
	for (int i = 0; i < pathEndPositions.size(); i++)
	{
		if ((fabs(pathEndPositions[i].endPathX - path.endPathX) < epsilon)
			&& (fabs(pathEndPositions[i].endPathY - path.endPathY) < epsilon))
		{
			return i;
		}
	}

	return -1;
}

int Pseudo3D::checkAmountOfPaths(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths, std::vector<std::shared_ptr<LineMap>>& lineArr, std::vector<int>& tempNotToCheckPaths, int currPathId, std::vector<int>& recentPaths, std::vector<int>& notCheckPaths, std::vector<std::vector<int>>& sectorsArray, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors)
{
	for (int j = 0; j < lineArr.size(); j++)
	{
		//проверям нужно ли нам проверять этот path или нет
		if (shouldCheck(recentPaths[j], notCheckPaths))
		{
			std::vector<std::shared_ptr<LineMap>>& tempPathArr = sectorPaths[recentPaths[j]];
			LineMap& tempPath = *tempPathArr[tempPathArr.size() - 1];
			int pathId = checkPath(sectorPaths, tempPath, sectors);

			//заводим массив найденных линий
			std::vector<std::shared_ptr<LineMap>> foundLines;

			for (int i = 0; i < vectorsMapArray.size(); i++)
			{
				LineMap& vectorMap = *vectorsMapArray[i];
				LineMap& lineArrElem = *lineArr[j];

				if (((fabs(vectorMap.endPosX - lineArrElem.endPosX) < epsilon) &&
					(fabs(vectorMap.endPosY - lineArrElem.endPosY) < epsilon)))
				{
					double tempEndPosX = vectorMap.startPosX;
					double tempEndPosY = vectorMap.startPosY;

					vectorMap.startPosX = vectorMap.endPosX;
					vectorMap.startPosY = vectorMap.endPosY;
					vectorMap.endPosX = tempEndPosX;
					vectorMap.endPosY = tempEndPosY;
					vectorMap.dirX = -(vectorMap.dirX);
					vectorMap.dirY = -(vectorMap.dirY);

					vectorsMapArray[i]->shdCheckSector = false;
					foundLines.push_back(vectorsMapArray[i]);

					vectorsMapArray.erase(vectorsMapArray.begin() + i);
					i -= 1;
				}
				else if (((fabs(vectorMap.startPosX - lineArrElem.endPosX) < epsilon)
					&& (fabs(vectorMap.startPosY - lineArrElem.endPosY) < epsilon)))
				{
					vectorsMapArray[i]->shdCheckSector = false;
					foundLines.push_back(vectorsMapArray[i]);

					vectorsMapArray.erase(vectorsMapArray.begin() + i);
					i -= 1;
				}
			}
			if (foundLines.size() > 1)
			{
				LineMap& lineArrElem = *lineArr[j];
				sortCosign(foundLines, 0, foundLines.size() - 1, lineArrElem);

				//снова заводим массив индексов путей которые не надо проверять т.к они содержат по одному вектору
				std::vector<int> tempNotCheckRepeat;
				std::vector<int> createdPaths;

				for (int i = 0; i < foundLines.size(); i++)
				{
					currPathId += 1;

					std::vector<std::shared_ptr<LineMap>> newPath;
					newPath.push_back(foundLines[i]);

					sectorPaths.push_back(newPath);

					tempNotCheckRepeat.push_back(currPathId);
					createdPaths.push_back(currPathId);
				}

				//делаем проверку, являяется ли конечная точка добавленного вектора начальной точкой для более одного вектора
				currPathId = checkAmountOfPaths(sectorPaths, foundLines, tempNotCheckRepeat, currPathId, createdPaths, notCheckPaths, sectorsArray, sectors);
				foundLines.clear();
			}
			else if (foundLines.size() == 1)
			{
				if (pathId != -1)
				{
					std::vector<int> tempNotCheckRepeat;
					std::vector<int> createdPaths;

					currPathId += 1;

					std::vector<std::shared_ptr<LineMap>> newPath;
					newPath.push_back(foundLines[0]);

					sectorPaths.push_back(newPath);

					tempNotCheckRepeat.push_back(currPathId);
					createdPaths.push_back(currPathId);

					currPathId = checkAmountOfPaths(sectorPaths, foundLines, tempNotCheckRepeat, currPathId, createdPaths, notCheckPaths, sectorsArray, sectors);
					foundLines.clear();
				}
				else
				{
					tempNotToCheckPaths.erase(tempNotToCheckPaths.begin() + j); //удаляем path, т.к path содержит менее 2 линий => на этом path не заканчивается
					lineArr.erase(lineArr.begin() + j); //удаляем path, т.к path содержит менее 2 линий => на этом path не заканчивается
					recentPaths.erase(recentPaths.begin() + j);

					vectorsMapArray.push_back(foundLines[0]);
					foundLines.clear();

					j -= 1;
				}
			}
			else
			{
				std::cout << "foundLines is empty" << std::endl;
			}

			if (pathId != -1)
			{
				createSector(recentPaths[j], pathId, notCheckPaths, sectorPaths, sectorsArray);

				auto indexToRemove = std::find(tempNotToCheckPaths.begin(), tempNotToCheckPaths.end(), pathId);
				if (indexToRemove != tempNotToCheckPaths.end())
				{
					tempNotToCheckPaths.erase(indexToRemove);
				}
			}
		}
		else
		{
			std::cout << "NOT checking single path" << recentPaths[j] << std::endl;
		}
	}
	//добавляем в массив путей которые не надо проверять оставшиеся пути
	//пути где по одной линии
	notCheckPaths.insert(notCheckPaths.end(), tempNotToCheckPaths.begin(), tempNotToCheckPaths.end());

	return currPathId;
}

bool Pseudo3D::shouldCheckSectorLine(int sectorLineId, std::vector<int> linesArr)
{
	if (linesArr.size() > 0)
	{
		for (int i = 0; i < linesArr.size(); i++)
		{
			if (sectorLineId == linesArr[i])
			{
				return false;
			}
		}
	}
	return true;
}

bool Pseudo3D::shouldCheck(int pathId, std::vector<int>& notCheckPaths)
{
	if (notCheckPaths.size() > 0)
	{
		for (int i = 0; i < notCheckPaths.size(); i++)
		{
			if (pathId == notCheckPaths[i])
			{
				notCheckPaths.erase(notCheckPaths.begin() + i);
				return false;
			}
		}
	}
	return true;
}

void Pseudo3D::createSector(int currentPathId, int foundPathId, std::vector<int>& notCheckPaths, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths, std::vector<std::vector<int>>& sectorsArray)
{
	//добавляем найденный путь в пути которые НЕ будем проверять 
	notCheckPaths.push_back(foundPathId);

	//массив сектора, состоящий из PATH
	std::vector<int> newSector;

	//получаем по ссылке путь чей id совпадает с найденным
	std::vector<std::shared_ptr<LineMap>>& foundPathArr = sectorPaths[foundPathId];
	std::vector<std::shared_ptr<LineMap>>& currPathArr = sectorPaths[currentPathId];

	LineMap& currPath = *currPathArr[currPathArr.size() - 1];
	LineMap& foundPath = *foundPathArr[0];

	//проверяем, если конец первого вектора в начало другого
	if ((fabs(currPath.endPosX - foundPath.startPosX) < epsilon) &&
		(fabs(currPath.endPosY - foundPath.startPosY) < epsilon))
	{
		//создаем массив, каждый элемент массива является массивом путей
		std::vector<std::vector<int>> sectorLine;

		std::vector<int> newSectorLine;
		newSectorLine.push_back(currentPathId);
		sectorLine.push_back(newSectorLine);

		std::vector<int> notCheckSectorLines;

		bool sectorIsClosed = false;

		while (!sectorIsClosed)
		{
			std::vector<std::vector<int>> addLaterPaths_Arr;

			for (int i = 0; i < sectorLine.size(); i++)
			{
				//проверка надо ли проверять этот массив
				if (shouldCheckSectorLine(i, notCheckSectorLines))
				{
					std::vector<int>& sectorLine_Paths = sectorLine[i];
					std::vector<int> foundVectors;

					std::vector<std::shared_ptr<LineMap>>& tempPath = sectorPaths[sectorLine_Paths[sectorLine_Paths.size() - 1]];

					for (int j = 0; j < sectorPaths.size(); j++)
					{
						std::vector<std::shared_ptr<LineMap>>& sectorPath = sectorPaths[j];
						LineMap& tempPathLine = *tempPath[0];
						LineMap& endLine = *sectorPath[sectorPath.size() - 1];

						if ((fabs(tempPathLine.startPosX - endLine.endPosX) < epsilon) &&
							(fabs(tempPathLine.startPosY - endLine.endPosY) < epsilon))
						{
							foundVectors.push_back(j);
						}
					}

					//если найдено более одного path
					if (foundVectors.size() > 1)
					{
						for (int n = 1; n < foundVectors.size(); n++)
						{
							//переносим данные текущего path в каждый новый и добавляем в каждый path их собственные ветки
							std::vector<int> newLinePaths;
							newLinePaths.insert(newLinePaths.end(), sectorLine_Paths.begin(), sectorLine_Paths.end());
							newLinePaths.push_back(foundVectors[n]);

							addLaterPaths_Arr.push_back(newLinePaths);
						}
						sectorLine_Paths.push_back(foundVectors[0]);
					}
					else if (foundVectors.size() != 0)
					{
						sectorLine_Paths.push_back(foundVectors[0]);
					}
					else
					{
						//запись линии path'ов в массив notCheckSectorLines который не надо проверять, т.к больше path на этом маршруте нет
						notCheckSectorLines.push_back(i);
						continue;
					}
				}
			}

			//добавляем недавно созданные массивы с path, это необходимо чтобы цикл не пошел на проверку недавно созданных массивов
			for (int i = 0; i < addLaterPaths_Arr.size(); i++)
			{
				sectorLine.push_back(addLaterPaths_Arr[i]);
			}

			//копируем текущий массив для сортировки
			std::vector<std::vector<int>>& copy_sectorLine = sectorLine;

			//сортировка
			sortPaths(copy_sectorLine, 0, (copy_sectorLine.size() - 1), foundPathId, sectorPaths);

			//проверяем закрытие сектора
			for (int i = 0; i < copy_sectorLine.size(); i++)
			{
				if (sectorIsClosed)
				{
					break;
				}
				std::vector<int>& sectorLine_Paths = copy_sectorLine[i];

				for (int j = 0; j < sectorLine_Paths.size(); j++)
				{
					if (sectorLine_Paths[j] == foundPathId)
					{
						if ((sectorLine_Paths.size() == 2) && (sectorLine_Paths[0] == sectorLine_Paths[1]))
						{
							newSector.push_back(sectorLine_Paths[0]);
						}
						else
						{
							newSector.insert(newSector.begin(), sectorLine_Paths.begin(), sectorLine_Paths.end());
						}

						sectorsArray.push_back(newSector);

						sectorIsClosed = true;
						break;
					}
				}
			}
		}
	}
	else
	{
		//создаем два массива, каждый элемент массива является массивом путей
		std::vector<std::vector<int>> sectorLine_1; //линия которая вошла в путь
		std::vector<std::vector<int>> sectorLine_2; //линия в которую вошел путь

		//добавляем первые найденые пути в массивы
		std::vector<int> newSectorLine1;
		newSectorLine1.push_back(currentPathId);
		sectorLine_1.push_back(newSectorLine1);

		std::vector<int> newSectorLine2;
		newSectorLine2.push_back(foundPathId);
		sectorLine_2.push_back(newSectorLine2);

		//далее делаем проверку и добавление путей, пока пути не столкнутся
		//если случается, что путь встретил более одного пути из которого он вышел => создаем новые массивы
		//первая найденая линия записывается в массив, в котором произошла встреча

		bool sectorIsDone = false;

		int sectorLine_1_size = sectorLine_1.size();
		int sectorLine_2_size = sectorLine_2.size();

		//массив содержит ID линий секторов которые не надо проверять
		std::vector<int> notCheckSectorLines_line_1;
		std::vector<int> notCheckSectorLines_line_2;

		while (!sectorIsDone)
		{
			//создаем массив новых path lines которые добавим в конце первого for
			std::vector<std::vector<int>> addLaterPaths_Arr_1;
			std::vector<std::vector<int>> addLaterPaths_Arr_2;

			//проходим по каждому массиву принадлежащему линии сектора
			for (int i = 0; i < sectorLine_1.size(); i++)
			{
				//проверка надо ли проверять этот массив
				if (shouldCheckSectorLine(i, notCheckSectorLines_line_1))
				{
					std::vector<int>& sectorLine_1_Paths = sectorLine_1[i];

					std::vector<int> foundVectors;

					std::vector<std::shared_ptr<LineMap>>& tempPath = sectorPaths[sectorLine_1_Paths[sectorLine_1_Paths.size() - 1]];

					for (int j = 0; j < sectorPaths.size(); j++)
					{
						std::vector<std::shared_ptr<LineMap>>& sectorPath = sectorPaths[j];

						LineMap& tempStartLine = *tempPath[0];
						LineMap& tempEndLine = *sectorPath[sectorPath.size() - 1];

						if ((fabs(tempStartLine.startPosX - tempEndLine.endPosX) < epsilon) &&
							(fabs(tempStartLine.startPosY - tempEndLine.endPosY) < epsilon))
						{
							foundVectors.push_back(j);
						}
					}
					//если найдено более одного path
					if (foundVectors.size() > 1)
					{
						for (int n = 1; n < foundVectors.size(); n++)
						{
							//переносим данные текущего path в каждый новый и добавляем в каждый path их собственные ветки
							std::vector<int> newLinePaths;
							newLinePaths.insert(newLinePaths.begin(), sectorLine_1_Paths.begin(), sectorLine_1_Paths.end());
							newLinePaths.push_back(foundVectors[n]);

							addLaterPaths_Arr_1.push_back(newLinePaths);
						}
						sectorLine_1_Paths.push_back(foundVectors[0]);
					}
					else if (foundVectors.size() != 0)
					{
						sectorLine_1_Paths.push_back(foundVectors[0]);
					}
					else
					{
						//запись линии path'ов в массив notCheckSectorLines который не надо проверять, т.к больше path на этом маршруте нет
						notCheckSectorLines_line_1.push_back(i);
						continue;
					}
				}
			}

			//добавляем недавно созданные массивы с path, это необходимо чтобы цико не пошел на проверку недавно созданных массивов
			for (int i = 0; i < addLaterPaths_Arr_1.size(); i++)
			{
				sectorLine_1.push_back(addLaterPaths_Arr_1[i]);
			}

			//аналогичная проверка для второй линии
			for (int i = 0; i < sectorLine_2.size(); i++)
			{
				//проверка надо ли проверять этот массив
				if (shouldCheckSectorLine(i, notCheckSectorLines_line_2))
				{
					std::vector<int>& sectorLine_2_Paths = sectorLine_2[i];
					std::vector<int> foundVectors;

					std::vector<std::shared_ptr<LineMap>>& tempPath = sectorPaths[sectorLine_2_Paths[sectorLine_2_Paths.size() - 1]];

					for (int j = 0; j < sectorPaths.size(); j++)
					{
						std::vector<std::shared_ptr<LineMap>>& sectorPath = sectorPaths[j];

						LineMap& tempStartLine = *tempPath[0];
						LineMap& tempEndLine = *sectorPath[sectorPath.size() - 1];

						if ((fabs(tempStartLine.startPosX - tempEndLine.endPosX) < epsilon) &&
							(fabs(tempStartLine.startPosY - tempEndLine.endPosY) < epsilon))
						{
							foundVectors.push_back(j);
						}
					}

					//если найдено более одного path
					if (foundVectors.size() > 1)
					{
						for (int n = 1; n < foundVectors.size(); n++)
						{
							//переносим данные текущего path в каждый новый и добавляем в каждый path их собственные ветки
							std::vector<int> newLinePaths;
							newLinePaths.insert(newLinePaths.end(), sectorLine_2_Paths.begin(), sectorLine_2_Paths.end());
							newLinePaths.push_back(foundVectors[n]);

							addLaterPaths_Arr_2.push_back(newLinePaths);
						}
						sectorLine_2_Paths.push_back(foundVectors[0]);
					}
					else if (foundVectors.size() != 0)
					{
						sectorLine_2_Paths.push_back(foundVectors[0]);
					}
					else
					{
						//запись линии path'ов в массив notCheckSectorLines который не надо проверять, т.к больше path на этом маршруте нет
						notCheckSectorLines_line_2.push_back(i);
						continue;
					}
				}
			}

			for (int i = 0; i < addLaterPaths_Arr_2.size(); i++)
			{
				sectorLine_2.push_back(addLaterPaths_Arr_2[i]);
			}

			//заводим массивы, каждый массив копия массивов путей
			//это необходимо для сортировки массивов, вся идея в том, чтобы отсортировать пути
			//и начать проверку с путей наиболее близко расположенных к закрытию сектора
			std::vector<std::vector<int>> copy_sectorLine_1 = sectorLine_1;
			std::vector<std::vector<int>> copy_sectorLine_2 = sectorLine_2;

			//сортируем каждый из массивов
			sortPaths(copy_sectorLine_1, 0, (copy_sectorLine_1.size() - 1), foundPathId, sectorPaths);
			sortPaths(copy_sectorLine_2, 0, (copy_sectorLine_2.size() - 1), currentPathId, sectorPaths);

			//проверяем закрытие сектора
			for (int i = 0; i < copy_sectorLine_1.size(); i++)
			{
				if (sectorIsDone == true)
				{
					break;
				}
				std::vector<int>& sectorLine_1_Paths = copy_sectorLine_1[i];

				for (int j = 0; j < copy_sectorLine_2.size(); j++)
				{
					if (sectorIsDone == true)
					{
						break;
					}
					std::vector<int>& sectorLine_2_Paths = copy_sectorLine_2[j];

					//проверка на совпадение путей в каждом из массивов путей
					//если имеется совпадение, то делаем удаление элементов после совпадающих элементов
					//из оставшихся элементов формируется сектор
					for (int n = 0; n < sectorLine_1_Paths.size(); n++)
					{
						if (sectorIsDone == true)
						{
							break;
						}
						for (int m = 0; m < sectorLine_2_Paths.size(); m++)
						{
							if (sectorLine_1_Paths[n] == sectorLine_2_Paths[m])
							{
								sectorLine_1_Paths.erase(sectorLine_1_Paths.begin() + n, sectorLine_1_Paths.end());
								sectorLine_2_Paths.erase(sectorLine_2_Paths.begin() + m, sectorLine_2_Paths.end());

								newSector.insert(newSector.begin(), sectorLine_1_Paths.begin(), sectorLine_1_Paths.end());
								newSector.insert(newSector.end(), sectorLine_2_Paths.begin(), sectorLine_2_Paths.end());

								sectorIsDone = true;

								sectorsArray.push_back(newSector);

								break;
							}
						}
					}
				}
			}
		}
	}
}

int Pseudo3D::partitionCosign(std::vector<std::shared_ptr<LineMap>>& arr, int low, int high, LineMap& currentLine)
{
	//устанавливаем опорный элемент
	LineMap& tempHigh = *arr[high];
	double pivot = countCosign(currentLine, tempHigh);
	int i = low - 1;

	for (int j = low; j <= high - 1; j++)
	{
		LineMap& tempNumer = *arr[j];
		if (countCosign(currentLine, tempNumer) >= pivot)
		{
			//меняем местами найденные элементы
			i++;
			std::swap(arr[i], arr[j]);
		}
	}
	//меняем позициями последний в списке элемент с текущим
	std::swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void Pseudo3D::sortCosign(std::vector<std::shared_ptr<LineMap>>& arr, int low, int high, LineMap& line)
{
	if (low < high) {
		//pivot - получаем индекс опорного элемента
		//элемент разделитель двух массивов внутри одного массива
		int pivot = partitionCosign(arr, low, high, line);

		//Рекурсивно сортируем элементы после pivot
		sortCosign(arr, low, pivot - 1, line);
		//и перед
		sortCosign(arr, pivot + 1, high, line);
	}
}

double Pseudo3D::countCosign(LineMap& lineFirst, LineMap& lineSecond)
{
	//находим общие координаты векторов
	double lineFirstX = lineFirst.endPosX - lineFirst.startPosX;
	double lineFirstY = lineFirst.endPosY - lineFirst.startPosY;

	double lineSecondX = lineSecond.endPosX - lineSecond.startPosX;
	double lineSecondY = lineSecond.endPosY - lineSecond.startPosY;

	//находим также длину векторов, необходимо для скалярного произведения
	double lineFirstLength = sqrt(pow(lineFirstX, 2) + pow(lineFirstY, 2));
	double lineSecondLength = sqrt(pow(lineSecondX, 2) + pow(lineSecondY, 2));

	//значение косинуса
	double cosign = acos(((lineFirstX * lineSecondX) + (lineFirstY * lineSecondY)) / (lineFirstLength * lineSecondLength));

	return cosign;
}

void Pseudo3D::sortPaths(std::vector<std::vector<int>>& pathsArr, int low, int high, int aimPath, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths)
{
	if (low < high) {
		//pivot - получаем индекс опорного элемента
		//элемент разделитель двух массивов внутри одного массива
		int pivot = partitionPaths(pathsArr, low, high, aimPath, sectorPaths);

		//Рекурсивно сортируем элементы после pivot
		sortPaths(pathsArr, low, pivot - 1, aimPath, sectorPaths);
		//и перед
		sortPaths(pathsArr, pivot + 1, high, aimPath, sectorPaths);
	}
}

int Pseudo3D::partitionPaths(std::vector<std::vector<int>>& pathsArr, int low, int high, int aimPath, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths)
{
	std::vector<int>& tempStringArr = pathsArr[high];

	double pivot = countPathDist(tempStringArr[tempStringArr.size() - 1], aimPath, sectorPaths);
	int i = low - 1;

	for (int j = low; j <= high - 1; j++)
	{
		std::vector<int>& tempArr = pathsArr[j];

		if (countPathDist(tempArr[tempArr.size() - 1], aimPath, sectorPaths) <= pivot)
		{
			i++;
			std::swap(pathsArr[i], pathsArr[j]);
		}
	}
	std::swap(pathsArr[i + 1], pathsArr[high]);
	return i + 1;
}

double Pseudo3D::countPathDist(int pathStart, int pathEnd, std::vector<std::vector<std::shared_ptr<LineMap>>>& sectorPaths)
{
	std::vector<std::shared_ptr<LineMap>>& startLinesArr = sectorPaths[pathStart];
	std::vector<std::shared_ptr<LineMap>>& endLinesArr = sectorPaths[pathEnd];

	LineMap& startLineEnd = *startLinesArr[startLinesArr.size() - 1];
	LineMap& startLineStart = *endLinesArr[0];

	double startX = startLineEnd.endPosX;
	double startY = startLineEnd.endPosY;

	double endX = startLineStart.startPosX;
	double endY = startLineStart.startPosY;

	double distance = sqrt(pow((startX - endX), 2) + pow((startY - endY), 2));

	return distance;
}

int Pseudo3D::getCellIndex(double coord, double cellSize) {
	return static_cast<int>(std::floor((coord / cellSize) + cell_epsilon));
}

void Pseudo3D::addNewLine(double startPosX, double startPosY, double endPosX, double endPosY, double dirX, double dirY, std::shared_ptr<SectorMap> &ptr, std::shared_ptr<LineMap>& vectorPtr)
{
	double midX = (startPosX + endPosX) / 2.0;
	double midY = (startPosY + endPosY) / 2.0;

	int mapX = int((midX / squareWidth) - zoomEpsW);
	int mapY = int((midY / squareHeight) - zoomEpsH);

	auto sptr = std::make_shared<LineMap>(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
	std::weak_ptr<LineMap> wptr = sptr;

	sptr->sectorObj = ptr;
	sptr->linePtr = vectorPtr;

	vectorPtr->vectorToLines.push_back(sptr);

	squareMapArray[mapY][mapX].isEmpty = false;
	squareMapArray[mapY][mapX].linesList.push_back(wptr);
}

void Pseudo3D::getArrForRepeat(std::vector<std::vector<std::shared_ptr<LineMap>>>& sectors, std::vector<std::shared_ptr<LineMap>>& repeatingVectors)
{
	for (int i = 0; i < sectors.size(); i++)
	{
		for (int j = 0; j < sectors[i].size(); j++)
		{
			LineMap& obj1 = *sectors[i][j];

			for (int k = 0; k < sectors.size(); k++)
			{
				for (int l = 0; l < sectors[k].size(); l++)
				{
					std::shared_ptr<LineMap> ptr(sectors[k][l]);
					LineMap& obj2 = *ptr;

					if (k != i)
					{
						if (&obj1 == &obj2)  // сравниваем адреса объектов, а не объекты через operator==
						{
							// проверяем, что этого указателя ещё нет в repeatingVectors
							bool exists = false;
							for (std::shared_ptr<LineMap> vecPtr : repeatingVectors)
							{
								if (vecPtr == ptr)
								{
									exists = true;
									break;
								}
							}
							if (!exists)
							{
								repeatingVectors.push_back(ptr);
							}
						}
					}
				}
			}
		}
	}
}

void Pseudo3D::checkForRepeat(
	std::vector<std::shared_ptr<LineMap>>& repeatingVectors,
	LineMap line,
	std::vector<std::shared_ptr<LineMap>>& sectorPaths, std::shared_ptr<SectorMap> sectorPtr)
{
	for (int i = 0; i < repeatingVectors.size(); i++)
	{
		LineMap& repeatVector = *repeatingVectors[i];
		if (line == repeatVector)
		{
			double x0 = (line.startPosX + line.endPosX) / 2.0;
			double y0 = (line.startPosY + line.endPosY) / 2.0;

			double rayDirX = line.dirY;
			double rayDirY = -line.dirX;

			for (int j = 0; j < sectorPaths.size(); j++)
			{
				LineMap& sectorLine = *sectorPaths[j];
				if (line != sectorLine)
				{
					double sector_x0 = sectorLine.startPosX;
					double sector_y0 = sectorLine.startPosY;
					double sector_dirX = sectorLine.dirX; 
					double sector_dirY = sectorLine.dirY;

					double det = rayDirX * sector_dirY - rayDirY * sector_dirX;

					if (fabs(det) < epsilon) {
						continue;
					}

					double dx = sector_x0 - x0;
					double dy = sector_y0 - y0;

					double t = (dx * sector_dirY - dy * sector_dirX) / det;
					double u = (dx * rayDirY - dy * rayDirX) / det;

					double hit_x = x0 + t * rayDirX;
					double hit_y = y0 + t * rayDirY;

					double min_x = std::min(sectorLine.startPosX, sectorLine.endPosX);
					double max_x = std::max(sectorLine.startPosX, sectorLine.endPosX);
					double min_y = std::min(sectorLine.startPosY, sectorLine.endPosY);
					double max_y = std::max(sectorLine.startPosY, sectorLine.endPosY);

					bool inside_segment =
						(hit_x >= min_x - epsilon && hit_x <= max_x + epsilon &&
							hit_y >= min_y - epsilon && hit_y <= max_y + epsilon);

					bool forward = (t > epsilon);

					if (inside_segment && forward)
					{
						sectorLine.pDirX = -rayDirX;
						sectorLine.pDirY = -rayDirY;
						sectorLine.inside_sectorPtr = sectorPtr;
					}
					else
					{
						sectorLine.outside_sectorPtr = sectorPtr;
					}
				}
			}
		} 
	}
}

Pseudo3D::~Pseudo3D()
{
	clearSquareMapArray();
}