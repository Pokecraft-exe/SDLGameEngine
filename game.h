#pragma once
#include <chrono>
#include <fstream>
#include <iostream> //
#include <SDL.h>
#include <stdio.h>
#include <strstream>
#include <thread>
#include "maths_engine.h"
#include "window.h"
#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 400

class SDLGameEngine {
private:
	float _fps;
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
				_fps = 1 / fElapsedTime;
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
			r_mesh.Rx->m[0][0] = 1.0f;
			r_mesh.Rx->m[1][1] = cosf(r_mesh._mesh->RotationX);
			r_mesh.Rx->m[1][2] = sinf(r_mesh._mesh->RotationX);
			r_mesh.Rx->m[2][1] = -sinf(r_mesh._mesh->RotationX);
			r_mesh.Rx->m[2][2] = cosf(r_mesh._mesh->RotationX);
			r_mesh.Rx->m[3][3] = 1.0f;
			
			r_mesh.Ry->m[0][0] = cosf(r_mesh._mesh->RotationY);
			r_mesh.Ry->m[0][2] = sinf(r_mesh._mesh->RotationY);
			r_mesh.Ry->m[2][0] = -sinf(r_mesh._mesh->RotationY);
			r_mesh.Ry->m[1][1] = 1.0f;
			r_mesh.Ry->m[2][2] = cosf(r_mesh._mesh->RotationY);
			r_mesh.Ry->m[3][3] = 1.0f;
			
			r_mesh.Rz->m[0][0] = cosf(3.14 + r_mesh._mesh->RotationZ);
			r_mesh.Rz->m[0][1] = sinf(3.14 + r_mesh._mesh->RotationZ);
			r_mesh.Rz->m[1][0] = -sinf(3.14 + r_mesh._mesh->RotationZ);
			r_mesh.Rz->m[1][1] = cosf(3.14 + r_mesh._mesh->RotationZ);
			r_mesh.Rz->m[2][2] = 1.0f;
			r_mesh.Rz->m[3][3] = 1.0f;
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
	void _calculatePolygons(__parameters _param) {
	
		vector<polygon>* polygons = _param.polygones;
		mesh* _mesh = _param._mesh;
		camera* _camera = _param._camera;
		light* _light = _param._light;

		mat4x4 World, Rx, Ry, Rz, t;

		_param.Rx = &Rx;
		_param.Ry = &Ry;
		_param.Rz = &Rz;
		_param.t = &t;
		_param.World = &World;

		_meshApplyRotations(_param);
		_meshApplyTransations(_param);

		World = _matrixMakeIdentity();
		World = Matrix_MultiplyMatrix(Ry, Rz);
		World = Matrix_MultiplyMatrix(World, Rx);
		World = Matrix_MultiplyMatrix(World, t);

		/*mat4x4 _cameraRotation;

		_param.Rx = nullptr;//&_cameraRotation;
		_param.Ry = &_cameraRotation;
		_param.Rz = nullptr;//&_cameraRotation;
		
		_cameraApplyRotations(_param);

		_camera->_up = { 0,1,0 };
		_camera->_pointAt = { 0,0,1 };//MultiplyMatrixVector(_cameraRotation, _camera->_target);
		//_camera->_target = _camera->position + _camera->_pointAt;

		_camera->_target = Vector_Add(_camera->position, _camera->_pointAt);

		mat4x4 __matrix = Matrix_PointAt(_camera->position, _camera->_target, _camera->_up);
		_camera->_matrix = Matrix_QuickInverse(__matrix);*/

		for (polygon& poly : _mesh->polygon)
		{
			polygon Projected, Transformed, Viewed;

			size_t size = poly.p.size();

			for (int i = 0; i < size; i++) {
				Transformed.p.push_back(MultiplyMatrixVector(World, poly.p[i]));
			}

			vec3d normal = calculate_normal(Transformed);

			vec3d vCameraRay;
			vCameraRay = Vector_Sub(Transformed.p[0], _camera->position);

			if (Vector_DotProduct(normal, vCameraRay) < 0.0f) {

				float l = sqrtf(_light->orientation.x * _light->orientation.x +
					_light->orientation.y * _light->orientation.y +
					_light->orientation.z * _light->orientation.z);
				_light->orientation.x /= l; _light->orientation.y /= l; _light->orientation.z /= l;
				

				Projected.dp = normal.x * _light->orientation.x + normal.y * _light->orientation.y + normal.z * _light->orientation.z;

				for (int i = 0; i < size; i++) {
					Viewed.p.push_back(MultiplyMatrixVector(_camera->_matrix, Transformed.p[i]));
				}

				for (int i = 0; i < size; i++) {
					Projected.p.push_back(MultiplyMatrixVector(_camera->projection, Transformed.p[i]));
				}

				for (int i = 0; i < size; i++) {
					Projected.p[i].x += 1.0f;
					Projected.p[i].y += 1.0f;
					Projected.p[i].x *= 0.5f * (float)screen.SCREEN_WIDTH;
					Projected.p[i].y *= 0.5f * (float)screen.SCREEN_HEIGHT;
				}

				polygons->push_back(Projected);
			}
		}

		sort(polygons->begin(), polygons->end(), [](polygon& t1, polygon& t2)
			{
				size_t size1 = t1.p.size() - 1;
				size_t size2 = t2.p.size() - 1;
				float z1 = 0.0f;
				float z2 = 0.0f;
				for (int i = 0; i < size1; i++) {
					z1 += t1.p[i].z;
				}
				z1 /= size1 + 1;
				for (int i = 0; i < size2; i++) {
					z2 += t2.p[i].z;
				}
				z2 /= size2 + 1;
				return z1 > z2;
			});
	}
	void _drawPolygons(vector<polygon>* polygones) {
		for (auto& Projected : *polygones) {
			screen.fillPolygon(Projected.p, _getColorByLum(Projected.dp));
			screen.drawPolygon(Projected.p, 0xff6666);
		}
	}
public:
	std::atomic<bool> quit = false;
	window screen;
	vector<mesh> meshes;
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

