#include "../headers/Camera.h"
#include "../headers/TempParams.h"
#include "cmath"
#include <thread>

void Camera::DDArayCasting(double dirX, double dirY, double planeX, double planeY, double player_x, double player_y, int player_map_x, int player_map_y, double max_dist)
{
	if (max_dist > 0)
	{
		// присваиваем значения переменных в локалки для потока, чтобы сохранить иммутабельность внутри потока
		double dirX_thread = dirX;
		double dirY_thread = dirY;
		double planeX_thread = planeX;
		double planeY_thread = planeY;
		double player_x_thread = player_x;
		double player_y_thread = player_y;
		int player_map_x_thread = player_map_x;
		int player_map_y_thread = player_map_y;
		double max_dist_thread = max_dist;

		// запускаем поток через лямбда выражение
		std::thread t([=]()
		{
			this->DDAraycastingThread(
				dirX_thread, dirY_thread, planeX_thread, planeY_thread,
				player_x_thread, player_y_thread,
				player_map_x_thread, player_map_y_thread,
				max_dist_thread
			);
		});

		/*DDAraycastingThread(
			dirX_thread, dirY_thread, planeX_thread, planeY_thread,
			player_x_thread, player_y_thread,
			player_map_x_thread, player_map_y_thread,
			max_dist_thread
		);*/

		for (int x = 0; x < int(img_w / 2); x++)
		{
			double cameraX = 2 * x / (double)img_w - 1;
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;

			int hit = 0;

			double sideDistX = 0.00;
			double sideDistY = 0.00;

			double deltaDistX = 0.00;
			double deltaDistY = 0.00;

			double perpWallDist = 0.00;
			int side = 0;

			double total_line_length = max_dist * squareWidth;
			double dot_start_pos_x = player_x;
			double dot_start_pos_y = player_y;
			int dot_map_x = player_map_x;
			int dot_map_y = player_map_y;

			double allPerpWallDist = 0.0;

			while (total_line_length > epsilon)
			{
				DDArayCounting(rayDirX, rayDirY, sideDistX, sideDistY, deltaDistX, deltaDistY, side, hit,
					dot_start_pos_x, dot_start_pos_y, dot_map_x, dot_map_y, total_line_length, dirX, dirY, x, cameraDotsLeft, allPerpWallDist);
			}

			if (hit != 1)
			{
				if (side == 0)
				{
					perpWallDist = (sideDistX - deltaDistX);
					if (perpWallDist > max_dist)
					{
						perpWallDist = max_dist;
					}
				}
				else
				{
					perpWallDist = (sideDistY - deltaDistY);
					if (perpWallDist > max_dist)
					{
						perpWallDist = max_dist;
					}
				}
				double dotX = (player_x + (max_dist * rayDirX)) * squareWidth;
				double dotY = (player_y + (max_dist * rayDirY)) * squareHeight;

				if (cameraDotsLeft.size() >= (x + 1))
				{
					cameraDotsLeft[x].emplace_back(dotX, dotY, 0.0, false);
				}
				else
				{
					std::vector<CameraDot> temp_arr;
					temp_arr.emplace_back(dotX, dotY, 0.0, false);

					cameraDotsLeft.push_back(temp_arr);
				}
			}
		}

		// тоже подумать как оптимизировать, чтобы не ждать пока поток отработает, а сразу вставлять в массив
		t.join();

		cameraDots.insert(cameraDots.end(),
			cameraDotsLeft.begin(),
			cameraDotsLeft.end());

		cameraDots.insert(cameraDots.end(),
			cameraDotsRight.begin(),
			cameraDotsRight.end());

		cameraDotsLeft.clear();
		cameraDotsRight.clear();
	}
}

