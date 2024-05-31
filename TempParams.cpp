#include "TempParams.h"

GridDots** gridDotsArray = NULL;
SquareMap** squareMapArray = NULL;

std::vector<std::vector<VectorToLines>> sectorLine_Pointers;

std::vector<LineMap> vectorsMapArray;
std::vector<LineMap> previousVectorsMapArray;

int gridRows = 0;
int gridCols = 0;

int currLineColorR = 255;
int currLineColorG = 255;
int currLineColorB = 255;

bool mouseButtonIsPressed = false;

void clearSquareMapArray()
{
	if ((squareMapArray != NULL) && gridCols != 0 && gridRows != 0)
	{
		for (int i = 0; i < gridCols - 1; ++i) {
			delete[] squareMapArray[i];
		}
		delete[] squareMapArray;
	}
	else
	{
		std::cout << "ERROR: not initialized array" << std::endl;
	}
}

void initSquareMapArray()
{
    int height = gridCols;
    int width = gridRows;

    if (height <= 0 || width <= 0)
    {
        std::cout << "ERROR: sizes of squareMapArray has not been set" << std::endl;
    }
    else
    {
        squareMapArray = new SquareMap * [height - 1];

        for (int i = 0; i < height - 1; i++)
        {
            squareMapArray[i] = new SquareMap[width - 1];

            for (int j = 0; j < width - 1; j++)
            {
                SquareMap* newSquare = new SquareMap();
                squareMapArray[i][j] = *newSquare;
            }
            
        }
    }
}

