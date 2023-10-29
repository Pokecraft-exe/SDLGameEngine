#pragma once
#include <algorithm>
#include <iostream> //
#include <math.h>
#include <vector>
#include <stdio.h>
#include "Color.h"
using namespace std;

/*
https://www.fileformat.info/format/material/
illum = 0 -> 10
0 Color on and Ambient off
1 Color on and Ambient on
2 Highlight on
3 Reflection on and Ray trace on
4 Transparency: Glass on
Reflection: Ray trace on
5 Reflection: Fresnel on and Ray trace on
6 Transparency: Refraction on
Reflection: Fresnel off and Ray trace on
7 Transparency: Refraction on
Reflection: Fresnel on and Ray trace on
8 Reflection on and Ray trace off
9 Transparency: Glass on
Reflection: Ray trace off
10 Casts shadows onto invisible surfaces
*/

struct material {
	uint32_t Ka = 0x000000; // ambiant color
	uint32_t Kd = 0x000000; // diffuse color
	uint32_t Ks = 0x000000; // specular color
	uint32_t Ke = 0x000000; // emissive color
	float Ni = 0.0f; // optic density
	float Tf = 0.0f; // Transmission Filter
	uint8_t illum = 0.0f;
	float d = 0.0f; // transparence
	uint16_t sharpness = 0.0f; // 0 -> 1000
	string texture_map = "";
	string specular_map = "";
	string reflection_map = "";
};


struct vec3d {
public:
	float x, y, z, w;
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

struct polygon {
	vector<size_t> p;
	material m;
	polygon(vector<size_t> v) { p = v; }
	polygon() { }
	size_t operator[](size_t i) {
		return p[i];
	}

	const size_t operator[](size_t i) const {
		return p[i];
	}
};


struct mat4x4 {
	float m[4][4] = { 0 };
};

struct mesh {
	mesh();
	mesh(vec3d* verts, size_t vsize, polygon* polys, size_t psize);
	mesh(vector<vec3d> verts, vector<polygon> polys);
	vector<vec3d> vertices;
	vector<polygon> polygons;
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
	vector<vec3d>* verts;
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
vec3d calculate_normal(vector<vec3d>& poly);
vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd);
uint8_t polygon_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, polygon* in_poly, polygon* out_poly, polygon* out_poly2, vector<vec3d>* vertices);
polygon marchingCubes(vector<vec3d> cubes, int ptr);