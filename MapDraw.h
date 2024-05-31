#ifndef MapDraw_hpp
#define MapDraw_hpp

#include <iostream>
#include <cmath>
#include <SDL.h>
#include <string>

class MapDraw
{
private:
	SDL_Renderer* renderer;

public:
	MapDraw(SDL_Renderer*& renderer);
	~MapDraw();

	void drawMap();
	void drawCircle(int centerX, int centerY, int radius, int& squarePosX, int& squarePosY, int& squareRadius);
	void drawLine(double startX, double startY, double endX, double endY, int colorRed, int colorGreen, int colorBlue);
	void drawMapLines(int amountOfSquaresWidth, int amountOfSquaresHeight);
};

#endif /* MapDraw_hpp */