	float fps() { return _fps; }

	mesh getMesh(string sFilename) {
		ifstream f(sFilename);
		bool file_read = false;
		mesh _mesh;
		if (!f.is_open()) return _mesh;

		// Local cache of verts
		vector<vec3d> verts;
		vector<vec3d> normals;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;
			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 'n') {
					vec3d n;
					s >> junk >> n.x >> n.y >> n.z;
					n.index = normals.size();
					normals.push_back(n);
				}
				else {
					vec3d v;
					s >> junk >> v.x >> v.y >> v.z;
					v.index = verts.size();
					verts.push_back(v);
				}
			}

			if (line[0] == 'f')
			{
				polygon _polygon;
				vector<int> f;
				s >> junk;
				vector<string> fi;
				string fs = "";
				while (!s.eof()) {
					fs = "";
					s >> fs;
					fi.push_back(fs);
				}
				for (string& _s : fi) {
					if (_s.find("/") == -1) {
						f.push_back(stoi(_s));
					}
					else {
						f.push_back(stoi(_s.substr(0, _s.find('/'))));
						if (normals.size() > 0) {
							_polygon.normal = normals[stoi(_s.substr(_s.rfind('/') + 1))-1];
							_polygon.has_normal = true;
						}
					}
				}
				
				for (int i : f) {
					_polygon.p.push_back(verts[i-1]);
				}
				_mesh.polygon.push_back(_polygon);
				
				file_read = true;
			}
		}
		f.close();
		if (file_read) std::cout << "file \"" << sFilename << "\" read successfully, imported "
			<< verts.size() << " vertices, " << normals.size() << " normals and "
			<< _mesh.polygon.size() << " polygones!" << std::endl;
		return _mesh;
	}

	void render(mesh* _mesh, camera* _camera, light* _light) {
		vector<polygon> polygones;

		__parameters param = { &polygones, _mesh, _camera, _light }; param.end = false;

		// delegates the triangle calculation to another thread while triangulating
		std::thread t = std::thread(&SDLGameEngine::_calculatePolygons, this, param);
		t.join();

		// delegates the triangle drawing to another thread
		std::thread t2 = std::thread(&SDLGameEngine::_drawPolygons, this, &polygones);
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