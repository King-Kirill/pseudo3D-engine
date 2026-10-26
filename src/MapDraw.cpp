#include "../headers/MapDraw.h"
#include "../headers/SquareMap.h"
#include "../headers/ConstParams.h"
#include "../headers/TempParams.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "../headers/LineMap.h"
#include "../headers/PathEndPosition.h"
#include <map>
#include <string>
#include <thread>

MapDraw::MapDraw(SDL_Renderer*& renderer)
{
	this->renderer = renderer;
}

MapDraw::~MapDraw()
{
    clearGridDotsArray();
}

void MapDraw::drawMap()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (gridDotsArray != nullptr)
    {
        clearGridDotsArray();
    }

    gridDotsArray = new GridDots * [int(amountOfDotsHeight)];

    for (int i = 0; i < int(amountOfDotsHeight); i++)
    {
        gridDotsArray[i] = new GridDots[int(amountOfDotsWidth)];

        for (int j = 0; j < int(amountOfDotsWidth); j++)
        {
            int squarePosX = 0;
            int squarePosY = 0;
            int squareRadius = 0;

            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

            drawCircle(((j + zoomEpsW) * squareWidth), ((i + zoomEpsH) * squareHeight), dotRadius, squarePosX, squarePosY, squareRadius);
            gridDotsArray[i][j] = GridDots(((j + zoomEpsW) * squareWidth), ((i + zoomEpsH) * squareHeight), squarePosX, squarePosY, squareRadius);
        }
    }

    if (vectorsMapArray.size() != 0)
    {
        drawMapLines(int(amountOfDotsWidth), int(amountOfDotsHeight));
    }

    drawMapEdges();
}

// ортисовка мини карты
void MapDraw::drawMapSmall()
{
    if (gridDotsArray != nullptr)
    {
        clearGridDotsArray();
    }

    if (vectorsMapArray.size() != 0)
    {
        drawMapLines(int(amountOfDotsWidth), int(amountOfDotsHeight));
    }
}

void MapDraw::drawCircle(double centerX, double centerY, double radius, int & squarePosX, int & squarePosY, int & squareRadius)
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
        LineMap& drawinLine = *vectorsMapArray[i];

        drawLine(drawinLine.startPosX + mini_map_differ_x, drawinLine.startPosY + mini_map_differ_y, drawinLine.endPosX + mini_map_differ_x, drawinLine.endPosY + mini_map_differ_y, drawinLine.lineColorR, drawinLine.lineColorG, drawinLine.lineColorB);
    }
}

void MapDraw::drawMapEdges()
{
    drawLine((0 + zoomEpsW) * squareWidth, (0 + zoomEpsH) * squareHeight, (int(amountOfDotsWidth) + zoomEpsW) * squareWidth, (0 + zoomEpsH) * squareHeight, 255, 255, 255);
    drawLine((0 + zoomEpsW) * squareWidth, (0 + zoomEpsH) * squareHeight, (0 + zoomEpsW) * squareWidth, (int(amountOfDotsHeight) + zoomEpsH) * squareHeight, 255, 255, 255);
    drawLine((int(amountOfDotsWidth) + zoomEpsW) * squareWidth, (0 + zoomEpsH) * squareHeight, (int(amountOfDotsWidth) + zoomEpsW) * squareWidth, (int(amountOfDotsHeight) + zoomEpsH) * squareHeight, 255, 255, 255);
    drawLine((0 + zoomEpsW) * squareWidth, (int(amountOfDotsHeight) + zoomEpsH) * squareHeight, (int(amountOfDotsWidth) + zoomEpsW) * squareWidth, (int(amountOfDotsHeight) + zoomEpsH) * squareHeight, 255, 255, 255);
}

// для миникарты
void MapDraw::drawMapEdgesSmall()
{
    drawLine((img_w - dev_zone_w - additional_window_w), 0, img_w - dev_zone_w, 0, 255, 255, 255);
    drawLine((img_w - dev_zone_w - additional_window_w), 0, (img_w - dev_zone_w - additional_window_w), additional_window_h + 1, 255, 255, 255);
    drawLine((img_w - dev_zone_w - additional_window_w), additional_window_h - 1, (img_w - dev_zone_w), additional_window_h - 1, 255, 255, 255);
    drawLine((img_w - dev_zone_w - 1), 0, (img_w - dev_zone_w - 1), additional_window_h, 255, 255, 255);
}


void MapDraw::drawLine(double startX, double startY, double endX, double endY, int colorRed, int colorGreen, int colorBlue)
{
    SDL_SetRenderDrawColor(renderer, colorRed, colorGreen, colorBlue, 255);
    SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
}


