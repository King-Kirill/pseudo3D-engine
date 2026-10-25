#include "../headers/GameSDL.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

std::future<void> get_map;
std::future<void> get_char_coordinates;
GameCharacter* selectedChar;
static bool pressOnChar;
bool waiting_for_key = false;
bool setWindow_character;
bool setWindow_changeZone;
int buttonId = 0;

Game::~Game()
{
	delete this->map;
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	//initialization section
	startPosSet = false;
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
		ImFont* iconFont = io.Fonts->AddFontFromFileTTF("fonts/Inter/InterVariable.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
		io.Fonts->Build();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);

		isRunning = true;
		this->map = new MapDraw(renderer);

		initSquareMapArray();
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
	int mouseX;
	int mouseY;

	if (selected_engine_window == "map")
	{
		switch (event.type)
		{
		case(SDL_QUIT):
			isRunning = false;
			break;
		case(SDL_MOUSEBUTTONDOWN):
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemFocused())
			{
				if (!ImGui::GetIO().WantCaptureMouse)
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
						if (!pressOnChar)
						{
							selectedChar = map->getGameCharacter(event.motion.x, event.motion.y);
						}

						pressBtnTick = SDL_GetTicks();
						mapMoove = true;
						prevMouseX = event.motion.x;
						prevMouseY = event.motion.y;
					}
				}
			}
			break;
		case(SDL_MOUSEBUTTONUP):
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemFocused())
			{
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					endPosX = event.motion.x;
					endPosY = event.motion.y;

					selectAreaIsDown = false;

					selectItem();
				}
				else if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x = event.button.x;
					int y = event.button.y;

					if (x > (img_w - dev_zone_w - additional_window_w) && x < (img_w - dev_zone_w) && y > 0 && y < additional_window_h)
					{
						selected_engine_window = "game";

						normalizeMap();

						mapMoove = false;

						break;
					}

					releaseBtnTick = SDL_GetTicks();

					if (mapMoove)
					{
						double distance = sqrt(pow(prevMouseX - event.motion.x, 2) + pow(prevMouseY - event.motion.y, 2));

						if (distance <= 0.1)
						{
							if (releaseBtnTick - pressBtnTick <= 100)
							{
								if (placeCharacter)
								{
									if (!ImGui::IsAnyItemFocused() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
									{
										//вызов метода создания игрока
										std::cout << currCharName << std::endl;
										if (map->checkCharName(currCharName))
										{
											if (currCharRole == "main" && mainCharacterCreated)
											{
												std::cout << "main character already exists, please try another role" << std::endl;
											}
											else
											{
												if (currCharRole == "main")
												{
													mainCharacterCreated = true;
												}

												characterX = prevMouseX;
												characterY = prevMouseY;

												map->createCharacter(currCharName, currCharColorR, currCharColorG, currCharColorB, characterX, characterY, currDirX, currDirY);
												placeCharacter = false;
												currCharName = "";
											}
										}
										else
										{
											std::cout << "seems that this name is already in use, pls try another one..." << std::endl;
										}
									}
								}
								else if (selectedChar != nullptr && pressOnChar == false)
								{
									pressOnChar = true;
									setWindow_character = true;
								}
								else
								{
									setDotPosition(prevMouseX, prevMouseY);
									drawinWhileMoove = true;
								}
							}
						}

					}
					pressBtnTick = 0;
					releaseBtnTick = 0;
					mapMoove = false;
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
			if (startPosSet || selectAreaIsDown)
			{
				endPosX = event.motion.x;
				endPosY = event.motion.y;
				setWindow_changeZone = true;
			}
			break;
		case(SDL_MOUSEWHEEL):
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemFocused())
			{
				if (event.wheel.y > 0) {
					if (squareHeight < maxCeil_h)
					{
						SDL_GetMouseState(&mouseX, &mouseY);
						zooming(true, mouseX, mouseY);
					}
				}
				else if (event.wheel.y < 0) {
					if (squareHeight > ceil_h)
					{
						SDL_GetMouseState(&mouseX, &mouseY);
						zooming(false, mouseX, mouseY);
					}
				}
				break;
			}
		case(SDL_KEYDOWN):
			if (waiting_for_key)
			{
				SDL_Scancode key = event.key.keysym.scancode;

				if (buttonId == 1)
				{
					selectedChar->bound_key_forward = key;
				}
				else if (buttonId == 2)
				{
					selectedChar->bound_key_backward = key;
				}
				else if (buttonId == 3)
				{
					selectedChar->bound_key_left = key;
				}
				else if (buttonId == 4)
				{
					selectedChar->bound_key_right = key;
				}
				waiting_for_key = false;
				buttonId = 0;
			}
			else if (!ImGui::IsAnyItemFocused() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				for (int i = 0; i < characters.size(); i++)
				{

					if (characters[i]->key_pressed_rotation == '\0')
					{
						SDL_Scancode key = event.key.keysym.scancode;

						if (key == characters[i]->bound_key_left)
						{
							characters[i]->key_pressed_rotation = 'l';
						}
						else if (key == characters[i]->bound_key_right)
						{
							characters[i]->key_pressed_rotation = 'r';
						}
					}

					if (characters[i]->key_pressed == '\0')
					{
						SDL_Scancode key = event.key.keysym.scancode;

						if (key == characters[i]->bound_key_forward)
						{
							characters[i]->key_pressed = 'f';
						}
						else if (key == characters[i]->bound_key_backward)
						{
							characters[i]->key_pressed = 'b';
						}
					}
				}
			}
			break;
		case(SDL_KEYUP):
			for (int i = 0; i < characters.size(); i++)
			{
				if (characters[i]->key_pressed != '\0')
				{
					SDL_Scancode key = event.key.keysym.scancode;

					if (key == characters[i]->bound_key_forward)
					{
						characters[i]->key_pressed = '\0';
					}
					else if (key == characters[i]->bound_key_backward)
					{
						characters[i]->key_pressed = '\0';
					}
				}

				if (characters[i]->key_pressed_rotation != '\0')
				{
					SDL_Scancode key = event.key.keysym.scancode;

					if (key == characters[i]->bound_key_left)
					{
						characters[i]->key_pressed_rotation = '\0';
					}
					else if (key == characters[i]->bound_key_right)
					{
						characters[i]->key_pressed_rotation = '\0';
					}
				}
			}

			break;
		default:
			break;
		}

		if (mapMoove)
		{
			int currentX;
			int currentY;
			SDL_GetMouseState(&currentX, &currentY);

			get_map = std::async(std::launch::async, Game::getNewMap_coroutine, currentX, currentY, std::ref(startPosX), std::ref(startPosY), std::ref(window));
		}
	}
	else if(selected_engine_window == "game")
	{
		switch (event.type)
		{
		case(SDL_QUIT):
			isRunning = false;
			break;
		case(SDL_MOUSEBUTTONUP):
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemFocused())
			{
				if (!ImGui::GetIO().WantCaptureMouse)
				{
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						int x = event.button.x;
						int y = event.button.y;

						if (x > (img_w - dev_zone_w - additional_window_w) && x < (img_w - dev_zone_w) && y > 0 && y < additional_window_h)
						{
							selected_engine_window = "map";

							normalizeMap();

							break;
						}
					}
				}
			}
			break;
		case(SDL_KEYDOWN):
			if (waiting_for_key)
			{
				SDL_Scancode key = event.key.keysym.scancode;

				if (buttonId == 1) selectedChar->bound_key_forward = key;
				else if (buttonId == 2) selectedChar->bound_key_backward = key;
				else if (buttonId == 3) selectedChar->bound_key_left = key;
				else if (buttonId == 4) selectedChar->bound_key_right = key;

				waiting_for_key = false;
				buttonId = 0;
			}
			else if (!ImGui::IsAnyItemFocused() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				for (int i = 0; i < characters.size(); i++)
				{
					if (characters[i]->key_pressed_rotation == '\0')
					{
						SDL_Scancode key = event.key.keysym.scancode;

						if (key == characters[i]->bound_key_left)
							characters[i]->key_pressed_rotation = 'l';
						else if (key == characters[i]->bound_key_right)
							characters[i]->key_pressed_rotation = 'r';
					}

					if (characters[i]->key_pressed == '\0')
					{
						SDL_Scancode key = event.key.keysym.scancode;

						if (key == characters[i]->bound_key_forward)
							characters[i]->key_pressed = 'f';
						else if (key == characters[i]->bound_key_backward)
							characters[i]->key_pressed = 'b';
					}
				}
			}
			break;
		case(SDL_KEYUP):
			for (int i = 0; i < characters.size(); i++)
			{
				if (characters[i]->key_pressed != '\0')
				{
					SDL_Scancode key = event.key.keysym.scancode;

					if (key == characters[i]->bound_key_forward)
					{
						characters[i]->key_pressed = '\0';
					}
					else if (key == characters[i]->bound_key_backward)
					{
						characters[i]->key_pressed = '\0';
					}
				}

				if (characters[i]->key_pressed_rotation != '\0')
				{
					SDL_Scancode key = event.key.keysym.scancode;

					if (key == characters[i]->bound_key_left)
					{
						characters[i]->key_pressed_rotation = '\0';
					}
					else if (key == characters[i]->bound_key_right)
					{
						characters[i]->key_pressed_rotation = '\0';
					}
				}
			}

			break;
		default:
			break;
		}
	}

	for (int i = 0; i < characters.size(); i++)
	{
		if (characters[i]->key_pressed != '\0' || characters[i]->key_pressed_rotation != '\0')
		{
			get_char_coordinates = std::async(std::launch::async, Game::getPlayerCoordinates_coroutine, characters[i]);
		}
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
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.8f);
	ImVec2 buttonSize(250, 50);
	ImVec2 midButtonSize(180, 30);
	ImVec2 smallBtnSize(50, 20);

	char placeBtnName[20] = "";
	std::string btnStr;

	if (player == NULL)
	{
		btnStr = "PLACE PLAYER";
	}
	else
	{
		btnStr = "PLACE CHARACTER";
	}

	for (int i = 0; i < 20; i++)
	{
		placeBtnName[i] = btnStr.c_str()[i];
	}

	if (ImGui::Button(placeBtnName, buttonSize))
	{
		if (placeCharacter == true)
		{
			placeCharacter = false;
		}
		else
		{
			placeCharacter = true;
		}
	}

	if (placeCharacter)
	{
		if (currCharName == "")
		{
			std::string numer = std::to_string(characters.size() + 1);
			currCharName = "character" + numer;
		}

		ImGui::SetNextWindowSize(ImVec2(200, 0));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin("Character properties", nullptr);
		ImGui::Spacing();

		cleanInputBuffer();

		if (currCharName != "")
		{
			for (int i = 0; i < bufSize; i++)
			{
				buf[i] = currCharName.c_str()[i];
			}
		}

		ImGui::TextWrapped("name: ");

		if (ImGui::InputText("##NameBinder", buf, bufSize))
		{
			currCharName = buf;
		}
		ImGui::Spacing();

		float color[3] = { static_cast<float>(currCharColorR) / 255.0f, static_cast<float>(currCharColorG) / 255.0f, static_cast<float>(currCharColorB) / 255.0f };
		ImGui::TextWrapped("color: ");
		ImGui::ColorEdit3("", color);
		ImGui::Spacing();

		currCharColorR = color[0] * 255;
		currCharColorG = color[1] * 255;
		currCharColorB = color[2] * 255;
		ImGui::Spacing();

		//выбор роли персонажа
		cleanInputBuffer();

		if (!mainCharacterCreated)
		{
			currCharRole = "main";
		}

		if (currCharRole != "")
		{
			for (int i = 0; i < bufSize; i++)
			{
				buf[i] = currCharRole.c_str()[i];
			}
		}

		float baseAngle = 270.0f * (PI / 180.0f); 
		static float angleRadians = baseAngle; 
		ImGui::TextWrapped("angle direction: "); 
		ImGui::SliderAngle("angle", &angleRadians, 0.0f, 360.0f); 
		ImGui::Spacing(); 
		
		currDirX = cosf(angleRadians); 
		currDirY = sinf(angleRadians);

		ImGui::TextWrapped("role: ");

		if (ImGui::InputText("##RoleBinder", buf, bufSize))
		{
			currCharRole = buf;
		}

		ImGui::Spacing();
		if (!mainCharacterCreated)
		{
			ImGui::TextWrapped("if u want to place main character (playable character), select 'main' role, if u want to avoid bugs");
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::End();
	}

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 250) * 0.5f);
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.9f);
	if (ImGui::Button("BUILD MAP", buttonSize))
	{
		if (previousVectorsMapArray != vectorsMapArray)
		{
			std::cout << "vectors map array: " << vectorsMapArray.size() << std::endl;
			if (vectorsMapArray.size() != 0)
			{
				//очитска и инициализация массивов для перезаписи
				for (int i = 0; i < previousVectorsMapArray.size(); i++)
				{ 
					for (int k = 0; k < previousVectorsMapArray[i]->vectorToLines.size(); k++)
					{
						previousVectorsMapArray[i]->vectorToLines[k].reset();
					}
					previousVectorsMapArray[i]->vectorToLines.clear();

					previousVectorsMapArray[i].reset();
					previousVectorsMapArray.erase(previousVectorsMapArray.begin() + i);
					i -= 1;
				}

				for (int i = 0; i < vectorsMapArray.size(); i++)
				{
					if (vectorsMapArray[i]->vectorToLines.size() > 0)
					{
						for (int k = 0; k < vectorsMapArray[i]->vectorToLines.size(); k++)
						{
							vectorsMapArray[i]->vectorToLines[k].reset();
						}
						vectorsMapArray[i]->vectorToLines.clear();

						i -= 1;
					}
				}

				sectorsArray.clear();
				
				clearSquareMapArray();
				initSquareMapArray();

				std::vector<std::vector<std::shared_ptr<LineMap>>> sectors;

				mainScene->performBSP(sectors);
				mainScene->rastorization(sectors, sectorsArray);
			}
		}
	}
	ImGui::End();

	if (vectorIndexes.size() != 0)
	{
		ImGui::SetNextWindowSize(ImVec2(lineOptionsWin_w, lineOptionsWin_h));

		if (setWindow_changeZone)
		{
			ImGui::SetNextWindowPos(lineOptionsWindowPos);
			setWindow_changeZone = false;
		}

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

		if (ImGui::Button("DELETE", smallButton_1_Size))
		{
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				if(vectorsMapArray.size() > 0)
				{
					for (int k = 0; k < vectorsMapArray[vectorIndexes[i]]->vectorToLines.size(); k++)
					{
						vectorsMapArray[vectorIndexes[i]]->vectorToLines[k].reset();
					}
					vectorsMapArray[vectorIndexes[i]]->vectorToLines.clear();

					vectorsMapArray[vectorIndexes[i]].reset();
					vectorsMapArray.erase(vectorsMapArray.begin() + vectorIndexes[i]);
				}

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
				vectorsMapArray[vectorIndexes[i]]->endPosX -= squareWidth;
				vectorsMapArray[vectorIndexes[i]]->startPosX -= squareWidth;
				 
				if(vectorsMapArray[vectorIndexes[i]]->vectorToLines.size() > 0)
				{
					auto sectorPtr = vectorsMapArray[vectorIndexes[i]]->vectorToLines[0]->sectorObj;
					
					for (int k = 0; k < vectorsMapArray[vectorIndexes[i]]->vectorToLines.size(); k++)
					{
						vectorsMapArray[vectorIndexes[i]]->vectorToLines[k].reset();
					}
					vectorsMapArray[vectorIndexes[i]]->vectorToLines.clear();

					mainScene->rastorLine(vectorsMapArray[vectorIndexes[i]], sectorPtr);
				}
			}
		}
		ImGui::SameLine(35);
		if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]]->endPosX += squareWidth;
				vectorsMapArray[vectorIndexes[i]]->startPosX += squareWidth;

				if (vectorsMapArray[vectorIndexes[i]]->vectorToLines.size() > 0)
				{
					auto sectorPtr = vectorsMapArray[vectorIndexes[i]]->vectorToLines[0]->sectorObj;

					for (int k = 0; k < vectorsMapArray[vectorIndexes[i]]->vectorToLines.size(); k++)
					{
						vectorsMapArray[vectorIndexes[i]]->vectorToLines[k].reset();
					}
					vectorsMapArray[vectorIndexes[i]]->vectorToLines.clear();

					mainScene->rastorLine(vectorsMapArray[vectorIndexes[i]], sectorPtr);
				}
			}
		}
		ImGui::SameLine(72);
		if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]]->endPosY -= squareHeight;
				vectorsMapArray[vectorIndexes[i]]->startPosY -= squareHeight;

				if (vectorsMapArray[vectorIndexes[i]]->vectorToLines.size() > 0)
				{
					auto sectorPtr = vectorsMapArray[vectorIndexes[i]]->vectorToLines[0]->sectorObj;

					for (int k = 0; k < vectorsMapArray[vectorIndexes[i]]->vectorToLines.size(); k++)
					{
						vectorsMapArray[vectorIndexes[i]]->vectorToLines[k].reset();
					}
					vectorsMapArray[vectorIndexes[i]]->vectorToLines.clear();

					mainScene->rastorLine(vectorsMapArray[vectorIndexes[i]], sectorPtr);
				}
			}
		}
		ImGui::SameLine(98);
		if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) {
			for (int i = 0; i < vectorIndexes.size(); i++)
			{
				vectorsMapArray[vectorIndexes[i]]->endPosY += squareHeight;
				vectorsMapArray[vectorIndexes[i]]->startPosY += squareHeight;

				if (vectorsMapArray[vectorIndexes[i]]->vectorToLines.size() > 0)
				{
					auto sectorPtr = vectorsMapArray[vectorIndexes[i]]->vectorToLines[0]->sectorObj;

					for (int k = 0; k < vectorsMapArray[vectorIndexes[i]]->vectorToLines.size(); k++)
					{
						vectorsMapArray[vectorIndexes[i]]->vectorToLines[k].reset();
					}
					vectorsMapArray[vectorIndexes[i]]->vectorToLines.clear();

					mainScene->rastorLine(vectorsMapArray[vectorIndexes[i]], sectorPtr);
				}
			}
		}

		ImGui::End();
	}

	if (pressOnChar)
	{
		ImGui::SetNextWindowSize(ImVec2(characterOptions_width, characterOptions_height));

		if (setWindow_character)
		{
			characterOptionsWindowPos = ImVec2(prevMouseX + (2 * charRadius), prevMouseY + charRadius);
			ImGui::SetNextWindowPos(characterOptionsWindowPos);
			setWindow_character = false;
		}

		ImGui::Begin("Character options", &pressOnChar, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		std::string selectedChar_name = selectedChar->name;
		std::string sleectedChar_role = selectedChar->role;
		static float walk_speed = selectedChar->walk;
		ImGui::TextWrapped("Walk speed: ");
		ImGui::SetNextItemWidth(175.0f);
		ImGui::SliderFloat("walk", &walk_speed, 0.0f, 10.0f, "%.1f");

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TextWrapped("Rotation speed: ");
		static float rotation_speed = selectedChar->rotation;
		ImGui::SetNextItemWidth(175.0f);
		ImGui::SliderFloat("rotation", &rotation_speed, 0.0f, 10.0f, "%.1f");

		selectedChar->walk = walk_speed;
		selectedChar->rotation = rotation_speed;

		ImGui::Spacing();
		ImGui::Spacing();

		float color[3] = { static_cast<float>(selectedChar->colorR) / 255.0f, static_cast<float>(selectedChar->colorG) / 255.0f, static_cast<float>(selectedChar->colorB) / 255.0f };
		ImGui::TextWrapped("Color: ");
		if (ImGui::ColorEdit3("##Color", color))
		{
			selectedChar->colorR = static_cast<uint8_t>(color[0] * 255.0f);
			selectedChar->colorG = static_cast<uint8_t>(color[1] * 255.0f);
			selectedChar->colorB = static_cast<uint8_t>(color[2] * 255.0f);
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TextWrapped("Movement: ");
		ImGui::Spacing();
		const char* keyNameForward =
			SDL_GetScancodeName(selectedChar->bound_key_forward);
		ImGui::Text("Forward: %s", keyNameForward);
		ImGui::SameLine(160);
		if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) {
			waiting_for_key = true;
			buttonId = 1;
		}

		const char* keyNameBack =
			SDL_GetScancodeName(selectedChar->bound_key_backward);
		ImGui::Text("Backward: %s", keyNameBack);
		ImGui::SameLine(160);
		if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) {
			waiting_for_key = true;
			buttonId = 2;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TextWrapped("Rotation: ");
		ImGui::Spacing();
		const char* keyNameLeft =
			SDL_GetScancodeName(selectedChar->bound_key_left);
		ImGui::Text("Left: %s", keyNameLeft);
		ImGui::SameLine(160);
		if (ImGui::ArrowButton("##Left", ImGuiDir_Left)) {
			waiting_for_key = true;
			buttonId = 3;
		}

		const char* keyNameRight =
			SDL_GetScancodeName(selectedChar->bound_key_right);
		ImGui::Text("Right: %s", keyNameRight);
		ImGui::SameLine(160);
		if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) {
			waiting_for_key = true;
			buttonId = 4;
		}

		ImGui::Spacing();
		if (!selectedChar->cameraExists)
		{
			if (ImGui::Button("add camera", midButtonSize)) {
				selectedChar->createCamera();
			}
		}
		else
		{
			static float fov = selectedChar->fov * (PI / 180.0f);
			static float ray_length = selectedChar->raycast_length;

			if (ImGui::Button("destroy camera", midButtonSize)) {
				selectedChar->destroyCamera();
				selectedChar->setFOV(0);
				selectedChar->raycast_length = 0;
				selectedChar->cameraExists = false;
			}
			ImGui::Spacing();

			ImGui::TextWrapped("Camera: ");
			ImGui::TextWrapped("FOV: ");
			ImGui::SetNextItemWidth(175.0f);
			ImGui::SliderAngle("fov", &fov, 0.0f, 120.0f);
			ImGui::Spacing();
			ImGui::TextWrapped("Ray length: ");
			ImGui::SetNextItemWidth(175.0f);
			ImGui::SliderFloat("ray length", &ray_length, 0.0f, 100.0f, "%.1f");

			double FOV = (fov / (PI / 180.0f)) / 100;
			selectedChar->setFOV(FOV);
			selectedChar->raycast_length = ray_length;
		}
		ImGui::Spacing();
		ImGui::Spacing();

		if (!selectedChar->echoExists)
		{
			if (ImGui::Button("add echo", midButtonSize)) {
				selectedChar->createEcho();
			}
		}
		else
		{
			static float fov = selectedChar->echo_fov * (PI / 180.0f);
			static float ray_length = selectedChar->echo_length;
			static float rotation_speed = selectedChar->echo_speed * 10;

			if (ImGui::Button("destroy echo", midButtonSize)) {
				selectedChar->echo_fov = 0;
				selectedChar->echo_length = 0;
				selectedChar->echo_speed = 0;
				selectedChar->echoExists = false;
			}
			ImGui::Spacing();

			ImGui::TextWrapped("Echo: ");
			ImGui::TextWrapped("FOV echo: ");
			ImGui::SetNextItemWidth(175.0f);
			ImGui::SliderAngle("fov echo", &fov, 0.0f, 360.0f);
			ImGui::Spacing();
			ImGui::TextWrapped("Ray length echo: ");
			ImGui::SetNextItemWidth(175.0f);
			ImGui::SliderFloat("ray length echo", &ray_length, 0.0f, 100.0f, "%.1f");
			ImGui::Spacing();
			ImGui::TextWrapped("Rotation speed: ");
			ImGui::SetNextItemWidth(175.0f);
			ImGui::SliderFloat("rotation speed", &rotation_speed, 0.0f, 5.0f, "%.1f");

			double FOV = (fov / (PI / 180.0f)) / 100;
			selectedChar->echo_fov = FOV;
			selectedChar->echo_length = ray_length;

			if (selectedChar->echo_speed > 0)
			{
				selectedChar->echo_speed = rotation_speed / 10;
			}
			else
			{
				selectedChar->echo_speed = -(rotation_speed / 10);
			}
		}
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("add weapon class", midButtonSize)) {
			std::cout << "adding weapon class" << std::endl;
		}
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("close tab", midButtonSize)) {
			pressOnChar = false;
		}

		ImGui::End();
	}
}

