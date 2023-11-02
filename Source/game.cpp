#include <iostream>
#include "game.h"
#include "window.h"
using namespace std;

/*
to do:
clipping
GPU threads,
draw when render,
textures,
materials,
raytracing,
clean and optimize,
animations
*/

int change(void* ptr);

class GameEngine : public SDLGameEngine {
public:
	void* _this;
	mesh axis;
	mesh cube;
	mesh cube1;
	mesh cube2;
	mesh suzanne;
	float fTheta;
	camera cam;
	light point;
	Button* b1;
	bool alt;
	Image i;

	bool OnUserCreates() {

		//cube = getMesh("Oaxis.obj");
		vec3d vert[4] = { vec3d(0, 1, 0), vec3d(1, 1, 0) , vec3d(0, 0, 0) , vec3d(1, 0, 0) };
		polygon polys[2] = { polygon({ 0, 3, 2 }), polygon({ 0, 1, 3}) };
		cube = mesh( vert, 4, polys, 2 );
		suzanne = getMesh("poly_suzanne.obj");
		cube1 = getMesh("poly_cube.obj");
		cube2 = getMesh("poly_cube.obj");

		cam = camera(screen.SCREEN_HEIGHT, screen.SCREEN_WIDTH);
		point.orientation = { 0.0f, 0.0f, -1.0f };

		cam._pointAt = { 0.0f, 0.0f, 1.0f };

		moveMeshZ(&suzanne, 12.0f);
		moveMeshX(&suzanne, 4.0f);
		moveMeshY(&suzanne, -4.0f);
		moveMeshZ(&cube2, 104.0f);
			
		for (polygon& p : cube.polygons) {
			p.m.Ka = 0xffffff;
		}
		cube.origin.z += 4;
		b1 = screen.addButton("Change", 30, 100, 60, 20, 0xff0066, change, _this);

		i = Image("lettuce.png", 0, 0);
		//rotateMeshZ(&suzanne, 180);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {

		//screen.clear();
		screen.ClearDepthBuffer();

		screen.fill(0x00FFFF);

		fTheta += 2.0f * fElapsedTime;

		while (screen.PollEvent()) {
			if (screen.e.type == QUIT) {
				quit = true; 
			}
			else if (screen.e.type == KEYDOWN) {
				if (screen.e.key.keysym.scancode == 41) { quit = true; }
				else if (screen.e.key.keysym.scancode == 26) {
					vec3d vForward = Vector_Mul(cam._pointAt, 8.0f * fElapsedTime);
					cam.position = Vector_Add(cam.position, vForward);
				}
				else if (screen.e.key.keysym.scancode == 22) {
					vec3d vForward = Vector_Mul(cam._pointAt, 8.0f * fElapsedTime);
					cam.position = Vector_Sub(cam.position, vForward);
				}
				else if (screen.e.key.keysym.scancode == 4) {
					cam.RotationY += 0.5f * fElapsedTime;
				}
				else if (screen.e.key.keysym.scancode == 7) {
					cam.RotationY -= 0.5f * fElapsedTime;
				}
				else if (screen.e.key.keysym.scancode == 82) {
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
				//cam.RotationX -= screen.e.motion.xrel * 0.5f * fElapsedTime;
				//cam.RotationY += screen.e.motion.yrel * 0.5f * fElapsedTime;
				screen.setMousePos(screen.SCREEN_WIDTH / 2, screen.SCREEN_HEIGHT / 2);
				//cout << "mouse moved in y : " << screen.e.motion.yrel << endl;
			}
		}

		rotateMeshY(&suzanne, fTheta * 10);

		render(&cube, &cam, &point);
		//render(&cube1, &cam, &point);
		//render(&cube2, &cam, &point);
		render(&suzanne, &cam, &point);

		//screen.RenderImage(i);

		//for (float w : screen.depthBuffer) if (w != 0) cout << w << endl;

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