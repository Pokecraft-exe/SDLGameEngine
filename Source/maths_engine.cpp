#include "maths_engine.h"

float radToDeg(float radian) { return radian * 180 / 3.14159f; }
camera::camera() { Near = 0.0f; }
camera::camera(int h, int w) {
	Near = 0.1f;
	Far = 1000.0f;
	Fov = 90.0f;
	AspectRatio = (float)h / (float)w;
	FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);

	_target = { 0, 0, 1 };

	projection.m[0][0] = AspectRatio * FovRad;
	projection.m[1][1] = FovRad;
	projection.m[2][2] = Far / (Far - Near);
	projection.m[3][2] = (-Far * Near) / (Far - Near);
	projection.m[2][3] = 1.0f;
	projection.m[3][3] = 0.0f;
}

uint32_t _getColorByLum(float lum) {
	uint8_t A = (uint8_t)(255.0f * lum);
	uint32_t result =
		((uint8_t)255 << 24) +
		((uint8_t)A << 16) + // r
		((uint8_t)A << 8) + // g
		(uint8_t)A; // b
	return result;
}

vec3d::vec3d() { w = 1; };
vec3d::vec3d(const vec3d& v)  {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
vec3d::vec3d(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}
vec3d::vec3d(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
	w = 1;
}
vec3d vec3d::operator+(const vec3d v) {
	return { x + v.x, y + v.y, z + z };
}
vec3d vec3d::operator-(const vec3d v) {
	return { x - v.x, y - v.y, z - v.z };
}
vec3d vec3d::operator/(float f) {
	return { x / f, y / f, z / f };
}
vec3d vec3d::operator*(float f) {
	return { x * f, y * f, z * f };
}
vec3d vec3d::operator*(vec3d v) {
	vec3d _v;
	_v.x = y * v.z - z * v.y;
	_v.y = z * v.x - x * v.z;
	_v.z = x * v.y - y * v.x;
	return _v;
}
bool vec3d::operator==(vec3d v) {
	if (x == v.x && y == v.y) return true;
	return false;
}

vec3d Vector_Add(vec3d& v1, vec3d& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
vec3d Vector_Sub(vec3d& v1, vec3d& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
vec3d Vector_Mul(vec3d& v1, float k)
{
	return { v1.x * k, v1.y * k, v1.z * k };
}
vec3d Vector_Div(vec3d& v1, float k)
{
	return { v1.x / k, v1.y / k, v1.z / k };
}
float Vector_DotProduct(vec3d& v1, vec3d& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2)
{
	vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
float Vector_Length(vec3d& v)
{
	return sqrtf(Vector_DotProduct(v, v));
}
vec3d Vector_Normalise(vec3d& v)
{
	float l = Vector_Length(v);
	return { v.x / l, v.y / l, v.z / l };
}

void vec3d::normalise()
{
	float l = length();
	x /= l; y /= l; z /= l;
}
float vec3d::length()
{
	return sqrtf(Vector_DotProduct(*this, *this));
}

float absolute(float x)
{
	if (x < 0) return -x;
	else return x;
}
int iPartOfNumber(float x)
{
	return (int)x;
}
int roundNumber(float x)
{
	return iPartOfNumber(x + 0.5);
}
float fPartOfNumber(float x)
{
	if (x > 0) return x - iPartOfNumber(x);
	else return x - (iPartOfNumber(x) + 1);

}
float rfPartOfNumber(float x)
{
	return 1 - fPartOfNumber(x);
}

vec3d MultiplyMatrixVector(mat4x4& m, vec3d& i) {
	vec3d v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}
mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2) {
	mat4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}
mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
{
	vec3d newForward = Vector_Sub(target, pos);
	newForward.normalise();

	// Calculate new Up direction
	vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
	vec3d newUp = Vector_Sub(up, a);
	newUp.normalise();

	// New Right direction is easy, its just cross product
	vec3d newRight = Vector_CrossProduct(newUp, newForward);

	mat4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}
mat4x4 Matrix_QuickInverse(mat4x4& m) // Only for Rotation/Translation Matrices
{
	mat4x4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

float sum(vector<vec3d> f) { float r = 0.0f; for (vec3d& i : f) { r += i.z; } return r; }
size_t search(vector<vec3d>& verts, vec3d v) { size_t i = 0; for (vec3d& b : verts) { if (b == v) return i; else i++; } return -1; }

float Cross(vec3d a, vec3d b) {
	return a.x * b.y - a.y * b.x;
}

bool IsPointInTriangle(vec3d p, vec3d a, vec3d b, vec3d c) {
	vec3d ab = b - a;
	vec3d bc = c - b;
	vec3d ca = a - c;

	vec3d ap = p - a;
	vec3d bp = p - b;
	vec3d cp = p - c;

	float cross1 = Cross(ab, ap);
	float cross2 = Cross(bc, bp);
	float cross3 = Cross(ca, cp);

	if (cross1 > 0.0f || cross2 > 0.0f || cross3 > 0.0f)
	{
		return false;
	}

	return true;
}

vec3d calculate_normal(vector<vec3d>& poly) {
	if (poly.size() < 3) { return { 0, 0, 0 }; }
	vec3d v1 = Vector_Sub(poly[1], poly[0]);
	vec3d v2 = Vector_Sub(poly[2], poly[0]);
	vec3d normal;
	normal = Vector_CrossProduct(v1, v2);
	normal.normalise();
	return normal;
}

vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
{
	plane_n = Vector_Normalise(plane_n);
	float plane_d = -Vector_DotProduct(plane_n, plane_p);
	float ad = Vector_DotProduct(lineStart, plane_n);
	float bd = Vector_DotProduct(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
	vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
	return Vector_Add(lineStart, lineToIntersect);
}

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, polygon* in_tri, polygon* out_tri1, polygon* out_tri2, vector<vec3d>* vertices)
{
	plane_n = Vector_Normalise(plane_n);
	auto dist = [&](vec3d& p)
	{
		vec3d n = Vector_Normalise(p);
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
	};

	size_t inside_points[3];  int nInsidePointCount = 0;
	size_t outside_points[3]; int nOutsidePointCount = 0;

	float d0 = dist(vertices->at(in_tri->p[0]));
	float d1 = dist(vertices->at(in_tri->p[1]));
	float d2 = dist(vertices->at(in_tri->p[2]));

	out_tri1->p.push_back(0);
	out_tri1->p.push_back(0);
	out_tri1->p.push_back(0);

	out_tri2->p.push_back(0);
	out_tri2->p.push_back(0);
	out_tri2->p.push_back(0);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[0]; }
	else { outside_points[nOutsidePointCount++] = in_tri->p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[1]; }
	else { outside_points[nOutsidePointCount++] = in_tri->p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = in_tri->p[2]; }
	else { outside_points[nOutsidePointCount++] = in_tri->p[2]; }

	if (nInsidePointCount == 3) {
		out_tri1->p[0] = in_tri->p[0];
		out_tri1->p[1] = in_tri->p[1];
		out_tri1->p[2] = in_tri->p[2];

		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		cout << "single" << endl;
		out_tri1->m.Ka = 0x00ff00;
		out_tri1->p[0] = inside_points[0];

		vertices->at(outside_points[0]) = Vector_IntersectPlane(plane_p, plane_n, vertices->at(inside_points[0]), vertices->at(outside_points[0]));
		out_tri1->p[1] = outside_points[0];

		vertices->at(outside_points[0]) = Vector_IntersectPlane(plane_p, plane_n, vertices->at(inside_points[0]), vertices->at(outside_points[0]));
		out_tri1->p[2] = outside_points[1];

		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		cout << "muliple" << endl;

		out_tri1->m.Ka = 0xff0000;
		out_tri1->p[0] = inside_points[0];
		out_tri1->p[1] = inside_points[1];
		out_tri1->p[2] = vertices->size();
		vertices->push_back(Vector_IntersectPlane(plane_p, plane_n, vertices->at(inside_points[0]), vertices->at(outside_points[0])));

		out_tri2->m.Ka = 0x0000ff;
		out_tri2->p[0] = inside_points[1];
		out_tri2->p[1] = out_tri1->p[2];
		out_tri2->p[2] = vertices->size();
		vertices->push_back(Vector_IntersectPlane(plane_p, plane_n, vertices->at(inside_points[1]), vertices->at(outside_points[0])));
		cout << inside_points[0] << " " << inside_points[1] << " " << outside_points[0] << endl;

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above

		return 2;
	}

	return 0;
}

uint8_t polygon_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, polygon* in_poly, polygon* out_poly, polygon* out_poly2, vector<vec3d>* vertices)
{
	out_poly->m = in_poly->m;
	out_poly2->m = in_poly->m;
	if (in_poly->p.size() == 3) {
		return Triangle_ClipAgainstPlane(plane_p, plane_n, in_poly, out_poly, out_poly2, vertices);
	}

	plane_n.normalise();
	auto dist = [&](vec3d& p)
	{
		vec3d n = p; n.normalise();
		return (plane_n.x * n.x + plane_n.y * n.y + plane_n.z * n.z - Vector_DotProduct(plane_n, plane_p));
	};

	const int size = in_poly->p.size();
	vector<size_t> inside_points;  int nInsidePointCount = 0;
	vector<size_t> outside_points; int nOutsidePointCount = 0;
	vector<float> distances;

	for (size_t& i : in_poly->p) {
		distances.push_back(dist(vertices->at(i)));
	}

	for (float d : distances) {
		if (d >= 0) {
			inside_points.push_back(in_poly->p[nInsidePointCount + nOutsidePointCount]); nInsidePointCount++;
		}
		else {
			outside_points.push_back(in_poly->p[nInsidePointCount + nOutsidePointCount]); nOutsidePointCount++;
		}
	}

	if (nInsidePointCount == 0)
	{
		out_poly->p = { 0, 0, 0 };
		return 0;
	}
	
	if (nInsidePointCount == in_poly->p.size())
	{
		out_poly = in_poly;

		return 1;
	}
	else
	{

		for (size_t i : inside_points) {
			out_poly->p.push_back(i);
		}
		
		vertices->at(outside_points[outside_points.size() - 1]) = Vector_IntersectPlane(plane_p, plane_n, vertices->at(inside_points[0]), vertices->at(outside_points[outside_points.size() - 1]));
		out_poly->p.push_back(outside_points[outside_points.size() - 1]);
		vertices->at(outside_points[0]) = Vector_IntersectPlane(plane_p, plane_n, vertices->at(inside_points[0]), vertices->at(outside_points[0]));
		out_poly->p.push_back(outside_points[0]);

		return 1;
	}
}

polygon marchingCubes(vector<vec3d> cubes, int ptr) {
	return polygon();
}

mesh::mesh() {};

mesh::mesh(vec3d* verts, size_t vsize, polygon* polys, size_t psize) {
	this->vertices.clear();
	for (size_t i = 0; i < vsize; i++) {
		this->vertices.push_back(verts[i]);
	}
	this->polygons.clear();
	for (size_t i = 0; i < psize; i++) {
		this->polygons.push_back(polys[i]);
	}
	

}
mesh::mesh(vector<vec3d> verts, vector<polygon> polys) {
	this->vertices = verts;
	this->polygons = polys;
}