void Game::update()
{
}

void Game::zooming(bool flag, int mouseX, int mouseY)
{
	prevSquareWidth = squareWidth;
	prevSquareHeight = squareHeight;

	double epsX = 0;
	double epsY = 0;

	if (flag)
	{
		if ((mouseX > firstZone_startX) && (mouseX < firstZone_endX)
			&& (mouseY > firstZone_startY) && (mouseY < firstZone_endY))
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;

			getPlayerSpeedRotation_zoomIn();

			epsX = 0;
			epsY = 0;
		}
		else if ((mouseX > secondZone_startX && mouseX < secondZone_endX)
			&& mouseY > secondZone_startY && mouseY < secondZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsW -= 0.75;

			getPlayerSpeedRotation_zoomIn();

			epsX = -0.75;
			epsY = 0;
		}
		else if ((mouseX > thirdZone_startX && mouseX < thirdZone_endX)
			&& mouseY > thirdZone_startY && mouseY < thirdZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsW -= 1.5;

			getPlayerSpeedRotation_zoomIn();

			epsX = -1.5;
			epsY = 0;
		}
		else if ((mouseX > fourthZone_startX && mouseX < fourthZone_endX)
			&& mouseY > fourthZone_startY && mouseY < fourthZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsH -= 0.5;

			getPlayerSpeedRotation_zoomIn();

			epsX = 0;
			epsY = -0.5;
		}
		else if ((mouseX > fifthZone_startX && mouseX < fifthZone_endX)
			&& mouseY > fifthZone_startY && mouseY < fifthZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsH -= 0.5;
			zoomEpsW -= 0.75;

			getPlayerSpeedRotation_zoomIn();

			epsX = -0.75;
			epsY = -0.5;
		}
		else if ((mouseX > sixZone_startX && mouseX < sixZone_endX)
			&& mouseY > sixZone_startY && mouseY < sixZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsH -= 0.5;
			zoomEpsW -= 1.5;

			getPlayerSpeedRotation_zoomIn();

			epsX = -1.5;
			epsY = -0.5;
		}
		else if ((mouseX > sevenZone_startX && mouseX < sevenZone_endX)
			&& mouseY > sevenZone_startY && mouseY < sevenZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsH -= 1;

			getPlayerSpeedRotation_zoomIn();

			epsX = 0;
			epsY = -1;
		}
		else if ((mouseX > eightZone_startX && mouseX < eightZone_endX)
			&& mouseY > eightZone_startY && mouseY < eightZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsH -= 1;
			zoomEpsW -= 0.75;

			getPlayerSpeedRotation_zoomIn();

			epsX = -0.75;
			epsY = -1;
		}
		else if ((mouseX > ninethZone_startX && mouseX < ninethZone_endX)
			&& mouseY > ninethZone_startY && mouseY < ninethZone_endY)
		{
			scrollDotsH -= 1;
			scrollDotsW -= 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size -= step_eps;
			zoomEpsH -= 1;
			zoomEpsW -= 1.5;

			getPlayerSpeedRotation_zoomIn();

			epsX = -1.5;
			epsY = -1;
		}
	}
	else
	{
		if ((mouseX > firstZone_startX) && (mouseX < firstZone_endX)
			&& (mouseY > firstZone_startY) && (mouseY < firstZone_endY))
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			getPlayerSpeedRotation_zoomOut();

			epsX = 0;
			epsY = 0;
		}
		else if ((mouseX > secondZone_startX && mouseX < secondZone_endX)
			&& mouseY > secondZone_startY && mouseY < secondZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsW += 0.75;

			getPlayerSpeedRotation_zoomOut();

			epsX = 0.75;
			epsY = 0;
		}
		else if ((mouseX > thirdZone_startX && mouseX < thirdZone_endX)
			&& mouseY > thirdZone_startY && mouseY < thirdZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsW += 1.5;

			getPlayerSpeedRotation_zoomOut();

			epsX = 1.5;
			epsY = 0;
		}
		else if ((mouseX > fourthZone_startX && mouseX < fourthZone_endX)
			&& mouseY > fourthZone_startY && mouseY < fourthZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsH += 0.5;

			getPlayerSpeedRotation_zoomOut();

			epsX = 0;
			epsY = 0.5;
		}
		else if ((mouseX > fifthZone_startX && mouseX < fifthZone_endX)
			&& mouseY > fifthZone_startY && mouseY < fifthZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsH += 0.5;
			zoomEpsW += 0.75;

			getPlayerSpeedRotation_zoomOut();

			epsX = 0.75;
			epsY = 0.5;
		}
		else if ((mouseX > sixZone_startX && mouseX < sixZone_endX)
			&& mouseY > sixZone_startY && mouseY < sixZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsH += 0.5;
			zoomEpsW += 1.5;

			getPlayerSpeedRotation_zoomOut();

			epsX = 1.5;
			epsY = 0.5;
		}
		else if ((mouseX > sevenZone_startX && mouseX < sevenZone_endX)
			&& mouseY > sevenZone_startY && mouseY < sevenZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsH += 1;

			getPlayerSpeedRotation_zoomOut();

			epsX = 0;
			epsY = 1;
		}
		else if ((mouseX > eightZone_startX && mouseX < eightZone_endX)
			&& mouseY > eightZone_startY && mouseY < eightZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsH += 1;
			zoomEpsW += 0.75;

			getPlayerSpeedRotation_zoomOut();

			epsX = 0.75;
			epsY = 1;
		}
		else if ((mouseX > ninethZone_startX && mouseX < ninethZone_endX)
			&& mouseY > ninethZone_startY && mouseY < ninethZone_endY)
		{
			scrollDotsH += 1;
			scrollDotsW += 1.5;
			squareHeight = img_h / scrollDotsH;
			squareWidth = (img_w - dev_zone_w) / scrollDotsW;
			step_size += step_eps;

			zoomEpsH += 1;
			zoomEpsW += 1.5;

			getPlayerSpeedRotation_zoomOut();

			epsX = 1.5;
			epsY = 1;
		}
	}

	for (int i = 0; i < vectorsMapArray.size(); i++)
	{
		LineMap& vectorMap = *vectorsMapArray[i];

		double startPosX = vectorMap.startPosX;
		double startPosY = vectorMap.startPosY;

		double endPosX = vectorMap.endPosX;
		double endPosY = vectorMap.endPosY;

		double coordinate_startX = (startPosX / prevSquareWidth) + epsX;
		double coordinate_startY = (startPosY / prevSquareHeight) + epsY;
		
		double coordinate_endX = (endPosX / prevSquareWidth) + epsX;
		double coordinate_endY = (endPosY / prevSquareHeight) + epsY;

		vectorMap.startPosX = coordinate_startX * squareWidth;
		vectorMap.startPosY = coordinate_startY * squareHeight;

		vectorMap.endPosX = coordinate_endX * squareWidth;
		vectorMap.endPosY = coordinate_endY * squareHeight;
	}

	for (int i = 0; i < gridCols - 1; i++)
	{
		for(int j = 0; j < gridRows - 1; j++)
		{
			if (squareMapArray[i][j].isEmpty == false)
			{
				std::vector<std::weak_ptr<LineMap>>& linesList = squareMapArray[i][j].linesList;

				for (int k = 0; k < linesList.size(); k++)
				{
					auto linePtr = squareMapArray[i][j].linesList[k].lock();

					if (linePtr != nullptr)
					{
						double startPosX = linePtr->startPosX;
						double startPosY = linePtr->startPosY;

						double endPosX = linePtr->endPosX;
						double endPosY = linePtr->endPosY;

						double coordinate_startX = (startPosX / prevSquareWidth) + epsX;
						double coordinate_startY = (startPosY / prevSquareHeight) + epsY;

						double coordinate_endX = (endPosX / prevSquareWidth) + epsX;
						double coordinate_endY = (endPosY / prevSquareHeight) + epsY;

						linePtr->startPosX = coordinate_startX * squareWidth;
						linePtr->startPosY = coordinate_startY * squareHeight;

						linePtr->endPosX = coordinate_endX * squareWidth;
						linePtr->endPosY = coordinate_endY * squareHeight;
					}
				}
			}
		}
	}

	for (int i = 0; i < characters.size(); i++)
	{
		double coordinate_startX = (characters[i]->posX / prevSquareWidth) + epsX;
		double coordinate_startY = (characters[i]->posY / prevSquareHeight) + epsY;

		characters[i]->posX = coordinate_startX * squareWidth;
		characters[i]->posY = coordinate_startY * squareHeight;
	}
}


