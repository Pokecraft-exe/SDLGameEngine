#pragma once
#include <vector>
#include <math.h>
#include <stdio.h>
#include "window.h"
using namespace std;

class vec3d {
public:
	float x, y, z, w;
	vec3d();
	vec3d(float _x, float _y, float _z, float _w);
	vec3d(float _x, float _y, float _z);
	vec3d(const vec3d& v);
	vec3d operator+(vec3d v);
	vec3d operator-(vec3d v);
	vec3d operator/(float f);
	vec3d operator*(float f);
	vec3d operator*(vec3d v);
	void normalise();
	float length();
};

struct triangle {
	vec3d p[3];
	float dp;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

struct mesh {
	vector<triangle> tris;
	vec3d origin;
	mat4x4 World;
	float RotationX, RotationY, RotationZ = 0.0f;
};

struct light {
	vec3d orientation;
	int x, y;
	float power;
};

class camera {
public:
	vec3d _forward = { 1, 0, 0};
	vec3d _up = { 0, 1, 0};
	vec3d _right;
	vec3d _pointAt = { 0, 0, 1 };
	mat4x4 _matrix;
	//
	mat4x4 projection;
	vec3d position;
	float RotationX, RotationY, RotationZ = 0.0f;
	float Near, Far, Fov, AspectRatio, FovRad = 0.0f;
	camera(int h, int w);
	camera();
};

struct __parameters {
	vector<triangle>* triangles;
	mesh* _mesh;
	camera* _camera;
	light* _light;
	mat4x4* World;
	mat4x4* Rx;
	mat4x4* Ry;
	mat4x4* Rz;
	mat4x4* t;
};

struct material {
	uint32_t color = 0x666666;
	float roughness = 0.0f;
};

float radToDeg(float radian);
vec3d Vector_Add(vec3d& v1, vec3d& v2);
vec3d Vector_Sub(vec3d& v1, vec3d& v2);
vec3d Vector_Div(vec3d& v1, float k);
float Vector_DotProduct(vec3d& v1, vec3d& v2);
vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);
vec3d MultiplyMatrixVector(mat4x4& m, vec3d& i);
mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);
mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
mat4x4 Matrix_QuickInverse(mat4x4& m);
uint32_t _getColorByLum(uint32_t base_color, float lum);
float Vector_DotProduct(vec3d& v1, vec3d& v2);
float absolute(float x);
int iPartOfNumber(float x);
int roundNumber(float x);
float fPartOfNumber(float x);
float rfPartOfNumber(float x);
