#include "GameSDL.h"
#include <thread>
#include <chrono>

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	//initialization section
	mouseIsDown = false;
	startPosX = 0;
	startPosY = 0;
	endPosX = 0;
	endPosY = 0;

	lineOptionsWindowPos = ImVec2(50, 50);

	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Sybsystem initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer)
		{
			std::cout << "Renderer created" << std::endl;
		}

		//imgui initialization
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO(); (void)io;
		show_demo_window = true;
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);

		isRunning = true;
		this->map = new MapDraw(renderer);
	}
	else
	{
		isRunning = false;
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	ImGui_ImplSDL2_ProcessEvent(&event);

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	switch (event.type)
	{
	case(SDL_QUIT):
		isRunning = false;
		break;
	case(SDL_MOUSEBUTTONDOWN):
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			if (vectorIndexes.size() != 0)
			{
				vectorIndexes.clear();
			}

			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				startPosX = event.motion.x;
				startPosY = event.motion.y;
				endPosX = event.motion.x;
				endPosY = event.motion.y;

				selectAreaIsDown = true;
			}
			else
			{
				setDotPosition(event.motion.x, event.motion.y);
			}
		}
		break;
	case(SDL_MOUSEBUTTONUP):
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				endPosX = event.motion.x;
				endPosY = event.motion.y;

				selectAreaIsDown = false;

				selectItem();
			}
		}
		break;
	case(SDL_WINDOWEVENT_FOCUS_LOST):
		if (vectorIndexes.size() != 0)
		{
			vectorIndexes.clear();
		}
		break;
	case(SDL_MOUSEMOTION):
		if (mouseIsDown || selectAreaIsDown)
		{
			endPosX = event.motion.x;
			endPosY = event.motion.y;
		}
	default:
		break;
	}

	static float f = 0.0f;
	static int counter = 0;

	ImGui::SetNextWindowPos(ImVec2(img_w - dev_zone_w, 0));
	ImGui::SetNextWindowSize(ImVec2(dev_zone_w, img_h));
	ImGui::Begin("Dev zone", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Avg. fps: %.1f FPS (%.1f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Spacing();

	ImVec4 colorPortal = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::ColorButton("##colorButton", colorPortal, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		currLineColorR = 255;
		currLineColorG = 0;
		currLineColorB = 0;
	}
	ImGui::SameLine();
	ImGui::TextWrapped("- portal's color");
	ImGui::Spacing();

	ImVec4 colorAutoDoor = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);
	ImGui::ColorButton("##colorButton", colorAutoDoor, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		currLineColorR = 255;
		currLineColorG = 165;
		currLineColorB = 0;
	}
	ImGui::SameLine();
	ImGui::TextWrapped("- auto door's color");
	ImGui::Spacing();

	ImVec4 colorSelectArea = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImGui::ColorButton("##colorButton", colorSelectArea, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoPicker);
	ImGui::SameLine();
	ImGui::TextWrapped("- we do not recommend to use this color, to prevent errors with lines selection");
	ImGui::Spacing();

	float color[3] = { static_cast<float>(currLineColorR) / 255.0f, static_cast<float>(currLineColorG) / 255.0f, static_cast<float>(currLineColorB) / 255.0f };
	ImGui::TextWrapped("current line color: ");
	ImGui::ColorEdit3("", color);
	ImGui::Spacing();

	currLineColorR = color[0] * 255;
	currLineColorG = color[1] * 255;
	currLineColorB = color[2] * 255;

	ImGui::TextWrapped("Notice: please make sure to not use portal's color for usual walls, in the case there are not going to be any walls, if only transparent ones");
	ImGui::Spacing();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 250) * 0.5f);
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.9f);
	ImVec2 buttonSize(250, 50);

	if (ImGui::Button("PERFORM 3D", buttonSize)) 
	{
		if (previousVectorsMapArray != vectorsMapArray)
		{
			//очитска и инициализация массивов для перезаписи
			clearSquareMapArray();
			initSquareMapArray();
			clearVectorArray(sectorLine_Pointers);

			std::vector<std::vector<LineMap>> sectorsArr;
			std::cout << "size of the new arr: " << sectorsArr.size() << std::endl;
			
			mainScene->performBSP(sectorsArr);

			
			for (int i = 0; i < sectorsArr.size(); i++)
			{
				std::vector<LineMap>& tempArr = sectorsArr[i];
				if (tempArr.size() > 0)
				{
					mainScene->rastorization(tempArr);
				}
				else
				{
					std::cout << "the array is empty!!! BLYAT" << std::endl;
				}
			}
			
			//очищаем перевыделенную память
			clearUnusedMemmory_simpleArr(sectorLine_Pointers);
		}
	}
	ImGui::End();

	if (vectorIndexes.size() != 0)
	{
		ImGui::SetNextWindowSize(ImVec2(lineOptionsWin_w, lineOptionsWin_h));
		ImGui::SetNextWindowPos(lineOptionsWindowPos);

		ImGui::Begin("Change's zone", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			lineOptionsWindowPos = ImGui::GetIO().MousePos;

		ImVec2 smallButton_1_Size(140, 25);

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 140) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (ImGui::Button("CLOSE TAB", smallButton_1_Size)) {
			vectorIndexes.clear();
		}

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 140) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.45f);

		if (ImGui::Button("DELETE", smallButton_1_Size)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray.erase(vectorsMapArray.begin() + vectorIndexes[i]);

				for (int j = 0; j < vectorIndexes.size(); j++)
				{
					vectorIndexes[j] -= 1;
				}
			}
			vectorIndexes.clear();
		}

		ImGui::Spacing();

		if (ImGui::ArrowButton("##Left", ImGuiDir_Left)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]].endPosX -= ceil_w;
				vectorsMapArray[vectorIndexes[i]].startPosX -= ceil_w;
			}
		}
		ImGui::SameLine(35);
		if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]].endPosX += ceil_w;
				vectorsMapArray[vectorIndexes[i]].startPosX += ceil_w;
			}
		}
		ImGui::SameLine(72);
		if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]].endPosY -= ceil_h;
				vectorsMapArray[vectorIndexes[i]].startPosY -= ceil_h;
			}
		}
		ImGui::SameLine(98);
		if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]].endPosY += ceil_h;
				vectorsMapArray[vectorIndexes[i]].startPosY += ceil_h;
			}
		}

		ImGui::End();
	}
}