void Game::render()
{
	if (get_map.valid())
	{
		get_map.get();
	}

	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_Rect main_rect = { 0, 0, (img_w - dev_zone_w), img_h };
	SDL_Rect mini_rect = { (img_w - dev_zone_w - additional_window_w), 0, additional_window_w, additional_window_h };

	SDL_RenderSetClipRect(renderer, &main_rect);

	map->countMiniMapCenterDiffer();

	if(selected_engine_window == "map")
	{
		//drawing the map
		map->drawMap();

		//drawing current line
		if (startPosSet)
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
				LineMap& vectorMap = *vectorsMapArray[vectorIndexes[i]];

				map->drawLine(vectorMap.startPosX, vectorMap.startPosY, vectorMap.endPosX,
					vectorMap.endPosY, 255, 255, 0);
			}
		}

		if (get_char_coordinates.valid())
		{
			get_char_coordinates.get();
		}

		if (characters.size() != 0)
		{
			map->drawCharacters();
		}
		
		//drawing game window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &mini_rect);
		SDL_RenderSetClipRect(renderer, &mini_rect);

		map->drawGame(selected_engine_window);
		map->drawMapEdgesSmall();
	}
	else if(selected_engine_window == "game")
	{
		//drawing game window
		map->drawGame(selected_engine_window);

		//drawing small map
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &mini_rect);
		SDL_RenderSetClipRect(renderer, &mini_rect);

		map->drawMapSmall();

		if (get_char_coordinates.valid())
		{
			get_char_coordinates.get();
		}

		if (characters.size() != 0)
		{
			map->drawCharacters();
		}
	}

	map->drawMapEdgesSmall();

	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void Game::cleanInputBuffer()
{
	for (int i = 0; i < bufSize; i++)
	{
		buf[i] = '\0';
	}
}