// аналогичный DDAraycasting метод, но заточенный на работу в отдельном потоке
void Camera::DDAraycastingThread(double dirX, double dirY, double planeX, double planeY, double player_x, double player_y, int player_map_x, int player_map_y, double max_dist)
{
	for (int x = 0; x < int(img_w / 2); x++)
	{
		double cameraX = 2 * (x + int(img_w / 2)) / (double)img_w - 1;
		double rayDirX = dirX + planeX * cameraX;
		double rayDirY = dirY + planeY * cameraX;

		int hit = 0;

		double sideDistX = 0.00;
		double sideDistY = 0.00;

		double deltaDistX = 0.00;
		double deltaDistY = 0.00;

		double perpWallDist = 0.00;
		int side = 0;

		double total_line_length = max_dist * squareWidth;
		double dot_start_pos_x = player_x;
		double dot_start_pos_y = player_y;
		int dot_map_x = player_map_x;
		int dot_map_y = player_map_y;

		double allPerpWallDist = 0.0;

		while (total_line_length > epsilon)
		{
			DDArayCounting(rayDirX, rayDirY, sideDistX, sideDistY, deltaDistX, deltaDistY, side, hit,
				dot_start_pos_x, dot_start_pos_y, dot_map_x, dot_map_y, total_line_length, dirX, dirY, x, cameraDotsRight, allPerpWallDist);
		}

		if (hit != 1)
		{
			if (side == 0)
			{
				perpWallDist = (sideDistX - deltaDistX);
				if (perpWallDist > max_dist)
				{
					perpWallDist = max_dist;
				}
			}
			else
			{
				perpWallDist = (sideDistY - deltaDistY);
				if (perpWallDist > max_dist)
				{
					perpWallDist = max_dist;
				}
			}
			double dotX = (player_x + (max_dist * rayDirX)) * squareWidth;
			double dotY = (player_y + (max_dist * rayDirY)) * squareHeight;

			if (cameraDotsRight.size() >= (x + 1))
			{
				cameraDotsRight[x].emplace_back(dotX, dotY, 0.0, false);
			}
			else
			{
				std::vector<CameraDot> temp_arr;
				temp_arr.emplace_back(dotX, dotY, 0.0, false);

				cameraDotsRight.push_back(temp_arr);
			}
		}
	}
}

void Camera::DDArayCounting(double& rayDirX, double& rayDirY, double& sideDistX, double& sideDistY, 
	double& deltaDistX, double& deltaDistY, int& side, int& hit, double& dot_start_x, double& dot_start_y, int& dot_map_x, int& dot_map_y, double& max_dist, double& dirX, double& dirY, int ray_num, std::vector<std::vector<CameraDot>>& cameraDots_local, double & allPerpWallDist, bool echo)
{
	deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
	deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

	int stepX;
	int stepY;

	int current_steps_x = 0;
	int current_steps_y = 0;

	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = ((dot_start_x - zoomEpsW) - dot_map_x) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (dot_map_x + 1 - (dot_start_x - zoomEpsW)) * deltaDistX;
	}

	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = ((dot_start_y - zoomEpsH) - dot_map_y) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (dot_map_y + 1 - (dot_start_y - zoomEpsH)) * deltaDistY;
	}

	// дополнительная проверка на side
	if (sideDistX < sideDistY)
	{
		side = 0;
	}
	else
	{
		side = 1;
	}

	double tempPerpWallDist = 0.0;

	while (max_dist > epsilon)
	{
		if (dot_map_y >= 0 && dot_map_y <= squares_amount_y - 2 &&
			dot_map_x >= 0 && dot_map_x <= squares_amount_x - 2 &&
			!squareMapArray[dot_map_y][dot_map_x].isEmpty)
		{
			if (side == 0)
			{
				tempPerpWallDist = (sideDistX - deltaDistX);
			}
			else
			{
				tempPerpWallDist = (sideDistY - deltaDistY);
			}

			double start_x = (dot_start_x * squareWidth);
			double start_y = (dot_start_y * squareHeight);

			double dotX = 0.0;
			double dotY = 0.0;

			if (tempPerpWallDist < 0)
			{
				dotX = start_x;
				dotY = start_y;
			}
			else
			{
				dotX = start_x + (tempPerpWallDist * rayDirX * squareWidth);
				dotY = start_y + (tempPerpWallDist * rayDirY * squareHeight);
			}

			LineMap* line_hit_ptr = nullptr;

			CameraDot obj = getHitDist(dotX, dotY, rayDirX, rayDirY, tempPerpWallDist, max_dist, dot_start_x, dot_start_y, dot_map_x, dot_map_y, dirX, dirY, line_hit_ptr);
			
			dotX = obj.posX;
			dotY = obj.posY;

			if (dotX != -1 && dotY != -1)
			{
				if (echo)
				{
					echo_posX = dotX;
					echo_posY = dotY;

					hit = 1;
				}
				else if (max_dist > epsilon)
				{
					// если tempPerpWallDist отрицательный => линия находится с игроком в одном квадрате
					if (tempPerpWallDist < 0)
					{
						tempPerpWallDist = (obj.wallHeight / squareWidth);
					}
					else
					{
						tempPerpWallDist += obj.wallHeight / squareWidth;
					}

					// прибавлемя полученное от текущей точки расстояние к общей длине луча для опрделения высоты стены
					allPerpWallDist += tempPerpWallDist;

					int wallHeight = int(img_h / allPerpWallDist);

					if (cameraDots_local.size() >= (ray_num + 1))
					{
						cameraDots_local[ray_num].emplace_back(dotX, dotY, wallHeight, true);

						// получение ссылки в точку на вектор стены
						if (line_hit_ptr)
						{
							cameraDots_local[ray_num][cameraDots_local[ray_num].size() - 1].line_ptr = line_hit_ptr;
						}
					}
					else
					{
						std::vector<CameraDot> temp_arr;
						temp_arr.emplace_back(dotX, dotY, wallHeight, true);

						// получение ссылки в точку на вектор стены
						if (line_hit_ptr)
						{
							temp_arr[temp_arr.size() - 1].line_ptr = line_hit_ptr;
						}

						cameraDots_local.push_back(temp_arr);
					}

					if (std::fabs(rayDirX) >= 0.0001)
					{
						dotX = ((dotX + (rayDirX * 0.1)));
					}

					if (std::fabs(rayDirY) >= 0.0001)
					{
						dotY = ((dotY + (rayDirY * 0.1)));
					}

					dot_start_x = dotX / squareWidth;
					dot_start_y = dotY / squareHeight;

					double map_hit_x = (dotX - (zoomEpsW * squareWidth)) / squareWidth;
					double map_hit_y = (dotY - (zoomEpsH * squareHeight)) / squareHeight;

					dot_map_x = int(map_hit_x);
					dot_map_y = int(map_hit_y); 
 					
					hit = 1;
				}

				break;
			}
		}

		if ((sideDistX - deltaDistX > (max_dist / squareWidth)) || (sideDistY - deltaDistY) > (max_dist / squareWidth))
		{
			max_dist = 0.0;
			break;
		}

		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			dot_map_x += stepX;

			current_steps_x += 1;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			dot_map_y += stepY;

			current_steps_y += 1;
			side = 1;
		}
	}
}

