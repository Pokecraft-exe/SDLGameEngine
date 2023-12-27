#pragma once
#include <chrono>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <fstream>
#include <functional>
#include <iostream> //
#include <list>
#include <SDL.h>
#include <stdio.h>
#include <strstream>
#include <thread>
#include <vcruntime_exception.h>
#include "maths_engine.h"
#include "window.h"
using namespace std;
#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 400

enum class gpu {
	None = 0,
	Cuda,
	Intel
};

void calculatePolygonsIntel(__parameters, void*);
void calculatePolygonsCuda(__parameters, void*);

class SDLGameEngine {
private:
	float _fps;
	void (*_calculatePolygons)(__parameters, void*);
	void GameThread();
public:
	gpu currentGpu;
	mat4x4 _matrixMakeIdentity();
	void _meshApplyRotations(__parameters r_mesh);
	void _cameraApplyRotations(__parameters r_camera);
	void _meshApplyTransations(__parameters t_mesh);
	void _drawPolygons(camera* _camera, light* _light, vector<vec3d>* _verts, vector<polygon>* _polys);
public:
	std::atomic<bool> quit = false;
	window screen;
	vector<mesh> meshes;
	vector<light> lights;
	float fElapsedTime = 0.0;
	SDLGameEngine();
	void start();
	void CreateNewScreen(string Name, int w, int h);
	virtual bool OnUserCreates() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	float fps();
	mesh getMesh(string sFilename);
	void render(mesh* _mesh, camera* _camera, light* _light);
	void moveMeshX(mesh* t_mesh, float unit);
	void moveMeshY(mesh* t_mesh, float unit);
	void moveMeshZ(mesh* t_mesh, float unit);
	void rotateMeshX(mesh* r_mesh, float degree);
	void rotateMeshY(mesh* r_mesh, float degree);
	void rotateMeshZ(mesh* r_mesh, float degree);
	void throwException(string file, string function, string str, bool ishost);
};