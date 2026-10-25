#include "../headers/GameCharacter.h"
#include "../headers/TempParams.h"

GameCharacter::GameCharacter(std::string name, int colorR, int colorG, int colorB, double posX, double posY, double dirX, double dirY, std::string role) : newCamera()
{
	this->name = name;

	this->colorR = colorR;
	this->colorG = colorG;
	this->colorB = colorB;

	this->posX = posX;
	this->posY = posY;

	this->dirX = dirX;
	this->dirY = dirY;

	this->role = role;

	this->cameraExists = false;

	double local_eps_w = 0;
	double local_eps_h = 0;

	this->map_pos_x = int((characterX / squareWidth) - zoomEpsW);
	this->map_pos_y = int((characterY / squareHeight) - zoomEpsH);

	walk = walk_size;
	rotation = rotation_speed;

	raycast_length = 0;
}

GameCharacter::GameCharacter() : newCamera()
{
	this->name = "string";

	this->colorR = 255;
	this->colorG = 255;
	this->colorB = 255;

	this->posX = 0;
	this->posY = 0;

	this->dirX = 0;
	this->dirY = 0;

	this->role = "role";

	this->cameraExists = false;

	this->map_pos_x = 0;
	this->map_pos_y = 0;

	walk = 0;
	rotation = 0;

	raycast_length = 0;
}

void GameCharacter::createCamera()
{
	if (!cameraExists)
	{
		cameraExists = true;
	}
}

void GameCharacter::drawCamera()
{
	if (cameraExists)
	{
		double world_coord_x = (posX / squareWidth);
		double world_coord_y = (posY / squareHeight);

		newCamera.DDArayCasting(dirX, dirY, planeX, planeY, world_coord_x, world_coord_y, map_pos_x, map_pos_y, raycast_length);
	}
}

void GameCharacter::setFOV(double field_of_view)
{
	this->fov = field_of_view;
	this->planeX = dirY * fov;
    this->planeY = -dirX * fov;
}

void GameCharacter::destroyCamera()
{
	newCamera.clearCameraDots();
	this->fov = 0;
	this->raycast_length = 0;
}

void GameCharacter::createEcho()
{
	if (!echoExists)
	{
		echoExists = true;
	}
}

void GameCharacter::drawEcho()
{
	if (echoExists)
	{
		double world_coord_x = (posX / squareWidth);
		double world_coord_y = (posY / squareHeight);

		newCamera.DDAecho(dirX, dirY, world_coord_x, world_coord_y, echo_length, echo_speed, echo_fov, map_pos_x, map_pos_y);
	}
} 

GameCharacter::~GameCharacter()
{
	newCamera.clearCameraDots();
}
