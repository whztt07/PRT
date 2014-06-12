#pragma once
#ifndef _PRT_H_
#define _PRT_H_

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <assimp/cimport.h>		// C importer
#include <assimp/scene.h>		// collects data
#include <assimp/postprocess.h> // various extra operations
#include "SHEval.h"
using std::cout;

const float PI = 3.1415926f;

struct Vector3
{
	float x;
	float y;
	float z;
	Vector3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}
	void setValue(float _x = 0.f, float _y = 0.f, float _z = 0.f) {x = _x; y = _y; z = _z; }
	void setValue(const aiVector3D& v) {x = v.x; y = v.y; z = v.z; }
	void scale(float factor) { x*=factor; y*=factor; z*=factor; }
	Vector3& operator=(const aiVector3D& v) {x = v.x; y = v.y; z = v.z; return *this; }
	Vector3& operator-=(const Vector3& v) {x -= v.x; y -= v.y; z -= v.z; return *this;}
	friend Vector3 operator+(const Vector3& l, const Vector3& r) {return Vector3(l.x+r.x, l.y+r.y, l.z+r.z);}
	friend Vector3 operator-(const Vector3& l, const Vector3& r) {return Vector3(l.x-r.x, l.y-r.y, l.z-r.z);}
	friend Vector3 operator/(const Vector3& v, const float f) {return Vector3(v.x/f, v.y/f, v.z/f); }
	float dot(const Vector3& v) const { return x*v.x + y*v.y + z*v.z; }
	Vector3 cross(const Vector3& v) const { return Vector3(y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y); };
};

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
	void setValue(int _a, int _b, int _c) {a=_a; b=_b; c=_c; }
};

struct Color
{
	float r;
	float g;
	float b;
	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {};
};

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

struct Scene
{
	Vector3*	vertices;
	Vector3*	normals;
	Triangle*	triangles;
	Color*	albedo;
	AABB	aabb;
	//Vector3	center;
	int*	material;
	int		number_of_vertices;
	int		number_of_triangles;
	Scene() : vertices(NULL), normals(NULL), triangles(NULL), albedo(NULL), material(NULL), number_of_triangles(0), number_of_vertices(0) {}
	bool loadMeshFromFile(const char* filename);
};

void GenerateSamples(Sampler* sampler, int N);
void PrecomputeSHFunctions(Sampler* sampler, int bands);
void LightProbeAccess(Color* color, Image* image, Vector3& direction);
void ProjectLightFunction(Color* coeffs, Sampler* sampler, Image* light, int bands);
void ProjectUnshadowed(Color** coeffs, Sampler* sampler, Scene* scene, int bands);
void ProjectShadowed(Color** coeffs, Sampler* sampler, Scene* scene, int bands);

#endif