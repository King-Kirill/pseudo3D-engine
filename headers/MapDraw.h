#ifndef MapDraw_hpp
#define MapDraw_hpp

#include <iostream>
#include <cmath>
#include <SDL.h>
#include "SDL2_gfxPrimitives.h"
#include "GameCharacter.h"
#include <string>

class MapDraw
{
private:
	SDL_Renderer* renderer;

public:
	MapDraw(SDL_Renderer*& renderer);
	~MapDraw();

	void drawMap();
	void drawMapSmall();
	void drawGame(std::string selected_engine_window);
	void drawMapEdges();
	void drawMapEdgesSmall();
	void drawCircle(double centerX, double centerY, double radius, int& squarePosX, int& squarePosY, int& squareRadius);
	void drawLine(double startX, double startY, double endX, double endY, int colorRed, int colorGreen, int colorBlue);
	void drawCharacters();
	void drawMapLines(int amountOfSquaresWidth, int amountOfSquaresHeight);

	void createCharacter(std::string characterName, int redColor, int greenColor, int blueColor, double characterPosX, double characterPosY, double dirX, double dirY);
	void changeCharColor(int redColor, int greenColor, int blueColor, std::string characterName);
	void changeCharName(std::string characterName, std::string newCharacterName);
	void changeCharPos(double characterPosX, double characterPosY, std::string characterName);
	void changeCharRole(std::string newRole, std::string characterName);
	bool checkCharName(std::string Name);
	bool checkCharRole(std::string Role);
	void countMiniMapCenterDiffer();
	GameCharacter* getGameCharacter(double posX, double posY);
};

#endif /* MapDraw_hpp */