#pragma once
#include <thread>
#include <SDL.h>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include <iostream> //
#include <strstream>
#include <algorithm>
#include <atomic>
#include "window.h"
#include "maths_engine.h"
#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 400

class SDLGameEngine {
private:
	void GameThread() {
		if (!OnUserCreates()) quit = true;

		auto time1 = std::chrono::system_clock::now();
		auto time0 = std::chrono::system_clock::now();
		while (!quit) {
			do {
				if (quit) return;

				time1 = std::chrono::system_clock::now();
				std::chrono::duration<float> _elapsedTime = time1 - time0;
				time0 = time1;
				fElapsedTime = _elapsedTime.count();
			} while (OnUserUpdate(fElapsedTime) && !quit);
		}

		screen.quit();
		return;
	}
	mat4x4 _matrixMakeIdentity()
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
	void _meshApplyRotations(__parameters r_mesh) {
		r_mesh.Rx->m[0][0] = 1;
		r_mesh.Rx->m[1][1] = cosf(r_mesh._mesh->RotationX);
		r_mesh.Rx->m[1][2] = sinf(r_mesh._mesh->RotationX);
		r_mesh.Rx->m[2][1] = -sinf(r_mesh._mesh->RotationX);
		r_mesh.Rx->m[2][2] = cosf(r_mesh._mesh->RotationX);
		r_mesh.Rx->m[3][3] = 1;

		r_mesh.Ry->m[0][0] = cosf(r_mesh._mesh->RotationY);
		r_mesh.Ry->m[0][2] = sinf(r_mesh._mesh->RotationY);
		r_mesh.Ry->m[2][0] = -sinf(r_mesh._mesh->RotationY);
		r_mesh.Ry->m[1][1] = 1.0f;
		r_mesh.Ry->m[2][2] = cosf(r_mesh._mesh->RotationY);
		r_mesh.Ry->m[3][3] = 1.0f;

		r_mesh.Rz->m[0][0] = cosf(r_mesh._mesh->RotationZ);
		r_mesh.Rz->m[0][1] = sinf(r_mesh._mesh->RotationZ);
		r_mesh.Rz->m[1][0] = -sinf(r_mesh._mesh->RotationZ);
		r_mesh.Rz->m[1][1] = cosf(r_mesh._mesh->RotationZ);
		r_mesh.Rz->m[2][2] = 1;
		r_mesh.Rz->m[3][3] = 1;
	}
	void _cameraApplyRotations(__parameters r_camera) {
		if (!(r_camera.Rx == nullptr)) {
			r_camera.Rx->m[0][0] = 1;
			r_camera.Rx->m[1][1] = cosf(r_camera._camera->RotationX);
			r_camera.Rx->m[1][2] = sinf(r_camera._camera->RotationX);
			r_camera.Rx->m[2][1] = -sinf(r_camera._camera->RotationX);
			r_camera.Rx->m[2][2] = cosf(r_camera._camera->RotationX);
			r_camera.Rx->m[3][3] = 1;
		}
		if (!(r_camera.Ry == nullptr)) {
			r_camera.Ry->m[0][0] = cosf(r_camera._camera->RotationY);
			r_camera.Ry->m[0][2] = sinf(r_camera._camera->RotationY);
			r_camera.Ry->m[2][0] = -sinf(r_camera._camera->RotationY);
			r_camera.Ry->m[1][1] = 1.0f;
			r_camera.Ry->m[2][2] = cosf(r_camera._camera->RotationY);
			r_camera.Ry->m[3][3] = 1.0f;
		}
		if (!(r_camera.Rz == nullptr)) {
			r_camera.Rz->m[0][0] = cosf(r_camera._camera->RotationZ);
			r_camera.Rz->m[0][1] = sinf(r_camera._camera->RotationZ);
			r_camera.Rz->m[1][0] = -sinf(r_camera._camera->RotationZ);
			r_camera.Rz->m[1][1] = cosf(r_camera._camera->RotationZ);
			r_camera.Rz->m[2][2] = 1;
			r_camera.Rz->m[3][3] = 1;
		}
	}
	void _meshApplyTransations(__parameters t_mesh) {
		t_mesh.t->m[0][0] = 1.0f;
		t_mesh.t->m[1][1] = 1.0f;
		t_mesh.t->m[2][2] = 1.0f;
		t_mesh.t->m[3][3] = 1.0f;
		t_mesh.t->m[3][0] = t_mesh._mesh->origin.x;
		t_mesh.t->m[3][1] = t_mesh._mesh->origin.y;
		t_mesh.t->m[3][2] = t_mesh._mesh->origin.z;
	}
	void _calculateTriangles(vector<triangle>* triangles, __parameters _param) {

		mesh* _mesh = _param._mesh;
		camera* _camera = _param._camera;
		light* _light = _param._light;

		mat4x4 World;
		mat4x4 Rx;
		mat4x4 Ry;
		mat4x4 Rz;
		mat4x4 t;

		_param.Rx = &Rx;
		_param.Ry = &Ry;
		_param.Rz = &Rz;
		_param.t = &t;
		_param.World = &World;

		_meshApplyRotations(_param);
		_meshApplyTransations(_param);

		World = _matrixMakeIdentity();
		World = Matrix_MultiplyMatrix(Rz, Rx);
		World = Matrix_MultiplyMatrix(World, Ry);
		World = Matrix_MultiplyMatrix(World, t);

		mat4x4 _cameraRotation;

		//_param.Rx = nullptr;//&_cameraRotation;
		//_param.Ry = &_cameraRotation;
		//_param.Rz = nullptr;//&_cameraRotation;

		//_cameraApplyRotations(_param);

		//_camera->_pointAt = MultiplyMatrixVector(_cameraRotation, _camera->_right);
		//_camera->_right = _camera->position + _camera->_pointAt;

		_camera->_right = Vector_Add(_camera->position, _camera->_pointAt);

		mat4x4 __matrix = Matrix_PointAt(_camera->position, _camera->_right, _camera->_up);
		_camera->_matrix = Matrix_QuickInverse(__matrix);

		for (triangle& tri : _mesh->tris)
		{
			triangle triProjected, triTransformed, triViewed;

			triTransformed.p[0] = MultiplyMatrixVector(World, tri.p[0]);
			triTransformed.p[1] = MultiplyMatrixVector(World, tri.p[1]);
			triTransformed.p[2] = MultiplyMatrixVector(World, tri.p[2]);

			vec3d normal, line1, line2;
			line1 = Vector_Sub(triTransformed.p[1] , triTransformed.p[0]);
			line2 = Vector_Sub(triTransformed.p[2] , triTransformed.p[0]);

			normal = Vector_CrossProduct(line1 , line2);

			normal.normalise();

			vec3d vCameraRay;
			vCameraRay = Vector_Sub(triTransformed.p[0] , _camera->position);

			if (Vector_DotProduct(normal, vCameraRay) < 0.0f) {

				float l = sqrtf(_light->orientation.x * _light->orientation.x +
					_light->orientation.y * _light->orientation.y +
					_light->orientation.z * _light->orientation.z);
				_light->orientation.x /= l; _light->orientation.y /= l; _light->orientation.z /= l;

				triProjected.dp = normal.x * _light->orientation.x + normal.y * _light->orientation.y + normal.z * _light->orientation.z;

				triViewed.p[0] = MultiplyMatrixVector(_camera->_matrix, triTransformed.p[0]);
				triViewed.p[1] = MultiplyMatrixVector(_camera->_matrix, triTransformed.p[1]);
				triViewed.p[2] = MultiplyMatrixVector(_camera->_matrix, triTransformed.p[2]);

				triProjected.p[0] = MultiplyMatrixVector(_camera->projection, triViewed.p[0]);
				triProjected.p[1] = MultiplyMatrixVector(_camera->projection, triViewed.p[1]);
				triProjected.p[2] = MultiplyMatrixVector(_camera->projection, triViewed.p[2]);

				triProjected.p[0] = Vector_Div(triProjected.p[0] , triProjected.p[0].w);
				triProjected.p[1] = Vector_Div(triProjected.p[1] , triProjected.p[1].w);
				triProjected.p[2] = Vector_Div(triProjected.p[2] , triProjected.p[2].w);


				vec3d vOffsetView = { 1,1,0 };
				triProjected.p[0] = triProjected.p[0], vOffsetView;
				triProjected.p[1] = triProjected.p[1], vOffsetView;
				triProjected.p[2] = triProjected.p[2], vOffsetView;
				triProjected.p[0].x *= 0.5f * (float)screen.SCREEN_WIDTH;
				triProjected.p[0].y *= 0.5f * (float)screen.SCREEN_HEIGHT;
				triProjected.p[1].x *= 0.5f * (float)screen.SCREEN_WIDTH;
				triProjected.p[1].y *= 0.5f * (float)screen.SCREEN_HEIGHT;
				triProjected.p[2].x *= 0.5f * (float)screen.SCREEN_WIDTH;
				triProjected.p[2].y *= 0.5f * (float)screen.SCREEN_HEIGHT;
				triangles->push_back(triProjected);
			}
		}

		sort(triangles->begin(), triangles->end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2;
			});
	}
	void _drawTriangles(vector<triangle>* triangles) {
		for (auto& triProjected : *triangles) {
			screen.fillAATriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				_getColorByLum(0, triProjected.dp));
		}
	}