void Camera::DDAecho(double dirX, double dirY, double player_x, double player_y, double max_echo_dist, double &echo_rotation, double echo_fov, int player_map_x, int player_map_y)
{
	if (max_echo_dist > 0)
	{
		echo_dirX = dirX;
		echo_dirY = dirY;

		if (rotation_sum >= echo_fov)
		{
			echo_rotation = -std::abs(echo_rotation);
		}
		else if (rotation_sum <= -echo_fov)
		{
			echo_rotation = std::abs(echo_rotation);
		}

		double oldDirX = echo_dirX;
		echo_dirX = dirX * cos(rotation_sum) - dirY * sin(rotation_sum);
		echo_dirY = oldDirX * sin(rotation_sum) + dirY * cos(rotation_sum);

		rotation_sum += echo_rotation;

		double rayDirX = echo_dirX;
		double rayDirY = echo_dirY;

		int mapX = 0;
		int mapY = 0;
		int hit = 0;

		double sideDistX = 0.00;
		double sideDistY = 0.00;

		double deltaDistX = 0.00;
		double deltaDistY = 0.00;

		double perpWallDist = 0.00;
		int side = 0;

		double temp_max_echo_dist = max_echo_dist * squareWidth;

		DDArayCounting(rayDirX, rayDirY, sideDistX, sideDistY, deltaDistX, deltaDistY, side, hit, player_x, player_y, player_map_x, player_map_y, temp_max_echo_dist, dirX, dirY, 0, cameraDots, perpWallDist, true);

		if (hit != 1)
		{
			if (side == 0)
			{
				perpWallDist = (sideDistX - deltaDistX);
				if (perpWallDist > max_echo_dist)
				{
					perpWallDist = max_echo_dist;
				}
			}
			else
			{
				perpWallDist = (sideDistY - deltaDistY);
				if (perpWallDist > max_echo_dist)
				{
					perpWallDist = max_echo_dist;
				}
			}
			double dotX = (player_x + (perpWallDist * rayDirX)) * squareWidth;
			double dotY = (player_y + (perpWallDist * rayDirY)) * squareHeight;

			echo_posX = dotX;
			echo_posY = dotY;
		}
	}
	else
	{
		echo_posX = player_x * squareWidth;
		echo_posY = player_y * squareHeight;
	}
}

