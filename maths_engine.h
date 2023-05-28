#pragma once
#include <algorithm>
#include <iostream> //
#include <math.h>
#include <vector>
#include <stdio.h>
#include "Color.h"
using namespace std;

struct vec3d {
public:
	float x, y, z, w;
	size_t index;
	vec3d();
	vec3d(float _x, float _y, float _z, float _w);
	vec3d(float _x, float _y, float _z);
	vec3d(const vec3d& v);
	vec3d operator+(const vec3d v);
	vec3d operator-(const vec3d v);
	vec3d operator/(float f);
	vec3d operator*(float f);
	vec3d operator*(vec3d v);
	bool operator==(vec3d v);
	void normalise();
	float length();
};

struct triangle {
	vec3d p[3];
	float dp;
	triangle(vec3d a, vec3d b, vec3d c) { p[0] = a; p[1] = b; p[2] = c; }
	triangle() { }
};

struct polygon {
	vector<vec3d> p;
	vec3d normal;
	float dp;
	bool has_normal;
	polygon(vector<vec3d> v) { p = v; }
	polygon() { }
	vec3d& operator[](size_t i) {
		return p[i];
	}

	const vec3d& operator[](size_t i) const {
		return p[i];
	}
};


struct mat4x4 {
	float m[4][4] = { 0 };
};

struct mesh {
	vector<polygon> polygon;
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
	//vec3d _forward
	vec3d _up;
	vec3d _target;
	vec3d _pointAt;
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
	vector<polygon>* polygones;
	mesh* _mesh;
	camera* _camera;
	light* _light;
	mat4x4* World;
	mat4x4* Rx;
	mat4x4* Ry;
	mat4x4* Rz;
	mat4x4* t;
	bool end = 0;
};

struct material {
	uint32_t color = 0x666666;
	float roughness = 0.0f;
};

float radToDeg(float radian);
vec3d Vector_Add(vec3d& v1, vec3d& v2);
vec3d Vector_Sub(vec3d& v1, vec3d& v2);
vec3d Vector_Div(vec3d& v1, float k);
vec3d Vector_Mul(vec3d& v1, float k);
float Vector_DotProduct(vec3d& v1, vec3d& v2);
vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);
vec3d MultiplyMatrixVector(mat4x4& m, vec3d& i);
mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);
mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
mat4x4 Matrix_QuickInverse(mat4x4& m);
uint32_t _getColorByLum(float lum);
float Vector_DotProduct(vec3d& v1, vec3d& v2);
float absolute(float x);
int iPartOfNumber(float x);
int roundNumber(float x);
float fPartOfNumber(float x);
float rfPartOfNumber(float x);
float sum(vector<vec3d> f);
size_t search(vector<vec3d>& verts, vec3d v);
vec3d calculate_normal(polygon& poly);