void MapDraw::createCharacter(std::string characterName, int redColor, int greenColor, int blueColor, double characterPosX, double characterPosY, double dirX, double dirY)
{
    GameCharacter* newCharacter = new GameCharacter(characterName, redColor, greenColor, blueColor, characterPosX, characterPosY, dirX, dirY, currCharRole);
    characters.push_back(newCharacter);

    if (currCharRole == "main")
    {
        player = newCharacter;
        currCharRole = "enemy";
    }
}

void MapDraw::changeCharColor(int redColor, int greenColor, int blueColor, std::string characterName)
{
    if (characters.size() != 0)
    {
        for (int i = 0; i < characters.size(); i++)
        {
            if (characters[i]->name == characterName)
            {
                characters[i]->colorR = redColor;
                characters[i]->colorG = greenColor;
                characters[i]->colorR = blueColor;
            }
        }
    }
    else
    {
        std::cout << "error: array of textures is empty!" << std::endl;
    }
}

void MapDraw::changeCharName(std::string characterName, std::string newCharacterName)
{
    if (characters.size() != 0)
    {
        for (int i = 0; i < characters.size(); i++)
        {
            if (characters[i]->name == characterName)
            {
                characters[i]->name = newCharacterName;
            }
        }
    }
    else
    {
        std::cout << "error: array of textures is empty!" << std::endl;
    }
}

void MapDraw::changeCharPos(double characterPosX, double characterPosY, std::string characterName)
{
    if (characters.size() != 0)
    {
        for (int i = 0; i < characters.size(); i++)
        {
            if (characters[i]->name == characterName)
            {
                characters[i]->posX = characterPosX;
                characters[i]->posY = characterPosY;
            }
        }
    }
    else
    {
        std::cout << "error: array of textures is empty!" << std::endl;
    }
}

void MapDraw::changeCharRole(std::string newRole, std::string characterName)
{
    if (characters.size() != 0)
    {
        for (int i = 0; i < characters.size(); i++)
        {
            if (characters[i]->name == characterName)
            {
                characters[i]->role = newRole;
            }
        }
    }
    else
    {
        std::cout << "error: aray of textures is empty!" << std::endl;
    }
}

void MapDraw::drawCharacters()
{
    if (characters.size() != 0)
    {
        for (int i = 0; i < characters.size(); i++)
        {
            if (characters[i]->cameraExists)
            {
                characters[i]->newCamera.clearCameraDots();
                characters[i]->drawCamera();

                if (characters[i]->newCamera.cameraDots.size() != 0)
                {
                    std::vector<Sint16> vx(img_w + 1);
                    std::vector<Sint16> vy(img_w + 1);

                    for (int j = 0; j < characters[i]->newCamera.cameraDots.size(); j++)
                    {
                        if (characters[i]->newCamera.cameraDots[j].size() > 0)
                        {
                            vx[j] = static_cast<Sint16>(characters[i]->newCamera.cameraDots[j][0].posX + mini_map_differ_x);
                            vy[j] = static_cast<Sint16>(characters[i]->newCamera.cameraDots[j][0].posY + mini_map_differ_y);
                        }
                        else
                        {
                            if (selected_engine_window != "map")
                            {
                                vx[j] = static_cast<Sint16>(mini_map_center_x);
                                vy[j] = static_cast<Sint16>(mini_map_center_y);
                            }
                            else
                            {
                                vx[j] = static_cast<Sint16>(characters[i]->posX + mini_map_differ_x);
                                vy[j] = static_cast<Sint16>(characters[i]->posY + mini_map_differ_y);
                            }
                        }
                    }

                    if (selected_engine_window != "map")
                    {
                        vx[img_w] = static_cast<Sint16>(mini_map_center_x);
                        vy[img_w] = static_cast<Sint16>(mini_map_center_y);
                    }
                    else
                    {
                        vx[img_w] = static_cast<Sint16>(characters[i]->posX + mini_map_differ_x);
                        vy[img_w] = static_cast<Sint16>(characters[i]->posY + mini_map_differ_y);
                    }
                    
                    filledPolygonRGBA(renderer, vx.data(), vy.data(), img_w + 1, 0, 255, 0, 255);
                }
            }

            if (characters[i]->echoExists)
            {
                characters[i]->drawEcho();
                drawLine(characters[i]->posX + mini_map_differ_x, characters[i]->posY + mini_map_differ_y, characters[i]->newCamera.echo_posX + mini_map_differ_x, characters[i]->newCamera.echo_posY + mini_map_differ_y, 255, 0, 0);
            }

            if(selected_engine_window == "map")
            { 
                filledCircleRGBA(renderer, characters[i]->posX + mini_map_differ_x, characters[i]->posY + mini_map_differ_y, charRadius, characters[i]->colorR, characters[i]->colorG, characters[i]->colorB, 255);

                if (characters[i]->role == "main")
                {
                    circleRGBA(renderer, characters[i]->posX + mini_map_differ_x, characters[i]->posY + mini_map_differ_y, charRadius, 255, 255, 255, 255);
                }
            }
            else
            {
                if (characters[i]->role != "main")
                {
                    filledCircleRGBA(renderer, characters[i]->posX + mini_map_differ_x, characters[i]->posY + mini_map_differ_y, charRadius, characters[i]->colorR, characters[i]->colorG, characters[i]->colorB, 255);
                }
            }
        }

        if (selected_engine_window != "map")
        {
            if (player != nullptr)
            {
                filledCircleRGBA(renderer, mini_map_center_x, mini_map_center_y, charRadius, player->colorR, player->colorG, player->colorB, 255);
                circleRGBA(renderer, mini_map_center_x, mini_map_center_y, charRadius, 255, 255, 255, 255);
            }
        }
    }
    else 
    {
        std::cout << "error: aray of textures is empty!" << std::endl;
    }
}

