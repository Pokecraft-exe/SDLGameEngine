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
	mesh suz;
	mesh tea;
	mesh* cube;
	float fTheta;
	camera cam;
	light point;
	Button b1 = Button("Click Me", 30, 100, 70, 20, 0xff0066, change, this);
	Scale scaleX = Scale(30, 200, 200, 10, 360);
	Scale scaleY = Scale(30, 220, 200, 10, 360);
	Scale scaleZ = Scale(30, 240, 200, 10, 360);
	Scale scaleOZ = Scale(30, 260, 200, 10, 100);
	EndWidget endW;
	bool alt;
	Image i = Image("lettuce.png", 0, 0);

	bool OnUserCreates() {

		//cube = getMesh("Oaxis.obj");
		//vec3d vert[4] = { vec3d(0, 1, 0), vec3d(1, 1, 0) , vec3d(0, 0, 0) , vec3d(1, 0, 0) };
		//polygon polys[2] = { polygon({ 0, 3, 2 }), polygon({ 0, 1, 3}) };
		suz = getMesh("C:\\Users\\Julian\\source\\repos\\gameIntel\\3DModels\\poly_suzanne.obj");//mesh( vert, 4, polys, 2 );
		tea = getMesh("C:\\Users\\Julian\\source\\repos\\gameIntel\\3DModels\\poly_cube.obj");
		cube = &suz;
		//suzanne = getMesh("poly_suzanne.obj");
		//cube1 = getMesh("poly_cube.obj");
		//cube2 = getMesh("poly_cube.obj");

		cam = camera(screen.sizex(), screen.sizey());
		point.orientation = { 0.0f, 0.0f, -1.0f };

		cam._pointAt = { 0.0f, 0.0f, 1.0f };
			
		for (polygon& p : suz.polygons) {
			p.m.Ka = 0xffffff;
		}

		for (polygon& p : tea.polygons) {
			p.m.Ka = 0xffffff;
		}

		scaleOZ.value = 40;

		b1.attach(&screen);
		scaleX.attach(&screen);
		scaleY.attach(&screen);
		scaleZ.attach(&screen);
		scaleOZ.attach(&screen);
		endW.attach(&screen);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {

		screen.fill(0x00FFFF);

		fTheta += 2.0f * fElapsedTime;

		while (screen.PollEvent()) {
			if (screen.e.type == QUIT) {
				quit = true; 
			}
			else if (screen.e.type == KEYDOWN) {
				if (screen.e.key.keysym.scancode == 41) { quit = true; }
				/*else if (screen.e.key.keysym.scancode == 26) {
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
				}*/
				else {
					cout << "Key \"" << screen.e.key.keysym.scancode << "\" pressed !" << endl;
				}
			}
			else if (screen.e.window.event == SDL_WINDOWEVENT_RESIZED) {
				cout << "resizing\n";
				cam = camera(screen.sizex(), screen.sizey());
			}
		}

		rotateMeshX(cube, scaleX.value);
		rotateMeshY(cube, scaleY.value);
		rotateMeshZ(cube, scaleZ.value);
		cube->origin.z = scaleOZ.value / 10;

		render(cube, &cam, &point);

		screen.DrawString("Rx", 10, 200, 0xff6666, 1);
		screen.DrawString("Ry", 10, 220, 0xff6666, 1);
		screen.DrawString("Rz", 10, 240, 0xff6666, 1);
		screen.DrawString("Z", 10, 260, 0xff6666, 1);

		screen.DrawString("FPS: " + to_string((int)fps()), 10, 5, 0xff6666, 2);

		screen.DrawWidgets();

		screen.update();
		return true;
	}
};

int change(void* ptr) {
	GameEngine* _this = (GameEngine*)ptr;
	if (_this->b1.color == 0xff0066) {
		_this->b1.color = 0x6600ff;
		_this->cube = &_this->tea;
	}
	else {
		_this->b1.color = 0xff0066;
		_this->cube = &_this->suz;
	}
	return 0;
}

void _main()
{
	GameEngine game;
	game.CreateNewScreen("Hello World", DEFAULT_WIDTH, DEFAULT_HEIGHT);
	game.screen.setIcon("icon.bmp");
	game.screen.resizable();

	game.start();
}