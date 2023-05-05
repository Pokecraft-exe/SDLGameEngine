#include <iostream>
#include "game.h"
using namespace std;

class GameEngine : public SDLGameEngine {
public:
	mesh axis;
	mesh suzanne;
	float fTheta;
	camera cam;
	light point;

	bool OnUserCreates() {

		axis = getMesh("axis.obj");
		suzanne = getMesh("suzanne.obj");

		cam = camera(screen.SCREEN_HEIGHT, screen.SCREEN_WIDTH);
		point.orientation = { 0.0f, 0.0f, -1.0f };

		moveMeshZ(&axis, 13.0f);
		moveMeshZ(&suzanne, 8.0f);
		rotateMeshZ(&suzanne, 180);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {

		screen.clear();

		fTheta += 2.0f * fElapsedTime;

		while (SDL_PollEvent(&screen.e)) {
			if (screen.e.type == SDL_QUIT) { quit = true; };
			if (screen.e.type == SDL_KEYDOWN) {
				if (screen.e.key.keysym.scancode == 82) {
					cam.position.y += 8.0f * fElapsedTime;
				}
				else if (screen.e.key.keysym.scancode == 81) {
					cam.position.y -= 8.0f * fElapsedTime;
				}
				else if (screen.e.key.keysym.scancode == 80) {
					cam.position.x += 8.0f * fElapsedTime;
				}
				else if (screen.e.key.keysym.scancode == 79) {
					cam.position.x -= 8.0f * fElapsedTime;
				}
				else if (screen.e.key.keysym.scancode == 26) {
					continue;//cam.position = cam.position * (8.0f * fElapsedTime); // Z
				}
				else if (screen.e.key.keysym.scancode == 4) {
					cam.RotationY -= 2.0f * fElapsedTime; // Q
				}
				//else if (screen.e.key.keysym.scancode == 22) {
				//	cam.position.x += 8.0f * fElapsedTime; // S
				//}
				else if (screen.e.key.keysym.scancode == 7) {
					cam.RotationY += 2.0f * fElapsedTime; // D
				}
				else {
					cout << "Key \"" << screen.e.key.keysym.scancode << "\" pressed !" << endl;
				}
			}
		}

		rotateMeshY(&suzanne, fTheta * 5.0f);

		renderMesh(&axis, &cam, &point);
		renderMesh(&suzanne, &cam, &point);

		screen.update();
		return true;
	}
};

int main(int argc, char* args[])
{
	GameEngine game;

	game.CreateNewScreen("Hello World", 1024, 720);
	game.screen.setIcon("icon.bmp");

	game.start();

	return 0;
}
