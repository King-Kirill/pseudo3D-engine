#include "MapDraw.h"
#include "SquareMap.h"
#include "ConstParams.h"
#include "TempParams.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "LineMap.h"
#include "PathEndPosition.h"
#include <map>
#include <string>

MapDraw::MapDraw(SDL_Renderer*& renderer)
{
	this->renderer = renderer;
}

MapDraw::~MapDraw()
{
    delete[] gridDotsArray;
}

void MapDraw::drawMap()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    int amountOfDotsHeight = img_h / ceil_h;
    int amountOfDotsWidth = (img_w - dev_zone_w) / ceil_w;

    if (gridDotsArray == NULL)
    {
        gridCols = amountOfDotsHeight;
        gridRows = amountOfDotsWidth;

        gridDotsArray = new GridDots * [amountOfDotsHeight];

        for (int i = 0; i < amountOfDotsHeight; i++)
        {
            gridDotsArray[i] = new GridDots[amountOfDotsWidth];

            for (int j = 0; j < amountOfDotsWidth; j++)
            {
                int squarePosX = 0;
                int squarePosY = 0;
                int squareRadius = 0;

                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                drawCircle(j * ceil_w, i * ceil_h, 2, squarePosX, squarePosY, squareRadius);

                GridDots* newDot = new GridDots((j * ceil_w), (i * ceil_h), squarePosX, squarePosY, squareRadius);
                gridDotsArray[i][j] = *newDot;
            }
        }
    }
    else
    {
        for (int i = 0; i < amountOfDotsHeight; i++)
        {
            for (int j = 0; j < amountOfDotsWidth; j++)
            {
                int squarePosX = 0;
                int squarePosY = 0;
                int squareRadius = 0;

                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                drawCircle(j * ceil_w, i * ceil_h, 2, squarePosX, squarePosY, squareRadius);
            }
        }

        drawMapLines(amountOfDotsWidth - 1, amountOfDotsHeight - 1);
    }
}

void MapDraw::drawCircle(int centerX, int centerY, int radius, int & squarePosX, int & squarePosY, int & squareRadius)
{
    //dots map drawing
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }

    int rectRadius = radius * 3;
    squarePosX = centerX - rectRadius; // x - ordinate movement
    squarePosY = centerY - rectRadius; // y - absciss movement

    squareRadius = rectRadius * 2; // multiply to 2 cuz till now we used a half of a ceil
}

void MapDraw::drawMapLines(int amountOfSquaresWidth, int amountOfSquaresHeight)
{
    for (int i = 0; i < vectorsMapArray.size(); i++)
    {
        drawLine(vectorsMapArray[i].startPosX, vectorsMapArray[i].startPosY, vectorsMapArray[i].endPosX, vectorsMapArray[i].endPosY, vectorsMapArray[i].lineColorR, vectorsMapArray[i].lineColorG, vectorsMapArray[i].lineColorB);
    }
}

void MapDraw::drawLine(double startX, double startY, double endX, double endY, int colorRed, int colorGreen, int colorBlue)
{
    SDL_SetRenderDrawColor(renderer, colorRed, colorGreen, colorBlue, 255);
    SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
}