void MapDraw::drawGame(std::string selected_engine_window)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (player != nullptr)
    {
        // отрисовываем только каждую n-ную линию в соответсвии с разрешением миникарты
		int draw_step_counter = 0;
		int x_offset = 0;

        for (int i = 0; i < player->newCamera.cameraDots.size(); i++)
        {
            for (int j = player->newCamera.cameraDots[i].size() - 1; j >= 0; j--)
            {
                if (player->newCamera.cameraDots[i][j].hit == true)
                {
                    if (selected_engine_window == "map")
                    {
                        if (draw_step_counter >= draw_step)
                        {
                            auto temp_line_ptr = player->newCamera.cameraDots[i][j].line_ptr;

                            int drawStart = ( - player->newCamera.cameraDots[i][j].wallHeight * 0.5) / 2 + additional_window_h / 2;
                            if (drawStart < 0)drawStart = 0;
                            int drawEnd = (player->newCamera.cameraDots[i][j].wallHeight * 0.5) / 2 + additional_window_h / 2;
                            if (drawEnd >= additional_window_h)drawEnd = additional_window_h - 1;

                            int start_x = ((img_w - dev_zone_w - additional_window_w) + x_offset);

                            drawLine(start_x, drawStart, start_x, drawEnd, temp_line_ptr->lineColorR, temp_line_ptr->lineColorG, temp_line_ptr->lineColorB);
                        }
                    }
					else if (selected_engine_window == "game")
                    {
                        auto temp_line_ptr = player->newCamera.cameraDots[i][j].line_ptr;

                        int drawStart = -player->newCamera.cameraDots[i][j].wallHeight / 2 + img_h / 2;
                        if (drawStart < 0)drawStart = 0;
                        int drawEnd = player->newCamera.cameraDots[i][j].wallHeight / 2 + img_h / 2;
                        if (drawEnd >= img_h)drawEnd = img_h - 1;

                        drawLine(player->newCamera.cameraDots.size() - i, drawStart, player->newCamera.cameraDots.size() - i, drawEnd, temp_line_ptr->lineColorR, temp_line_ptr->lineColorG, temp_line_ptr->lineColorB);
                    }

                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
            }

            if (selected_engine_window == "map")
            {
                if (draw_step_counter >= draw_step)
                {
                    draw_step_counter = 0;
                    x_offset++;
                }
                else
                {
                    draw_step_counter++;
                }
            }
        }
    }
}

// отдельный метод для определения изменения позиции игрока на мини карте
void MapDraw::countMiniMapCenterDiffer()
{
    if (player != nullptr)
    {
        if (selected_engine_window == "map")
        {
            mini_map_differ_x = 0;
            mini_map_differ_y = 0;
        }
        else
        {
            mini_map_differ_x = mini_map_center_x - player->posX;
            mini_map_differ_y = mini_map_center_y - player->posY;
        }
    }
}

bool MapDraw::checkCharName(std::string Name)
{
    for (int i = 0; i < characters.size(); i++)
    {
        if (characters[i]->name == Name)
        {
            return false;
        }
    }
    return true;
}

bool MapDraw::checkCharRole(std::string Role)
{
    for (int i = 0; i < characters.size(); i++)
    {
        if (characters[i]->role == Role)
        {
            return false;
        }
    }
    return true;
}

GameCharacter* MapDraw::getGameCharacter(double posX, double posY)
{
    for (int i = 0; i < characters.size(); i++)
    {
        if(((characters[i]->posX + charRadius >= posX) && (characters[i]->posX - charRadius <= posX))
            && ((characters[i]->posY + charRadius >= posY) && (characters[i]->posY - charRadius <= posY)))
        {
            return characters[i];
        }
    }
    return nullptr;
}