void Game::clean()
{
	//очистка памяти если есть что чистить
	clearCharacters();
	clearSquareMapArray();
	clearGridDotsArray();

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game was ended" << std::endl;
}

void Game::setDotPosition(int& x, int& y)
{
	for (int i = 0; i < gridCols; i++) {
		for (int j = 0; j < gridRows; j++) {
			int squareX = gridDotsArray[i][j].squarePosX;
			int squareY = gridDotsArray[i][j].squarePosY;
			int squareSide = gridDotsArray[i][j].squareLength;


			if ((x >= squareX && x <= (squareX + squareSide)) && (y >= squareY && y <= (squareY + squareSide)))
			{
				std::cout << "method was entered 2" << std::endl;
				if (!startPosSet)
				{
					startPosSet = true;

					startPosX = gridDotsArray[i][j].posX;
					startPosY = gridDotsArray[i][j].posY;
					endPosX = x;
					endPosY = y;
				}
				else if (startPosSet)
				{
					startPosSet = false;

					endPosX = gridDotsArray[i][j].posX;
					endPosY = gridDotsArray[i][j].posY;
					std::cout << "pos x end: " << endPosX << std::endl;
					std::cout << "pos y end: " << endPosY << std::endl;

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
		std::shared_ptr<LineMap> ptr(newLine);
		vectorsMapArray.push_back(ptr);
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
		std::shared_ptr<LineMap> ptr(newLine);
		vectorsMapArray.push_back(ptr);
	}
	else
	{
		int cooficient;

		double lineLengthX = fabs(endPosX - startPosX);
		double lineLengthY = fabs(endPosY - startPosY);

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
		std::shared_ptr<LineMap> ptr(newLine);
		vectorsMapArray.push_back(ptr);
		std::cout << "size: " << vectorsMapArray.size() << std::endl;
	}
}

void Game::selectItem()
{
	if (startPosX > endPosX && startPosY > endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			LineMap& vectorMap = *vectorsMapArray[i];

			if (vectorMap.startPosX >= endPosX && vectorMap.startPosY >= endPosY &&
				vectorMap.endPosX >= endPosX && vectorMap.endPosY >= endPosY &&
				vectorMap.startPosX <= startPosX && vectorMap.startPosY <= startPosY &&
				vectorMap.endPosX <= startPosX && vectorMap.endPosY <= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
	else if (startPosX > endPosX && startPosY < endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			LineMap& vectorMap = *vectorsMapArray[i];

			if (vectorMap.startPosX >= endPosX && vectorMap.startPosY <= endPosY &&
				vectorMap.endPosX >= endPosX && vectorMap.endPosY <= endPosY &&
				vectorMap.startPosX <= startPosX && vectorMap.startPosY >= startPosY &&
				vectorMap.endPosX <= startPosX && vectorMap.endPosY >= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
	else if (startPosX < endPosX && startPosY > endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			LineMap& vectorMap = *vectorsMapArray[i];

			if (vectorMap.startPosX <= endPosX && vectorMap.startPosY >= endPosY &&
				vectorMap.endPosX <= endPosX && vectorMap.endPosY >= endPosY &&
				vectorMap.startPosX >= startPosX && vectorMap.startPosY <= startPosY &&
				vectorMap.endPosX >= startPosX && vectorMap.endPosY <= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
	else if (startPosX < endPosX && startPosY < endPosY)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			LineMap& vectorMap = *vectorsMapArray[i];

			if (vectorMap.startPosX <= endPosX && vectorMap.startPosY <= endPosY &&
				vectorMap.endPosX <= endPosX && vectorMap.endPosY <= endPosY &&
				vectorMap.startPosX >= startPosX && vectorMap.startPosY >= startPosY &&
				vectorMap.endPosX >= startPosX && vectorMap.endPosY >= startPosY)
			{
				vectorIndexes.push_back(i);
			}
		}
	}
}

void Game::getNewMap_coroutine(double currentX, double currentY, double& startPosX, double& startPosY, SDL_Window*& window)
{
	double distance = sqrt(pow(prevMouseX - currentX, 2) + pow(prevMouseY - currentY, 2));

	double epsX = 0;
	double epsY = 0;

	if (distance > 1)
	{
		epsX = ((prevMouseY - currentY) * (deltaTime / 10)) * 0.025;
	    epsY = ((prevMouseX - currentX) * (deltaTime / 10)) * 0.025;
	}

	step_size = step_size * (deltaTime / 10);

	if (currentX <= 0)
	{
		epsY = step_size;

		SDL_WarpMouseInWindow(window, 0, currentY);
	}
	if (currentY <= 0)
	{
		epsX = step_size;

		SDL_WarpMouseInWindow(window, currentX, 0);
	}
	if (currentX >= img_w - dev_zone_w)
	{
		epsY = -step_size;

		SDL_WarpMouseInWindow(window, img_w - dev_zone_w, currentY);
	}
	if (currentY >= img_h)
	{
		epsX = -step_size;

		SDL_WarpMouseInWindow(window, currentX, img_h);
	}

	zoomEpsH += epsX;
	zoomEpsW += epsY;

	double epsDotX = epsY * squareWidth;
	double epsDotY = epsX * squareHeight;

	for (int i = 0; i < vectorsMapArray.size(); i++)
	{
		LineMap& vectorMap = *vectorsMapArray[i];

		vectorMap.startPosX += epsDotX;
		vectorMap.startPosY += epsDotY;

		vectorMap.endPosX += epsDotX;
		vectorMap.endPosY += epsDotY;
	}

	for (int i = 0; i < characters.size(); i++)
	{
		characters[i]->posX += epsDotX;
		characters[i]->posY += epsDotY;
	}

	for (int i = 0; i < gridCols - 1; i++)
	{
		for (int j = 0; j < gridRows - 1; j++)
		{
			if (squareMapArray[i][j].isEmpty == false)
			{
				std::vector<std::weak_ptr<LineMap>>& linesList = squareMapArray[i][j].linesList;

				for (int k = 0; k < linesList.size(); k++)
				{
					auto linePtr = linesList[k].lock();

					if(linePtr != nullptr)
					{ 
						linePtr->startPosX += epsDotX;
						linePtr->startPosY += epsDotY;

						linePtr->endPosX += epsDotX;
						linePtr->endPosY += epsDotY;
					}
				}
			}
		}
	}

	if (drawinWhileMoove)
	{
		startPosX += epsDotX;
		startPosY += epsDotY;
	}

	prevMouseX = currentX;
	prevMouseY = currentY;
}

void Game::getPlayerCoordinates_coroutine(GameCharacter* character)
{
	if (character != NULL)
	{
		//движение главного персонажа (основной камеры) на мини карте
		if (character->role == "main" && selected_engine_window != "map")
		{
			if (character->key_pressed == 'f')
			{
				double stepX = (-character->dirX * character->walk * (deltaTime / 10));
				double stepY = (-character->dirY * character->walk * (deltaTime / 10));

				mini_map_epsilon_x += stepX;
				mini_map_epsilon_y += stepY;

				character->posX += -stepX;
				character->posY += -stepY;
			}
			else if (character->key_pressed == 'b')
			{
				double stepX = (-character->dirX * character->walk * (deltaTime / 10));
				double stepY = (-character->dirY * character->walk * (deltaTime / 10));

				mini_map_epsilon_x -= stepX;
				mini_map_epsilon_y -= stepY;

				character->posX += stepX;
				character->posY += stepY;
			}

			double rotation = (character->rotation / 10) * (deltaTime / 10);

			if (character->key_pressed_rotation == 'l')
			{
				double oldDirX = character->dirX;
				character->dirX = character->dirX * cos(-rotation) - character->dirY * sin(-rotation);
				character->dirY = oldDirX * sin(-rotation) + character->dirY * cos(-rotation);
				double oldPlaneX = character->planeX;
				character->planeX = character->planeX * cos(-rotation) - character->planeY * sin(-rotation);
				character->planeY = oldPlaneX * sin(-rotation) + character->planeY * cos(-rotation);
			}
			else if (character->key_pressed_rotation == 'r')
			{
				double oldDirX = character->dirX;
				character->dirX = character->dirX * cos(rotation) - character->dirY * sin(rotation);
				character->dirY = oldDirX * sin(rotation) + character->dirY * cos(rotation);
				double oldPlaneX = character->planeX;
				character->planeX = character->planeX * cos(rotation) - character->planeY * sin(rotation);
				character->planeY = oldPlaneX * sin(rotation) + character->planeY * cos(rotation);
			}

			character->map_pos_x = int((character->posX / squareWidth) - zoomEpsW);
			character->map_pos_y = int((character->posY / squareHeight) - zoomEpsH);
		}
		else
		{
			if (character->key_pressed == 'f')
			{
				double stepX = (character->dirX * character->walk * (deltaTime / 10));
				double stepY = (character->dirY * character->walk * (deltaTime / 10));

				character->posX += stepX;
				character->posY += stepY;
			}
			else if (character->key_pressed == 'b')
			{
				double stepX = (character->dirX * character->walk * (deltaTime / 10));
				double stepY = (character->dirY * character->walk * (deltaTime / 10));

				character->posX += -stepX;
				character->posY += -stepY;
			}

			double rotation = (character->rotation / 10) * (deltaTime / 10);

			if (character->key_pressed_rotation == 'l')
			{
				double oldDirX = character->dirX;
				character->dirX = character->dirX * cos(-rotation) - character->dirY * sin(-rotation);
				character->dirY = oldDirX * sin(-rotation) + character->dirY * cos(-rotation);
				double oldPlaneX = character->planeX;
				character->planeX = character->planeX * cos(-rotation) - character->planeY * sin(-rotation);
				character->planeY = oldPlaneX * sin(-rotation) + character->planeY * cos(-rotation);
			}
			else if (character->key_pressed_rotation == 'r')
			{
				double oldDirX = character->dirX;
				character->dirX = character->dirX * cos(rotation) - character->dirY * sin(rotation);
				character->dirY = oldDirX * sin(rotation) + character->dirY * cos(rotation);
				double oldPlaneX = character->planeX;
				character->planeX = character->planeX * cos(rotation) - character->planeY * sin(rotation);
				character->planeY = oldPlaneX * sin(rotation) + character->planeY * cos(rotation);
			}

			character->map_pos_x = int((character->posX / squareWidth) - zoomEpsW);
			character->map_pos_y = int((character->posY / squareHeight) - zoomEpsH);
		}
	}
}

void Game::getPlayerSpeedRotation_zoomIn()
{
	double percent_width = prevSquareWidth / (100 * (squareWidth - prevSquareWidth));
	double player_one_percent_radius = charRadius / 100;

	for (int i = 0; i < characters.size(); i++)
	{
		double player_one_percent_value = characters[i]->rotation / 100;
		characters[i]->rotation -= percent_width * player_one_percent_value;
	}
	charRadius += percent_width * player_one_percent_radius;
}

void Game::getPlayerSpeedRotation_zoomOut()
{
	double percent_width = prevSquareWidth / (100 * (prevSquareWidth - squareWidth));
	double player_one_percent_radius = charRadius / 100;

	for (int i = 0; i < characters.size(); i++)
	{
		double player_one_percent_value = characters[i]->rotation / 100;
		characters[i]->rotation += percent_width * player_one_percent_value;
	}
	charRadius -= percent_width * player_one_percent_radius;
}

//нормализуем карту
void Game::normalizeMap()
{
	prevSquareWidth = squareWidth;
	prevSquareHeight = squareHeight;

	squareHeight = ceil_h;
	squareWidth = ceil_w;

	if (prevSquareHeight != 0 && prevSquareWidth != 0)
	{
		for (int i = 0; i < vectorsMapArray.size(); i++)
		{
			LineMap& vectorMap = *vectorsMapArray[i];

			double startPosX = vectorMap.startPosX;
			double startPosY = vectorMap.startPosY;

			double endPosX = vectorMap.endPosX;
			double endPosY = vectorMap.endPosY;

			double coordinate_startX = (startPosX / prevSquareWidth) - zoomEpsW;
			double coordinate_startY = (startPosY / prevSquareHeight) - zoomEpsH;

			double coordinate_endX = (endPosX / prevSquareWidth) - zoomEpsW;
			double coordinate_endY = (endPosY / prevSquareHeight) - zoomEpsH;

			vectorMap.startPosX = (coordinate_startX * squareWidth);
			vectorMap.startPosY = coordinate_startY * squareHeight;

			vectorMap.endPosX = (coordinate_endX * squareWidth);
			vectorMap.endPosY = (coordinate_endY * squareHeight);
		}

		for (int i = 0; i < gridCols - 1; i++)
		{
			for (int j = 0; j < gridRows - 1; j++)
			{
				if (squareMapArray[i][j].isEmpty == false)
				{
					std::vector<std::weak_ptr<LineMap>>& linesList = squareMapArray[i][j].linesList;

					for (int k = 0; k < linesList.size(); k++)
					{
						auto linePtr = squareMapArray[i][j].linesList[k].lock();

						if (linePtr != nullptr)
						{
							double startPosX = linePtr->startPosX;
							double startPosY = linePtr->startPosY;

							double endPosX = linePtr->endPosX;
							double endPosY = linePtr->endPosY;

							double coordinate_startX = (startPosX / prevSquareWidth) - zoomEpsW;
							double coordinate_startY = (startPosY / prevSquareHeight) - zoomEpsH;

							double coordinate_endX = (endPosX / prevSquareWidth) - zoomEpsW;
							double coordinate_endY = (endPosY / prevSquareHeight) - zoomEpsH;

							linePtr->startPosX = coordinate_startX * squareWidth;
							linePtr->startPosY = coordinate_startY * squareHeight;

							linePtr->endPosX = coordinate_endX * squareWidth;
							linePtr->endPosY = coordinate_endY * squareHeight;
						}
					}
				}
			}
		}

		for (int i = 0; i < characters.size(); i++)
		{
			double coordinate_startX = (characters[i]->posX / prevSquareWidth) - zoomEpsW;
			double coordinate_startY = (characters[i]->posY / prevSquareHeight) - zoomEpsH;

			characters[i]->posX = coordinate_startX * squareWidth;
			characters[i]->posY = coordinate_startY * squareHeight;
		}
	}

	scrollDotsW = amountOfDotsWidth;
	scrollDotsH = amountOfDotsHeight;
	step_size = 0.4;
	zoomEpsH = 0;
	zoomEpsW = 0;
	prevZoomEpsH = 0;
	prevZoomEpsW = 0;

	prevSquareWidth = squareWidth;
	prevSquareHeight = squareHeight;
}

bool Game::running()
{
	return isRunning;
}