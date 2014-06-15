#ifndef _ACCELERATIONSTRUCTURE_H_
#define _ACCELERATIONSTRUCTURE_H_
#include <vector>
#include <climits>
#include "global.h"
#include "CAssimpModel.h"
using std::vector;

//typedef vector<uint32_t> Cell;
class IsectData;

class Grid
{
	struct Cell {
		// TODO remove model
		Cell(CAssimpModel* model) : model(model) {}
		void insert(uint32_t idx) { triangles.push_back(idx); };
		bool intersect(Ray& ray) const;
		// tris id in model->indices
		vector<uint32_t> triangles;
		CAssimpModel* model;
	};
public:
	Grid(CAssimpModel* model);
	~Grid() {
		// TODO
	}
	bool intersect(Ray& ray) const;
	uint32_t resolution[3];
	vec3 cellDimension;
	int ncell;
	AABB bbox;
	Cell** cells;
	CAssimpModel* model;
};

#endif