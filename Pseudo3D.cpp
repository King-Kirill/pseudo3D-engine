#include "Pseudo3D.h"

void Pseudo3D::rastorization(std::vector<LineMap>& sectorPaths)
{
	std::vector<VectorToLines> newVectorLines;

	for (int i = 0; i < sectorPaths.size(); i++)
	{
		std::vector<LineMap*> sectorLines_pointers;

		std::cout << "step1" << std::endl;

		double startPosX = sectorPaths[i].startPosX;
		double startPosY = sectorPaths[i].startPosY;
		double endPosX = sectorPaths[i].endPosX;
		double endPosY = sectorPaths[i].endPosY;

		double dirX = sectorPaths[i].dirX;
		double dirY = sectorPaths[i].dirY;

		std::cout << "dirX: " << dirX << std::endl;
		std::cout << "dirY: " << dirY << std::endl;

		std::cout << "step2" << std::endl;

		if (startPosX == endPosX)
		{
			std::cout << "step3" << std::endl;
			if (startPosY > endPosY)
			{
				int linesAmount = (startPosY - endPosY) / ceil_h;
				std::cout << "step4" << std::endl;
				if (linesAmount == 1)
				{
					std::cout << "step5" << std::endl;
					LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
					squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].isEmpty = false;
					squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].linesList.push_back(newLine);

					sectorLines_pointers.push_back(newLine);
				}
				else
				{
					std::cout << "step6" << std::endl;
					endPosY = startPosY - ceil_h;
					for (int i = 0; i < linesAmount; i++)
					{
						std::cout << "startPosY " << i << " : " << startPosY << std::endl;
						std::cout << "endPosY " << i << " : " << endPosY << std::endl;

						LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].isEmpty = false;
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].linesList.push_back(newLine);

						sectorLines_pointers.push_back(newLine);

						startPosY = startPosY - (ceil_h);
						endPosY = endPosY - (ceil_h);
					}
				}
			}
			else if (startPosY < endPosY)
			{
				std::cout << "step7" << std::endl;
				int linesAmount = (endPosY - startPosY) / ceil_h;

				if (linesAmount == 1)
				{
					std::cout << "step8" << std::endl;
					LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
					squareMapArray[int(startPosY / ceil_h)][int(startPosX / ceil_w)].isEmpty = false;
					squareMapArray[int(startPosY / ceil_h)][int(startPosX / ceil_w)].linesList.push_back(newLine);

					sectorLines_pointers.push_back(newLine);
				}
				else
				{
					startPosY = endPosY - ceil_h;
					std::cout << "step9" << std::endl;
					for (int i = 0; i < linesAmount; i++)
					{
						std::cout << "startPosY " << i << " : " << startPosY << std::endl;
						std::cout << "endPosY " << i << " : " << endPosY << std::endl;

						LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].isEmpty = false;
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].linesList.push_back(newLine);

						sectorLines_pointers.push_back(newLine);

						startPosY = startPosY - (ceil_h);
						endPosY = endPosY - (ceil_h);
					}
				}
			}
		}
		else if (startPosY == endPosY)
		{
			std::cout << "step10" << std::endl;
			if (startPosX > endPosX)
			{
				int linesAmount = (startPosX - endPosX) / ceil_w;

				if (linesAmount == 1)
				{
					std::cout << "step11" << std::endl;
					LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
					squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].isEmpty = false;
					squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].linesList.push_back(newLine);

					sectorLines_pointers.push_back(newLine);
				}
				else
				{
					std::cout << "step12" << std::endl;
					endPosX = startPosX - ceil_w;
					for (int i = 0; i < linesAmount; i++)
					{
						std::cout << "startPosX " << i << " : " << startPosX << std::endl;
						std::cout << "endPosX " << i << " : " << endPosX << std::endl;

						LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].isEmpty = false;
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].linesList.push_back(newLine);

						sectorLines_pointers.push_back(newLine);

						startPosX = startPosX - (ceil_w);
						endPosX = endPosX - (ceil_w);
					}
				}
			}
			else if (startPosX < endPosX)
			{
				std::cout << "step13" << std::endl;
				int linesAmount = (endPosX - startPosX) / ceil_w;

				if (linesAmount == 1)
				{
					LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
					squareMapArray[int(startPosY / ceil_h)][int(startPosX / ceil_w)].isEmpty = false;
					squareMapArray[int(startPosY / ceil_h)][int(startPosX / ceil_w)].linesList.push_back(newLine);

					sectorLines_pointers.push_back(newLine);
				}
				else
				{
					std::cout << "step14" << std::endl;
					startPosX = endPosX - ceil_w;
					for (int i = 0; i < linesAmount; i++)
					{
						std::cout << "startPosX " << i << " : " << startPosX << std::endl;
						std::cout << "endPosX " << i << " : " << endPosX << std::endl;

						LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].isEmpty = false;
						squareMapArray[int(endPosY / ceil_h)][int(endPosX / ceil_w)].linesList.push_back(newLine);

						sectorLines_pointers.push_back(newLine);

						startPosX = startPosX - (ceil_w);
						endPosX = endPosX - (ceil_w);
					}
				}
			}
		}
		else
		{
			std::cout << "step15" << std::endl;
			double lineStartX = startPosX;
			double lineStartY = startPosY;
			double lineEndX = 0;
			double lineEndY = 0;

			int cooficient = 0;
			std::cout << "step16" << std::endl;
			double lineLengthX = abs(endPosX - startPosX);
			double lineLengthY = abs(endPosY - startPosY);

			if (lineLengthX > lineLengthY)
			{
				cooficient = lineLengthY / ceil_h;
			}
			else
			{
				cooficient = lineLengthX / ceil_w;
			}

			std::cout << "step17" << std::endl;
			double stepX = dirX * ceil_w;
			double stepY = dirY * ceil_h;

			std::cout << "stepX: " << stepX << std::endl;
			std::cout << "stepY: " << stepY << std::endl;

			double deltaLengthX = 0;
			double deltaLengthY = 0;

			double previousStartX = 0;
			double previousStartY = 0;
			std::cout << "step18" << std::endl;
			if (abs(dirX) > abs(dirY))
			{
				std::cout << "step19" << std::endl;
				double deltaDistX = sqrt(pow(dirX, 2) + pow(dirY, 2)) * ceil_w;
				double deltaDistY = sqrt(pow((lineLengthY / cooficient), 2) + pow((lineLengthX / cooficient), 2));

				double stepsYlength = ceil_h;
				deltaLengthY += deltaDistY;

				std::cout << "step20" << std::endl;
				while ((lineStartX != endPosX) && (lineStartY != endPosY))
				{
					if (int(deltaLengthY) < int(deltaLengthX))
					{
						std::cout << "step20.1" << std::endl;
						squareMapArray[int(previousStartY / ceil_h)][int(previousStartX / ceil_w)].linesList.pop_back();
						sectorLines_pointers.pop_back();

						std::cout << "step20.1.1" << std::endl;
						lineEndX = sqrt(pow(deltaLengthY, 2) - pow(stepsYlength, 2));
						lineEndY = stepsYlength;

						std::cout << "step20.1.2" << std::endl;
						lineEndX = startPosX + ((stepX / abs(stepX)) * lineEndX);
						lineEndY = startPosY + ((stepY / abs(stepY)) * lineEndY);

						std::cout << "lineEndY: " << lineEndY << std::endl;
						std::cout << "lineEndX: " << lineEndX << std::endl;

						std::cout << "step20.1.3" << std::endl;
						LineMap* newLine1 = new LineMap(previousStartX, previousStartY, lineEndX, lineEndY, dirX, dirY);
						squareMapArray[int(previousStartY / ceil_h)][int(previousStartX / ceil_w)].isEmpty = false;
						squareMapArray[int(previousStartY / ceil_h)][int(previousStartX / ceil_w)].linesList.push_back(newLine1);

						std::cout << "step20.1.4" << std::endl;
						sectorLines_pointers.push_back(newLine1);

						std::cout << "step20.1.5" << std::endl;
						LineMap* newLine2 = new LineMap(lineEndX, lineEndY, lineStartX, lineStartY, dirX, dirY);
						squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].isEmpty = false;
						squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].linesList.push_back(newLine2);

						std::cout << "step20.1.6" << std::endl;
						sectorLines_pointers.push_back(newLine2);

						stepsYlength += ceil_h;
						deltaLengthY += deltaDistY;
					}
					else if (int(deltaLengthY) > int(deltaLengthX))
					{
						std::cout << "step20.2" << std::endl;
						deltaLengthX += deltaDistX;

						std::cout << "step20.2.1" << std::endl;
						previousStartX = lineStartX;
						previousStartY = lineStartY;

						std::cout << "step20.2.2" << std::endl;
						std::cout << "dirX: " << dirX << std::endl;
						std::cout << "dirY: " << dirY << std::endl;
						std::cout << "stepY: " << stepY << std::endl;
						std::cout << "stepX: " << stepX << std::endl;
						std::cout << "lineStartY: " << lineStartY << std::endl;
						std::cout << "lineStartX: " << lineStartX << std::endl;
						lineEndX = lineStartX + stepX;
						lineEndY = lineStartY + stepY;

						std::cout << "step20.2.3" << std::endl;

						LineMap* newLine = new LineMap(lineStartX, lineStartY, lineEndX, lineEndY, dirX, dirY);
						std::cout << "step20.2.3.1" << std::endl;
						std::cout << "row: " << int(lineEndY / ceil_h) << std::endl;
						std::cout << "col: " << int(lineEndX / ceil_w) << std::endl;
						std::cout << "lineEndY: " << lineEndY << std::endl;
						std::cout << "lineEndX: " << lineEndX << std::endl;
						squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].isEmpty = false;
						std::cout << "step20.2.3.2" << std::endl;
						std::cout << "square capacity: " << squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].linesList.size() << std::endl;
						squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].linesList.push_back(newLine);

						std::cout << "step20.2.4" << std::endl;
						sectorLines_pointers.push_back(newLine);

						std::cout << "step20.2.5" << std::endl;
						lineStartX = lineEndX;
						lineStartY = lineEndY;
					}
					else if (int(deltaLengthY) == int(deltaLengthX))
					{
						std::cout << "step20.3" << std::endl;
						stepsYlength += ceil_h;
						deltaLengthY += deltaDistY;
					}
				}
			}
			else if (abs(dirX) < abs(dirY))
			{
				std::cout << "step21" << std::endl;
				double deltaDistX = sqrt(pow((lineLengthX / cooficient), 2) + pow((lineLengthY / cooficient), 2));
				double deltaDistY = sqrt(pow(dirY, 2) + pow(dirX, 2)) * ceil_h;

				double stepsXlength = ceil_w;
				deltaLengthX += deltaDistX;

				while ((lineStartX != endPosX) && (lineStartY != endPosY))
				{
					if (int(deltaLengthX) < int(deltaLengthY))
					{
						squareMapArray[int(previousStartY / ceil_h)][int(previousStartX / ceil_w)].linesList.pop_back();
						sectorLines_pointers.pop_back();

						lineEndX = stepsXlength;
						lineEndY = sqrt(pow(deltaLengthX, 2) - pow(stepsXlength, 2));

						lineEndX = startPosX + ((stepX / abs(stepX)) * lineEndX);
						lineEndY = startPosY + ((stepY / abs(stepY)) * lineEndY);

						LineMap* newLine1 = new LineMap(previousStartX, previousStartY, lineEndX, lineEndY, dirX, dirY);
						squareMapArray[int(previousStartY / ceil_h)][int(previousStartX / ceil_w)].isEmpty = false;
						squareMapArray[int(previousStartY / ceil_h)][int(previousStartX / ceil_w)].linesList.push_back(newLine1);

						sectorLines_pointers.push_back(newLine1);

						LineMap* newLine2 = new LineMap(lineEndX, lineEndY, lineStartX, lineStartY, dirX, dirY);
						squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].isEmpty = false;
						squareMapArray[int(lineEndY / ceil_h)][int(lineEndX / ceil_w)].linesList.push_back(newLine2);

						sectorLines_pointers.push_back(newLine2);

						stepsXlength += ceil_w;
						deltaLengthX += deltaDistX;
					}
					else if (int(deltaLengthX) > int(deltaLengthY))
					{
						deltaLengthY += deltaDistY;

						previousStartY = lineStartY;
						previousStartX = lineStartX;

						lineEndX = lineStartX + stepX;
						lineEndY = lineStartY + stepY;

						LineMap* newLine = new LineMap(lineStartX, lineStartY, lineEndX, lineEndY, dirX, dirY);
						squareMapArray[int(lineStartY / ceil_h)][int(lineStartX / ceil_w)].isEmpty = false;
						squareMapArray[int(lineStartY / ceil_h)][int(lineStartX / ceil_w)].linesList.push_back(newLine);

						sectorLines_pointers.push_back(newLine);

						lineStartX = lineEndX;
						lineStartY = lineEndY;
					}
					else if (int(deltaLengthX) == int(deltaLengthY))
					{
						stepsXlength += ceil_w;
						deltaLengthX += deltaDistX;
					}
				}
			}
			else if(abs(dirX) == abs(dirY))
			{
				std::cout << "step22" << std::endl;
				std::cout << "lineStartX: " << lineStartX << std::endl;
				std::cout << "lineStartY: " << lineStartY << std::endl;
				std::cout << "lineEndX: " << endPosX << std::endl;
				std::cout << "lineEndY: " << endPosY << std::endl;

				while ((lineStartX != endPosX) && (lineStartY != endPosY))
				{
					lineEndX = lineStartX + stepX;
					lineEndY = lineStartY + stepY;

					LineMap* newLine = new LineMap(lineStartX, lineStartY, lineEndX, lineEndY, dirX, dirY);
					squareMapArray[int(lineStartY / ceil_h)][int(lineStartX / ceil_w)].isEmpty = false;
					squareMapArray[int(lineStartY / ceil_h)][int(lineStartX / ceil_w)].linesList.push_back(newLine);

					std::cout << "row: " << int(lineStartY / ceil_h) << std::endl;
					std::cout << "col: " << int(lineStartX / ceil_w) << std::endl;
					std::cout << "lineStartX: " << lineEndX << std::endl;
					std::cout << "lineStartY: " << lineEndY << std::endl;

					sectorLines_pointers.push_back(newLine);

					lineStartX = lineEndX;
					lineStartY = lineEndY;
				}
			}
		}
		std::cout << "step23" << std::endl;
		//очищаем занятую перевыделенную память
		clearUnusedMemmory_simpleArr(sectorLines_pointers);

		//добавляем вектор и связанный с ним маленькие вектора в массив 
		VectorToLines* newVector = new VectorToLines(sectorPaths[i], sectorLines_pointers);
		newVectorLines.push_back(*newVector);
	}
	std::cout << "step24" << std::endl;
	//очищаем занятую перевыделенную память
	//clearUnusedMemmory_simpleArr(newVectorLines);
	sectorLine_Pointers.push_back(newVectorLines);
}