void Camera::reserveMemmory()
{
	cameraDots.resize(img_w);

	cameraDotsLeft.resize(img_w / 2);
	cameraDotsRight.resize(img_w / 2);

	for (int i = 0; i < img_w; i++)
	{
		cameraDots[i].reserve(max_camera_dots_reserved);

		if (i >= (img_w / 2))
		{
			cameraDotsLeft[i - (img_w / 2)].reserve(max_camera_dots_reserved);
		}
		else
		{
			cameraDotsRight[i].reserve(max_camera_dots_reserved);
		}
	}
}

void Camera::clearReservedMemmory()
{
	for (int i = 0; i < cameraDots.size(); i++)
	{
		cameraDots[i].clear();
		cameraDots[i].shrink_to_fit();

		if (i >= (cameraDots.size() / 2))
		{
			cameraDotsLeft[i - (cameraDots.size() / 2)].clear();
			cameraDotsLeft[i - (cameraDots.size() / 2)].shrink_to_fit();
		}
		else
		{
			cameraDotsRight[i].clear();
			cameraDotsRight[i].shrink_to_fit();
		}
	}
}

void Camera::clearCameraDots()
{ 
	if (cameraDots.size() != 0) 
	{ 
		for (int i = 0; i < cameraDots.size(); i++) 
		{
			cameraDots[i].clear(); 
		} 
		cameraDots.clear(); 
	} 
}

CameraDot Camera::getHitDist(double dotX, double dotY, double rayDirX, double rayDirY, double perpWallDist, double& max_dist, double player_x, double player_y, int mapX, int mapY, double dirX, double dirY, LineMap*& line_hit_ptr)
{
	double hitX = -1;
	double hitY = -1;

	double dist = 0;

	bool found = false;

	if (perpWallDist * squareWidth >= max_dist)
	{
		max_dist = 0.0;
	}
	else
	{
		for (int i = 0; i < squareMapArray[mapY][mapX].linesList.size(); i++)
		{
			auto linePtr = squareMapArray[mapY][mapX].linesList[i];

			if (linePtr != nullptr)
			{
				double& line_startX = linePtr->startPosX;
				double& line_startY = linePtr->startPosY;
				double& line_endX = linePtr->endPosX;
				double& line_endY = linePtr->endPosY;
				double& line_dirX = linePtr->dirX;
				double& line_dirY = linePtr->dirY;

				double line_difX = (line_endX - line_startX);
				double line_difY = (line_endY - line_startY);

				double determinant = (rayDirX * line_difY) - (rayDirY * line_difX);
				if (fabs(determinant) < 1e-9) continue; // параллельные

				double delta_t = ((line_startX - dotX) * line_difY) - ((line_startY - dotY) * line_difX);
				double delta_u = (rayDirY * (line_startX - dotX)) - (rayDirX * (line_startY - dotY));

				double t = delta_t / determinant;
				double u = delta_u / determinant;

				if (u < 0.0 || u > 1.0) continue;

				double newHitX = dotX + (t * rayDirX);
				double newHitY = dotY + (t * rayDirY);

				//находим координаты вектора для дальнейшего определения его направления
				double vx = newHitX - (player_x * squareWidth);
				double vy = newHitY - (player_y * squareHeight);

				//dirX и dirY воспринимаем как координаты нормализованного вектора чего достаточно для определения косинуса между векторами
				double dot = vx * rayDirX + vy * rayDirY;

				//если меньше 0 => угол больше 90 градусов и меньше 270 градусов ввиду отражения
				//и справделиво сказать что точка лежит за полем зрения игрока

				if (dot < 0)
				{
					continue;
				}

				double dist_x = (t * dirX);
				double dist_y = (t * dirY);

				double newDist = dist_x * dist_x + dist_y * dist_y;

				if (!found || newDist < dist)
				{
					hitX = newHitX;
					hitY = newHitY;
					dist = newDist;

					if (fabs(rayDirX - dirX) < epsilon && fabs(rayDirY - dirY) < epsilon)
					{
						line_start_x = line_startX;
						line_start_y = line_startY;
						line_end_x = line_endX;
						line_end_y = line_endY;
					}

					found = true;

					// при соударении присваиваем линию соударения к точке соударения
					line_hit_ptr = linePtr->linePtr;
				}
			}
		}

		dist = sqrt(dist);

		double distance_from_player = (perpWallDist * squareWidth) + dist;

		if (distance_from_player >= max_dist)
		{
			hitX = -1;
			hitY = -1;

			max_dist = 0.0;
		}
		else if (hitX != -1 && hitY != -1)
		{
			max_dist = max_dist - distance_from_player;
		}
	}

	CameraDot newObj = CameraDot(hitX, hitY, dist, false);
	return newObj;
}

Camera::Camera()
{
}