public:
	std::atomic<bool> quit = false;
	window screen;
	vector<light> lights;
	float fElapsedTime = 0.0;

	void start() {
		GameThread();
	}
	
	void CreateNewScreen(string Name, int w, int h) {
		screen.changeName(Name);
		screen.changeSize(w, h);
	}

	virtual bool OnUserCreates() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;

	mesh getMesh(string sFilename) {
		ifstream f(sFilename);
		bool file_read = false;
		mesh _mesh;
		if (!f.is_open()) return _mesh;

		// Local cache of verts
		vector<vec3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int fi[3];
				s >> junk >> fi[0] >> fi[1] >> fi[2];
				file_read = true;
				_mesh.tris.push_back({ verts[fi[0] - 1], verts[fi[1] - 1], verts[fi[2] - 1] });
			}
		}
		f.close();
		if (file_read) std::cout << "file \"" << sFilename << "\" read successfully!" << std::endl;
		return _mesh;
	}

	void renderMesh(mesh* _mesh, camera* _camera, light* _light) {

		vector<triangle> triangles;

		__parameters param = { &triangles, _mesh, _camera, _light };

		// delegates the triangle calculation to another thread
		std::thread t = std::thread(&SDLGameEngine::_calculateTriangles, this, &triangles, param);
		t.join();

		// delegates the triangle drawing to another thread 
		std::thread t2 = std::thread(&SDLGameEngine::_drawTriangles, this, &triangles);
		t2.join();

	}

	void moveMeshX(mesh* t_mesh, float unit) {
		t_mesh->origin.x += unit;
	}
	void moveMeshY(mesh* t_mesh, float unit) {
		t_mesh->origin.y += unit;
	}
	void moveMeshZ(mesh* t_mesh, float unit) {
		t_mesh->origin.z += unit;
	}

	void rotateMeshX(mesh *r_mesh, float degree) {
		r_mesh->RotationX = degree * 3.14159f / 180;
	}
	void rotateMeshY(mesh* r_mesh, float degree) {
		r_mesh->RotationY = degree * 3.14159f / 180;
	}
	void rotateMeshZ(mesh* r_mesh, float degree) {
		r_mesh->RotationZ = degree * 3.14159f / 180;
	}
};