void Pseudo3D::performBSP(std::vector<std::vector<LineMap>>& Sectors)
{
	//копируем текущий массив векторов в прошлый
	previousVectorsMapArray = vectorsMapArray;

	//временный массив для сохранения секторов
	std::vector<std::vector<int>> sectorsArray;
	std::vector<std::vector<LineMap>> sectorPaths;

	//заводим массив содержащий path ID, где будут содержаться ID которые не будут проверяться
	std::vector<int> notCheckPaths;

	int currentPathId = 0;

	//массив для сохранения найденных векторров выходящих из одной общей точки
	std::vector<LineMap> foundLines;
	std::vector<LineMap> lines;

	//каждый раз после добавления вектора удаляем его из прошлого массива, сокращение итераций цикла
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
				std::vector<LineMap>& tempLineMapComponent = sectorPaths[j];

				std::cout << "CHECKING PATH #" << j << std::endl;
				std::cout << std::endl;

				LineMap previousLineMapComponent = *new LineMap();

				//проверяем все дотупные вектора, имеют ли они общее начало с выбранным вектором
				for (int i = 0; i < vectorsMapArray.size(); i++)
				{
					if (vectorsMapArray.size() > 0)
					{
						//проверка если вектора совпадают в концах
						if (((vectorsMapArray[i].endPosX == tempLineMapComponent[tempLineMapComponent.size() - 1].endPosX) &&
							(vectorsMapArray[i].endPosY == tempLineMapComponent[tempLineMapComponent.size() - 1].endPosY)))
						{
							//разворачиваем вектор, чтобы в пути каждый вектор следовал из другого
							double tempEndPosX = vectorsMapArray[i].startPosX;
							double tempEndPosY = vectorsMapArray[i].startPosY;

							std::cout << "dirX before: " << vectorsMapArray[i].dirX << std::endl;
							std::cout << "dirY before: " << vectorsMapArray[i].dirY << std::endl;

							vectorsMapArray[i].startPosX = vectorsMapArray[i].endPosX;
							vectorsMapArray[i].startPosY = vectorsMapArray[i].endPosY;
							vectorsMapArray[i].endPosX = tempEndPosX;
							vectorsMapArray[i].endPosY = tempEndPosY;
							vectorsMapArray[i].dirX = -(vectorsMapArray[i].dirX);
							vectorsMapArray[i].dirY = -(vectorsMapArray[i].dirY);

							std::cout << "adding reversed vector start pos: " << vectorsMapArray[i].startPosX << ", " << vectorsMapArray[i].startPosY << std::endl;
							std::cout << "dirX after: " << vectorsMapArray[i].dirX << std::endl;
							std::cout << "dirY after: " << vectorsMapArray[i].dirY << std::endl;

							//добавляем в массив foundLines найденный вектор, и удаляем из прошлого массива
							foundLines.push_back(vectorsMapArray[i]);
							vectorsMapArray.erase(vectorsMapArray.begin() + i);
							i = -1;
						}
						else if (((vectorsMapArray[i].startPosX == tempLineMapComponent[tempLineMapComponent.size() - 1].endPosX)
							&& (vectorsMapArray[i].startPosY == tempLineMapComponent[tempLineMapComponent.size() - 1].endPosY)))
						{
							std::cout << "adding vector start pos: " << vectorsMapArray[i].startPosX << ", " << vectorsMapArray[i].startPosY << std::endl;

							//добавляем в массив foundLines найденный вектор, и удаляем из прошлого массива
							foundLines.push_back(vectorsMapArray[i]);
							vectorsMapArray.erase(vectorsMapArray.begin() + i);
							i = -1;
						}
					}
				}

				//если кол-во найденных векторов, чье начало
				if (foundLines.size() > 1)
				{
					std::cout << "-----------------------------------------" << std::endl;
					std::cout << "creating new paths cuz amount of foundLines == " << foundLines.size() << std::endl;

					//вызов метода для сортировки массива
					std::cout << "sorting the array" << std::endl;
					sortCosign(foundLines, 0, foundLines.size() - 1, tempLineMapComponent[tempLineMapComponent.size() - 1]);

					//заведем временный массив для сохранения считанных path
					//необходимо для того чтобы потом добавить в массив notVheckPaths path'ы которые не надо проверять т.к они содержат только один вектор 
					std::vector<int> tempNotToCheckPaths;
					std::vector<int> createdPaths;

					//добавляем найденные вектора
					for (int i = 0; i < foundLines.size(); i++)
					{
						//увеличиваем на 1 при создании нового path
						currentPathId += 1;

						std::string pathName = "path" + std::to_string(currentPathId);
						std::cout << pathName << std::endl;

						std::vector<LineMap> newPath;
						newPath.push_back(foundLines[i]);

						sectorPaths.push_back(newPath);

						tempNotToCheckPaths.push_back(currentPathId);
						createdPaths.push_back(currentPathId);

						std::cout << "path" << currentPathId << " has been created" << std::endl;
					}
					//делаем проверку, являяется ли конечная точка добавленного вектора начальной точкой для более одного вектора
					currentPathId = checkAmountOfPaths(sectorPaths, foundLines, tempNotToCheckPaths, currentPathId, createdPaths, notCheckPaths, sectorsArray);

					std::cout << "paths indexes that aint gonna be checked: ";
					for (int i = 0; i < notCheckPaths.size(); i++)
					{
						std::cout << notCheckPaths[i] << ", ";
					}
					std::cout << std::endl;

					std::cout << "-----------------------------------------" << std::endl;
					//очистка массива и переход на следующий путь
					foundLines.clear();
					j++;
				}
				else
				{
					//делаем проверку пути, является ли путь вхождением в другой путь
					int pathId = checkPath(sectorPaths, tempLineMapComponent[tempLineMapComponent.size() - 1]);

					std::cout << "found path: " << pathId << std::endl;

					//-1 - значит нет такого пути в который входил бы текущий путь
					if (pathId == -1)
					{
						//проверка является ли массив найденных векторов пустым
						if (foundLines.size() == 1)
						{
							std::cout << "----------------------" << std::endl;
							std::cout << "adding new vector to path" << j << std::endl;
							std::cout << "----------------------" << std::endl;

							sectorPaths[j].push_back(foundLines[0]);
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
					foundLines.clear();
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

				std::vector<LineMap> lines;
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
		std::vector<LineMap> newSector;

		std::cout << "--------------------" << std::endl;
		std::cout << "sector #" << i << std::endl;
		for (int j = 0; j < pathIndexes.size(); j++)
		{
			std::cout << "path id: " << pathIndexes[j] << std::endl;
			newSector.insert(newSector.end(), sectorPaths[pathIndexes[j]].begin(), sectorPaths[pathIndexes[j]].end());
		}
		std::cout << "--------------------" << std::endl;
		clearUnusedMemmory_simpleArr(newSector);
		Sectors.push_back(newSector);
	}

	vectorsMapArray = previousVectorsMapArray;
}

int Pseudo3D::checkPath(std::vector<std::vector<LineMap>>& sectorPaths, LineMap& tempVector)
{
	for (int i = 0; i < sectorPaths.size(); i++)
	{
		std::vector<LineMap>& pathFirst = sectorPaths[i];

		std::cout << "-----------------------------------------------------------------------------------------" << std::endl;

		std::cout << "path " << i << " vector startPosX, startPosY: " << pathFirst[0].startPosX << " ," << pathFirst[0].startPosY << std::endl;
		std::cout << "path " << i << "vector endPosX, endPosY: " << pathFirst[pathFirst.size() - 1].endPosX << " ," << pathFirst[pathFirst.size() - 1].endPosY << std::endl;
		std::cout << "current vector startPosX, startPosY: " << tempVector.startPosX << " ," << tempVector.startPosY << std::endl;
		std::cout << "current vector endPosX, endPosY: " << tempVector.endPosX << " ," << tempVector.endPosY << std::endl;

		std::cout << "-----------------------------------------------------------------------------------------" << std::endl;

		if ((pathFirst[pathFirst.size() - 1] != tempVector) && (pathFirst[0] != tempVector))
		{
			if ((pathFirst[0].startPosX == tempVector.endPosX) && (pathFirst[0].startPosY == tempVector.endPosY))
			{
				return i;
			}
			else if (((pathFirst[pathFirst.size() - 1].endPosX == tempVector.endPosX) && (pathFirst[pathFirst.size() - 1].endPosY == tempVector.endPosY)))
			{
				return i;
			}
		}
		else if ((pathFirst[pathFirst.size() - 1] == tempVector) && (pathFirst[0] != tempVector))
		{
			if ((pathFirst[0].startPosX == tempVector.endPosX) && (pathFirst[0].startPosY == tempVector.endPosY))
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
		if ((pathEndPositions[i].endPathX == path.endPathX)
			&& (pathEndPositions[i].endPathY == path.endPathY))
		{
			return i;
		}
	}

	return -1;
}

//ПЕРЕДЕЛЫВАТЬ!!!
int Pseudo3D::checkAmountOfPaths(std::vector<std::vector<LineMap>>& sectorPaths, std::vector<LineMap>& lineArr, std::vector<int>& tempNotToCheckPaths, int currPathId, std::vector<int>& recentPaths, std::vector<int>& notCheckPaths, std::vector<std::vector<int>>& sectorsArray)
{
	for (int j = 0; j < lineArr.size(); j++)
	{
		//проверям нужно ли нам проверять этот path или нет
		if (shouldCheck(recentPaths[j], notCheckPaths))
		{
			std::vector<LineMap>& tempPathArr = sectorPaths[recentPaths[j]];
			int pathId = checkPath(sectorPaths, tempPathArr[tempPathArr.size() - 1]);

			//заводим массив найденных линий
			std::vector<LineMap> foundLines;

			std::cout << "checking single path" << recentPaths[j] << std::endl;
			for (int i = 0; i < vectorsMapArray.size(); i++)
			{
				if (((vectorsMapArray[i].endPosX == lineArr[j].endPosX) &&
					(vectorsMapArray[i].endPosY == lineArr[j].endPosY)))
				{
					double tempEndPosX = vectorsMapArray[i].startPosX;
					double tempEndPosY = vectorsMapArray[i].startPosY;

					vectorsMapArray[i].startPosX = vectorsMapArray[i].endPosX;
					vectorsMapArray[i].startPosY = vectorsMapArray[i].endPosY;
					vectorsMapArray[i].endPosX = tempEndPosX;
					vectorsMapArray[i].endPosY = tempEndPosY;
					vectorsMapArray[i].dirX = -(vectorsMapArray[i].dirX);
					vectorsMapArray[i].dirY = -(vectorsMapArray[i].dirY);

					foundLines.push_back(vectorsMapArray[i]);

					std::cout << "adding new line reversed" << std::endl;
					std::cout << "startPosx: " << vectorsMapArray[i].startPosX << std::endl;
					std::cout << "startPosy: " << vectorsMapArray[i].startPosY << std::endl;
					std::cout << "endPosx: " << vectorsMapArray[i].endPosX << std::endl;
					std::cout << "endPosy: " << vectorsMapArray[i].endPosY << std::endl;

					vectorsMapArray.erase(vectorsMapArray.begin() + i);
					i -= 1;
				}
				else if (((vectorsMapArray[i].startPosX == lineArr[j].endPosX)
					&& (vectorsMapArray[i].startPosY == lineArr[j].endPosY)))
				{
					foundLines.push_back(vectorsMapArray[i]);

					std::cout << "adding new line" << std::endl;
					std::cout << "startPosx: " << vectorsMapArray[i].startPosX << std::endl;
					std::cout << "startPosy: " << vectorsMapArray[i].startPosY << std::endl;
					std::cout << "endPosx: " << vectorsMapArray[i].endPosX << std::endl;
					std::cout << "endPosy: " << vectorsMapArray[i].endPosY << std::endl;


					vectorsMapArray.erase(vectorsMapArray.begin() + i);
					i -= 1;
				}
			}
			if (foundLines.size() > 1)
			{
				sortCosign(foundLines, 0, foundLines.size() - 1, lineArr[j]);

				//снова заводим массив индексов путей которые не надо проверять т.к они содержат по одному вектору
				std::vector<int> tempNotCheckRepeat;
				std::vector<int> createdPaths;

				for (int i = 0; i < foundLines.size(); i++)
				{
					currPathId += 1;

					std::vector<LineMap> newPath;
					newPath.push_back(foundLines[i]);

					sectorPaths.push_back(newPath);

					std::cout << "size: " << foundLines.size() << std::endl;
					std::cout << "path" << currPathId << " has been created" << std::endl;
					tempNotCheckRepeat.push_back(currPathId);
					createdPaths.push_back(currPathId);
				}

				//делаем проверку, являяется ли конечная точка добавленного вектора начальной точкой для более одного вектора
				currPathId = checkAmountOfPaths(sectorPaths, foundLines, tempNotCheckRepeat, currPathId, createdPaths, notCheckPaths, sectorsArray);
				foundLines.clear();
			}
			else if (foundLines.size() == 1)
			{
				if (pathId != -1)
				{
					std::vector<int> tempNotCheckRepeat;
					std::vector<int> createdPaths;

					currPathId += 1;

					std::vector<LineMap> newPath;
					newPath.push_back(foundLines[0]);

					sectorPaths.push_back(newPath);

					std::cout << "size: " << foundLines.size() << std::endl;
					std::cout << "path" << currPathId << " has been created" << std::endl;
					tempNotCheckRepeat.push_back(currPathId);
					createdPaths.push_back(currPathId);

					currPathId = checkAmountOfPaths(sectorPaths, foundLines, tempNotCheckRepeat, currPathId, createdPaths, notCheckPaths, sectorsArray);
					foundLines.clear();
				}
				else
				{
					std::cout << "path" << recentPaths[j] << " - vector hasnt been added cuz its only one" << std::endl;

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
				std::cout << "sector between path" << pathId << " and path" << recentPaths[j] << std::endl;

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
				std::cout << "-------------------------------" << std::endl;
				std::cout << "shouldn't check path" << notCheckPaths[i] << std::endl;
				std::cout << "-------------------------------" << std::endl;
				notCheckPaths.erase(notCheckPaths.begin() + i);
				return false;
			}
		}
	}
	return true;
}

void Pseudo3D::createSector(int currentPathId, int foundPathId, std::vector<int>& notCheckPaths, std::vector<std::vector<LineMap>>& sectorPaths, std::vector<std::vector<int>>& sectorsArray)
{
	//добавляем найденный путь в пути которые НЕ будем проверять 
	notCheckPaths.push_back(foundPathId);

	std::cout << "----------------------" << std::endl;
	//иначе мы получаем id путя в который произошло вхождение
	std::cout << "creating new sector" << std::endl;

	//массив сектора, состоящий из PATH
	std::vector<int> newSector;

	//получаем по ссылке путь чей id совпадает с найденным
	std::vector<LineMap>& foundPathArr = sectorPaths[foundPathId];
	std::vector<LineMap>& currPathArr = sectorPaths[currentPathId];

	//проверяем, если конец первого вектора в начало другого
	if ((currPathArr[currPathArr.size() - 1].endPosX == foundPathArr[0].startPosX) &&
		(currPathArr[currPathArr.size() - 1].endPosY == foundPathArr[0].startPosY))
	{
		std::cout << "end to start" << std::endl;
		//создаем массив, каждый элемент массива является массивом путей
		std::vector<std::vector<int>> sectorLine;

		std::vector<int> newSectorLine;
		newSectorLine.push_back(currentPathId);
		sectorLine.push_back(newSectorLine);

		std::vector<int> notCheckSectorLines;

		bool sectorIsClosed = false;

		while (!sectorIsClosed)
		{
			std::cout << "step 1" << std::endl;
			std::vector<std::vector<int>> addLaterPaths_Arr;

			for (int i = 0; i < sectorLine.size(); i++)
			{
				//проверка надо ли проверять этот массив
				if (shouldCheckSectorLine(i, notCheckSectorLines))
				{
					std::vector<int>& sectorLine_Paths = sectorLine[i];
					std::vector<int> foundVectors;

					std::vector<LineMap>& tempPath = sectorPaths[sectorLine_Paths[sectorLine_Paths.size() - 1]];

					for (int j = 0; j < sectorPaths.size(); j++)
					{
						std::vector<LineMap>& sectorPath = sectorPaths[j];

						if ((tempPath[0].startPosX == sectorPath[sectorPath.size() - 1].endPosX) &&
							(tempPath[0].startPosY == sectorPath[sectorPath.size() - 1].endPosY))
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
			std::cout << "step 2" << std::endl;
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
						//КОСТЫЛЬ, НУ И ХУЙ С НИМ
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
		std::cout << "end to end" << std::endl;

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

					std::cout << "line #1" << std::endl;
					std::cout << "checking " << sectorLine_1_Paths[sectorLine_1_Paths.size() - 1] << std::endl;

					std::vector<int> foundVectors;

					std::vector<LineMap>& tempPath = sectorPaths[sectorLine_1_Paths[sectorLine_1_Paths.size() - 1]];

					for (int j = 0; j < sectorPaths.size(); j++)
					{
						std::vector<LineMap>& sectorPath = sectorPaths[j];

						if ((tempPath[0].startPosX == sectorPath[sectorPath.size() - 1].endPosX) &&
							(tempPath[0].startPosY == sectorPath[sectorPath.size() - 1].endPosY))
						{
							std::cout << "found path: " << "path" + std::to_string(j) << std::endl;
							foundVectors.push_back(j);
						}
					}
					//если найдено более одного path
					if (foundVectors.size() > 1)
					{
						std::cout << "amount of found vectors to a path: " << foundVectors.size() << std::endl;
						std::cout << "found path: " << foundVectors[0] << std::endl;
						for (int n = 1; n < foundVectors.size(); n++)
						{
							std::cout << "found path: " << foundVectors[n] << std::endl;
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

					std::cout << "line #2" << std::endl;
					std::cout << "checking " << sectorLine_2_Paths[sectorLine_2_Paths.size() - 1] << std::endl;

					std::vector<LineMap>& tempPath = sectorPaths[sectorLine_2_Paths[sectorLine_2_Paths.size() - 1]];

					for (int j = 0; j < sectorPaths.size(); j++)
					{
						std::vector<LineMap>& sectorPath = sectorPaths[j];

						if ((tempPath[0].startPosX == sectorPath[sectorPath.size() - 1].endPosX) &&
							(tempPath[0].startPosY == sectorPath[sectorPath.size() - 1].endPosY))
						{
							std::cout << "found path: " << "path" + std::to_string(j) << std::endl;
							foundVectors.push_back(j);
						}
					}

					//если найдено более одного path
					if (foundVectors.size() > 1)
					{
						for (int n = 1; n < foundVectors.size(); n++)
						{
							std::cout << "found path: " << foundVectors[n] << std::endl;
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

	std::cout << "------------------------" << std::endl;
	std::cout << "created sector paths: " << std::endl;
	for (int i = 0; i < newSector.size(); i++)
	{
		std::cout << newSector[i] << ", ";
	}
	std::cout << std::endl;
	std::cout << "------------------------" << std::endl;
}

int Pseudo3D::partitionCosign(std::vector<LineMap>& arr, int low, int high, LineMap& currentLine)
{
	//устанавливаем опорный элемент
	double pivot = countCosign(currentLine, arr[high]);
	int i = low - 1;

	for (int j = low; j <= high - 1; j++)
	{
		if (countCosign(currentLine, arr[j]) >= pivot)
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

void Pseudo3D::sortCosign(std::vector<LineMap>& arr, int low, int high, LineMap& line)
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

	std::cout << std::endl;
	std::cout << "cosign: " << cosign << std::endl;

	return cosign;
}

void Pseudo3D::sortPaths(std::vector<std::vector<int>>& pathsArr, int low, int high, int aimPath, std::vector<std::vector<LineMap>>& sectorPaths)
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

int Pseudo3D::partitionPaths(std::vector<std::vector<int>>& pathsArr, int low, int high, int aimPath, std::vector<std::vector<LineMap>>& sectorPaths)
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

double Pseudo3D::countPathDist(int pathStart, int pathEnd, std::vector<std::vector<LineMap>>& sectorPaths)
{
	std::vector<LineMap>& startLinesArr = sectorPaths[pathStart];
	std::vector<LineMap>& endLinesArr = sectorPaths[pathEnd];

	double startX = startLinesArr[startLinesArr.size() - 1].endPosX;
	double startY = startLinesArr[startLinesArr.size() - 1].endPosY;

	double endX = endLinesArr[0].startPosX;
	double endY = endLinesArr[0].startPosY;

	double distance = sqrt(pow((startX - endX), 2) + pow((startY - endY), 2));

	return distance;
}

Pseudo3D::~Pseudo3D()
{
	clearSquareMapArray();
}