void Game::update()
{
}

void Game::render()
{
	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	//drawing the map
	map->drawMap();

	//drawing current line
	if (mouseIsDown)
	{
		map->drawLine(startPosX, startPosY, endPosX, endPosY, currLineColorR, currLineColorG, currLineColorB);
	}
	else if (selectAreaIsDown)
	{
		map->drawLine(startPosX, startPosY, startPosX, endPosY, 255, 255, 0);
		map->drawLine(startPosX, startPosY, endPosX, startPosY, 255, 255, 0);
		map->drawLine(endPosX, startPosY, endPosX, endPosY, 255, 255, 0);
		map->drawLine(startPosX, endPosY, endPosX, endPosY, 255, 255, 0);
	}

	if (vectorIndexes.size() != 0)
	{
		for (int i = 0; i < vectorIndexes.size(); i++)
		{
			map->drawLine(vectorsMapArray[vectorIndexes[i]].startPosX, vectorsMapArray[vectorIndexes[i]].startPosY, vectorsMapArray[vectorIndexes[i]].endPosX, 
				vectorsMapArray[vectorIndexes[i]].endPosY, 255, 255, 0);
		}
	}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game was ended" << std::endl;
}

void Game::setDotPosition(int x, int y)
{
	for (int i = 0; i < gridCols; i++) {
		for (int j = 0; j < gridRows; j++) {
			int squareX = gridDotsArray[i][j].squarePosX;
			int squareY = gridDotsArray[i][j].squarePosY;
			int squareSide = gridDotsArray[i][j].squareLength;

			if ((x >= squareX && x <= (squareX + squareSide)) && (y >= squareY && y <= (squareY + squareSide)))
			{
				std::cout << "method was entered 2" << std::endl;
				if (!mouseIsDown)
				{
					mouseIsDown = true;

					startPosX = gridDotsArray[i][j].posX;
					startPosY = gridDotsArray[i][j].posY;
					endPosX = x;
					endPosY = y;

					std::cout << "pos x start: " << startPosX << std::endl;
					std::cout << "pos y start: " << startPosY << std::endl;
				}
				else if(mouseIsDown)
				{
					mouseIsDown = false;

					endPosX = gridDotsArray[i][j].posX;
					endPosY = gridDotsArray[i][j].posY;
					std::cout << "pos x end: " << endPosX << std::endl;
					std::cout << "pos y end: " << endPosY << std::endl;

					//call the method to add a line to the list of square lines
					if ((startPosX != endPosX) || (startPosY != endPosY))
					{
						addNewLine();
					}
				}
			}
		}
	}
}

