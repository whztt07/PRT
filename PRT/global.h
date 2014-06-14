#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <glm/glm.hpp>
using std::cout;
using glm::vec3;

typedef vec3 Vector3;
typedef vec3 Color;
const float PI = 3.1415926f;

struct Spherical
{
	float theta;
	float phi;
};

struct Sample
{
	//Spherical spherical_coord;
	Vector3   cartesian_coord;
	float*    sh_functions;
};

struct Sampler
{
	Sample* samples;
	int number_of_samples;
};

struct Triangle
{
	int a;
	int b;
	int c;
	Triangle(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {}
	void setValue(int _a, int _b, int _c) {a=_a; b=_b; c=_c; }
};

//struct Color
//{
//	float r;
//	float g;
//	float b;
//	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {};
//};

struct Image {
	int width;
	int height;
	float* pixel[3];
	bool loadFromFile(const char* filename);
};

struct AABB {
	Vector3 min;
	Vector3 max;
	void expand(Vector3& p) {
		if (max.x < p.x) max.x = p.x;
		if (max.y < p.y) max.y = p.y;
		if (max.z < p.z) max.z = p.z;
		if (min.x > p.x) min.x = p.x;
		if (min.y > p.y) min.y = p.y;
		if (min.z > p.z) min.z = p.z;
	}
};

class Ray
{
public:
	vec3 color;
	vec3 orig;
	vec3 dir;
	float tmin;
	float tmax;

	Ray(vec3 orig, vec3 dir, float near = 0, float far = 1000) : orig(orig), dir(dir), tmin(near), tmax(far) {}
	vec3 operator() (const float t) const
	{ return orig + dir*t; }
};


#endif