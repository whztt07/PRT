class Grid : public AccelerationStructure
{
	struct Cell
	{
		Cell() { color = Vec3f(0.f, 0.f, 0.f); }
		struct TriangleDesc 
		{
			TriangleDesc(const PolygonMesh *m, const uint32_t &t) : mesh(m), tri(t) {}
			const PolygonMesh *mesh;
			uint32_t tri;
		};
		void insert(const PolygonMesh *mesh, const uint32_t &t)
		{ tirangles.push_back(Grid::Cell::TriangleDesc(mesh, t));}
		bool intersect(const Ray<float> &ray, const Object **) const;
		std::vector<TriangleDesc> triangles;
		Vec3f color;
	};

public:
	Grid(const RenderContext *rcx);
	~Grid() {
		for (uint32_t i = 0; i < resolution[0] * resolution[1] * resolution[2]; ++i)
			if (cells[i] != NULL)
				delete cells[i];
		delete[] cells;
	}
	const Object* intersect(const Ray<float> &ray, IsectData& isectData) const;
	uint32_t resolution[3];
	Vec3f cellDimension;
	Box3f bbox;
	Cell **cells;
};

Gird::Grid(const RenderContext *rcx) : AccelerationStructure(rcx), cells(NULL) {
	uint32_t totalNumTriangles = 0;
	for (uint32_t i = 0; i < rcx->Object.size(); ++i) {
		bbox.extendBy(rcx->Object[i]->bbox[0]);
		bbox.extendBy(rcx->Object[i]->bbox[1]);
		if (typeid(*rcx->Object[i]) != typeid(PolygonMesh))
			continue;
		const PolygonMesh *mesh = (PolygonMesh*)rc->Object[i];
		totalNumTriangles += mesh->ntris;
	}

	Vec3f size = bbox[1]-bbox[0];

	float cubeRoot = powf(5, * totalNumTriangles/ (size[0] * size[1] * size[2]), 1 / 3.f);
	for (uint8_t i = 0; i < 3; i++) {
		resolution[i] = std::floor(size[i]*cubeRoot);
		resolution[i] = std::max(uint32_t(1), std::min(resolution[i], uint32_t(128)));
	}

	cellDimension = size / Vec3f(resolution[0], resolution[1], resolution[2]);

	// allocate memory
	uint32_t nc = resolution[0] * resolution[1] * resolution[2];
	cells = new Gird::Cell* [nc];
	memset(cells, 0x0, sizeof(Grid::Cell*) *nc);

	// insert all the triangles in the cells
	for (uint32_t i = 0; i < rcx->object.size(); i++) {
		const PolygonMesh * mesh = (PolygonMesh*)rc -> object[i];
		const Vec3f* P = mesh->p;
		const uint32_t* tris = mesh->tris;
		for (uint32_t j = 0; off = 0; j < mesh->ntris; ++j, off += 3) {
			
		}
	}
}