void Game::addNewLine()
{
	std::cout << "Adding new line" << std::endl;

	double dirX;
	double dirY;

	if (startPosX == endPosX)
	{
		if (startPosY > endPosY)
		{
			dirX = 0;
			dirY = -1;
		}
		else
		{
			dirX = 0;
			dirY = 1;
		}

		LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
		vectorsMapArray.push_back(*newLine);
	}
	else if (startPosY == endPosY)
	{
		if (startPosX > endPosX)
		{
			dirX = -1;
			dirY = 0;
		}
		else
		{
			dirX = 1;
			dirY = 0;
		}

		LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
		vectorsMapArray.push_back(*newLine);
	}
	else
	{
		int cooficient;

		double lineLengthX = abs(endPosX - startPosX);
		double lineLengthY = abs(endPosY - startPosY);

		if (lineLengthX > lineLengthY)
		{
			cooficient = lineLengthX / ceil_w;
		}
		else
		{
			cooficient = lineLengthY / ceil_h;
		}

		dirX = (lineLengthX / cooficient);
		dirY = (lineLengthY / cooficient);

		if ((startPosX > endPosX) && (startPosY > endPosY))
		{
			dirX = -(dirX / ceil_w);
			dirY = -(dirY / ceil_h);
		}
		else if ((startPosX < endPosX) && (startPosY < endPosY))
		{
			dirX = (dirX / ceil_w);
			dirY = (dirY / ceil_h);
		}
		else if ((startPosX < endPosX) && (startPosY > endPosY))
		{
			dirX = (dirX / ceil_w);
			dirY = -(dirY / ceil_h);
		}
		else if ((startPosX > endPosX) && (startPosY < endPosY))
		{
			dirX = -(dirX / ceil_w);
			dirY = (dirY / ceil_h);
		}

		LineMap* newLine = new LineMap(startPosX, startPosY, endPosX, endPosY, dirX, dirY);
		vectorsMapArray.push_back(*newLine);
		std::cout << "size: " <<  vectorsMapArray.size() << std::endl;
	}

	std::cout << "dirX: " << dirX << std::endl;
	std::cout << "dirY: " << dirY << std::endl;
}

void Game::selectItem()
{
	if (startPosX > endPosX && startPosY > endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			if (vectorsMapArray[i].startPosX >= endPosX && vectorsMapArray[i].startPosY >= endPosY &&
				vectorsMapArray[i].endPosX >= endPosX && vectorsMapArray[i].endPosY >= endPosY &&
				vectorsMapArray[i].startPosX <= startPosX && vectorsMapArray[i].startPosY <= startPosY &&
				vectorsMapArray[i].endPosX <= startPosX && vectorsMapArray[i].endPosY <= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
	else if (startPosX > endPosX && startPosY < endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			if (vectorsMapArray[i].startPosX >= endPosX && vectorsMapArray[i].startPosY <= endPosY &&
				vectorsMapArray[i].endPosX >= endPosX && vectorsMapArray[i].endPosY <= endPosY &&
				vectorsMapArray[i].startPosX <= startPosX && vectorsMapArray[i].startPosY >= startPosY &&
				vectorsMapArray[i].endPosX <= startPosX && vectorsMapArray[i].endPosY >= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
	else if (startPosX < endPosX && startPosY > endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			if (vectorsMapArray[i].startPosX <= endPosX && vectorsMapArray[i].startPosY >= endPosY &&
				vectorsMapArray[i].endPosX <= endPosX && vectorsMapArray[i].endPosY >= endPosY &&
				vectorsMapArray[i].startPosX >= startPosX && vectorsMapArray[i].startPosY <= startPosY &&
				vectorsMapArray[i].endPosX >= startPosX && vectorsMapArray[i].endPosY <= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
	else if (startPosX < endPosX && startPosY < endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			if (vectorsMapArray[i].startPosX <= endPosX && vectorsMapArray[i].startPosY <= endPosY &&
				vectorsMapArray[i].endPosX <= endPosX && vectorsMapArray[i].endPosY <= endPosY &&
				vectorsMapArray[i].startPosX >= startPosX && vectorsMapArray[i].startPosY >= startPosY &&
				vectorsMapArray[i].endPosX >= startPosX && vectorsMapArray[i].endPosY >= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
}

bool Game::running()
{
	return isRunning;
}

