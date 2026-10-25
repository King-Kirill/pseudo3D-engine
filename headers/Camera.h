#ifndef Camera_hpp
#define Camera_hpp

#include <iostream>
#include "ConstParams.h"
#include "CameraDot.h"
#include <vector>
#include <mutex>

class Camera
{
public:
	double echo_dirX = NAN;
	double echo_dirY = NAN;

	//current echo dot pos
	double echo_posX;
	double echo_posY;

	double rotation_sum = 0.0;
	// массив основного потока
	std::vector<std::vector<CameraDot>> cameraDotsLeft;
	// массив дочернего потока
	std::vector<std::vector<CameraDot>> cameraDotsRight;
	// общий массив точек (основной)
	std::vector<std::vector<CameraDot>> cameraDots;

	Camera();

	void DDArayCasting(double dirX, double dirY, double planeX, double planeY, double player_x, double player_y, int player_map_x, int player_map_y, double max_dist);
	void DDAraycastingThread(double dirX, double dirY, double planeX, double planeY, double player_x, double player_y, int player_map_x, int player_map_y, double max_dist);
	void DDArayCounting(double& rayDirX, double& rayDirY, double& sideDistX, double& sideDistY, double& deltaDistX, double& deltaDistY, int& side, int& hit, double& dot_start_x, double& dot_start_y, int& dot_map_x, int& dot_map_y, double& max_dist, double& dirX, double& dirY, int ray_num, std::vector<std::vector<CameraDot>>& cameraDots_local, double& allPerpWallDist, bool echo = false);
	void DDAecho(double dirX, double dirY, double player_x, double player_y, double max_echo_dist, double &echo_rotation, double echo_fov, int player_map_x, int player_map_y);
	void clearCameraDots();
	CameraDot getHitDist(double dotX, double dotY, double rayDirX, double rayDirY, double peprWallDist, double &max_dist, double player_x, double player_y, int mapX, int mapY, double dirX, double dirY, std::weak_ptr<LineMap>& line_hit_ptr);
};

#endif /* Camera_hpp */