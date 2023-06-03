#include <iostream>
#include "game.h"
#include "window.h"
using namespace std;

/*

to do:
polygon instead of triangles,
fix sort with camera ray
don't render polygons off screen,
draw when render,
textures,
materials,
raytracing,
clean and optimize,
GPU threads,
animations

*/

int change(void* ptr);

class GameEngine : public SDLGameEngine {
public:
	void* _this;
	mesh axis;
	mesh cube;
	mesh suzanne;
	float fTheta;
	camera cam;
	light point;
	Button* b1;
	bool alt;
	Image i;

	bool OnUserCreates() {

		axis = getMesh("Oaxis.obj");
		cube = getMesh("poly_suzanne.obj");
		//suzanne = getMesh("suzanne.obj");

		cam = camera(screen.SCREEN_HEIGHT, screen.SCREEN_WIDTH);
		point.orientation = { 0.0f, 0.0f, -1.0f };

		//moveMeshZ(&axis, 12.0f);
		cube.origin.z += 12;
		b1 = screen.addButton("Change", 30, 100, 60, 20, 0xff0066, change, _this);

		i = Image("lettuce.png", 0, 0);
		//rotateMeshZ(&suzanne, 180);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {

		//screen.clear();
		screen.fill(0x00FFFF);

		fTheta += 2.0f * fElapsedTime;

		while (screen.PollEvent()) {
			if (screen.e.type == QUIT) {
				quit = true; 
			}
			else if (screen.e.type == KEYDOWN) {
				if (screen.e.key.keysym.scancode == 41) { quit = true; }
				else if (screen.e.key.keysym.scancode == 82) {
					vec3d vForward = Vector_Mul(cam._pointAt, 8.0f * fElapsedTime);
					cam.position = Vector_Add(cam.position, vForward);
				}
				else if (screen.e.key.keysym.scancode == 81) {
					vec3d vForward = Vector_Mul(cam._pointAt, 8.0f * fElapsedTime);
					cam.position = Vector_Sub(cam.position, vForward);
				}
				else if (screen.e.key.keysym.scancode == 226) {
					alt = true;
					screen.ShowCursor();
				}
				else {
					cout << "Key \"" << screen.e.key.keysym.scancode << "\" pressed !" << endl;
				}
			}
			else if (screen.e.type == KEYUP) {
				if (screen.e.key.keysym.scancode == 226) {
					alt = false;
					screen.HideCursor();
				}
			}
			else if (screen.e.type = MOUSEMOTION && !alt) {
				cube.RotationZ -= screen.e.motion.xrel * 0.5f * fElapsedTime;
				cube.RotationY += screen.e.motion.yrel * 0.5f * fElapsedTime;
				screen.setMousePos(screen.SCREEN_WIDTH / 2, screen.SCREEN_HEIGHT / 2);
				//cout << "mouse moved in y : " << screen.e.motion.yrel << endl;
			}
		}

		//rotateMeshX(&cube, fTheta * 10);
		//renderMesh(&cube);
		//renderMesh(cube);
		//renderMesh(&suzanne);

		render(&cube, &cam, &point);

		i.x++;
		i.y++;
		if (i.x >= screen.SCREEN_WIDTH) i.x = 0;
		if (i.y >= screen.SCREEN_HEIGHT) i.y = 0;
		screen.RenderImage(i);

		screen.DrawString("FPS: " + to_string(fps()), 10, 5, 0xff6666, 2);

		screen.DrawButtons();

		screen.update();
		return true;
	}
};

int change(void* ptr) {
	GameEngine* _this = (GameEngine*)ptr;
	if (_this->b1->text == "Change") {
		Button b = *_this->b1; b.text = "Changed";
		_this->screen.changeButton(_this->b1->id, b);
		_this->cube = _this->getMesh("poly_cube.obj");
	}
	else {
		Button b = *_this->b1; b.text = "Change";
		_this->screen.changeButton(_this->b1->id, b);
		_this->cube = _this->getMesh("poly_suzanne.obj");
	}
	return 0;
}

int main(int argc, char* args[])
{
	GameEngine game;

	game._this = &game;

	game.CreateNewScreen("Hello World", DEFAULT_WIDTH, DEFAULT_HEIGHT);
	game.screen.setIcon("icon.bmp");

	game.screen.HideCursor();
	game.screen.setMousePos(game.screen.SCREEN_WIDTH / 2, game.screen.SCREEN_HEIGHT / 2);

	game.start();

	return 0;
}