#ifndef GameCharacter_hpp
#define GameCharacter_hpp

#include <iostream>
#include <cmath>
#include <string>
#include <SDL.h>
#include "Camera.h"

class GameCharacter
{
public:
	std::string name;

	int colorR;
	int colorG;
	int colorB;

	SDL_Scancode bound_key_forward;
	SDL_Scancode bound_key_backward;
	SDL_Scancode bound_key_left;
	SDL_Scancode bound_key_right;

	char key_pressed_rotation = '\0';
	char key_pressed = '\0';

	int map_pos_x;
	int map_pos_y;

	double posX;
	double posY;
	double squarePosX;
	double squarePosY;

	double dirX;
	double dirY;
	double planeX;
	double planeY;

	double walk;
	double rotation;

	std::string role;
	double fov;
	double raycast_length;

	double echo_length;
	double echo_speed;
	double echo_fov;

	Camera newCamera;
	bool cameraExists = false;
	bool echoExists = false;

	GameCharacter(std::string name, int colorR, int colorG, int colorB,
		double characterPosX, double characterPosY,
		double dirX, double dirY,
		std::string characterRole);

	GameCharacter(); // конструктор по умолчанию

	void createCamera();
	void drawCamera();
	void destroyCamera();

	void createEcho();
	void drawEcho();

	void setFOV(double field_of_view);

	~GameCharacter();
};

#endif /* GameCharacter_hpp */