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
	Sample() : sh_functions(NULL) {}
	~Sample() {
		if (NULL !=sh_functions)
			delete []sh_functions;
		sh_functions = NULL;
	}
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


struct Image {
	int width;
	int height;
	float* pixel[3];
	bool loadFromFile(const char* filename);
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

struct AABB {
	Vector3 min;
	Vector3 max;
	AABB() : max(INT_MIN, INT_MIN, INT_MIN), min(INT_MAX, INT_MAX, INT_MAX) {}
	
	void expand(const Vector3& p) {
		if (max.x < p.x) max.x = p.x;
		if (max.y < p.y) max.y = p.y;
		if (max.z < p.z) max.z = p.z;
		if (min.x > p.x) min.x = p.x;
		if (min.y > p.y) min.y = p.y;
		if (min.z > p.z) min.z = p.z;
	}

	bool intersect(Ray &r) const
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;
		if (r.dir.x < 0) {
			tmin = (max.x - r.orig.x) / r.dir.x;
			tmax = (min.x - r.orig.x) / r.dir.x;
		} else {
			tmax = (max.x - r.orig.x) / r.dir.x;
			tmin = (min.x - r.orig.x) / r.dir.x; 
		}
		if (r.dir.y < 0) {
			tymin = (max.y - r.orig.y) / r.dir.y;
			tymax = (min.y - r.orig.y) / r.dir.y;
		} else {
			tymax = (max.y - r.orig.y) / r.dir.y;
			tymin = (min.y - r.orig.y) / r.dir.y; 
		}
		if (r.dir.z < 0) {
			tzmin = (max.z - r.orig.z) / r.dir.z;
			tzmax = (min.z - r.orig.z) / r.dir.z;
		} else {
			tzmax = (max.z - r.orig.z) / r.dir.z;
			tzmin = (min.z - r.orig.z) / r.dir.z; 
		}

		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;
		if (tmin > r.tmin) r.tmin = tmin;
		if (tmax < r.tmax) r.tmax = tmax;
		return true;
	}
	
};


#endif