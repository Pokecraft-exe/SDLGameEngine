#include "maths_engine.h"

float radToDeg(float radian) { return radian * 180 / 3.14159f; }
camera::camera() { Near = 0.0f; }
camera::camera(int h, int w) {
	Near = 0.1f;
	Far = 1000.0f;
	Fov = 90.0f;
	AspectRatio = (float)h / (float)w;
	FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);

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

vec3d calculate_normal(polygon& poly) {
	vec3d v1 = Vector_Sub(poly.p[1], poly.p[0]);
	vec3d v2 = Vector_Sub(poly.p[2], poly.p[0]);
	vec3d normal;
	normal = Vector_CrossProduct(v1, v2);
	normal.normalise();
	return normal;
}