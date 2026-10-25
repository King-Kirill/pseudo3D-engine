#ifndef GameSDL_hpp
#define GameSDL_hpp
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "SDL.h"
#include <string>
#include <iostream>
#include "MapDraw.h"
#include "PathEndPosition.h"
#include "TempParams.h"
#include "ConstParams.h"
#include "LineMap.h"
#include "Pseudo3D.h"

class Game 
{
public: 
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void cleanInputBuffer();
	void clean();
	void setDotPosition(int &x, int &y);
	void addNewLine();
	void zooming(bool flag, int mouseX, int mouseY);
	void selectItem();
	
	static void getNewMap_coroutine(double currentX, double currentY, double &startPosX, double &startPosY, SDL_Window*& window);
	static void getPlayerCoordinates_coroutine(GameCharacter* character);
	void normalizeMap();
	void getPlayerSpeedRotation_zoomIn();
	void getPlayerSpeedRotation_zoomOut();

	bool running();
	MapDraw* map;
	Pseudo3D* mainScene;

private:
	bool isRunning;
	bool startPosSet;
	bool selectAreaIsDown;

	std::vector<int> vectorIndexes;

	double startPosX;
	double startPosY;

	double endPosX;
	double endPosY;

	ImGuiIO io;
	bool show_demo_window;
	ImVec4 clear_color;
	ImVec2 lineOptionsWindowPos;
	ImVec2 characterOptionsWindowPos;

	SDL_Window* window;
	SDL_Renderer* renderer;
};

#endif /* GameSDL_hpp */