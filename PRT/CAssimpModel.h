#ifndef _CASSIMPMODEL_H
#define _CASSIMPMODEL_H
#define INVALID_OGL_VALUE 0xFFFFFFFF
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include <string>
#include "global.h"
#include "ShaderProgram.h"
using namespace std;

struct Vertex
{
    glm::vec3 m_pos;
	glm::vec3 m_color;	// including lighting
    glm::vec3 m_normal;
	//glm::vec3 diffuseMaterial;
	float* unshadowedCoeffs;
	float* shadowedCoeffs;
	

    Vertex() {}
	Vertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& normal) 
		: m_pos(pos), m_color(color), m_normal(normal) { }
	Vertex(float vx, float vy, float vz, float cr, float cg, float cb, float nx, float ny, float nz)
		: m_pos(vx, vy, vz), m_color(cr, cg, cb), m_normal(nx, ny, nz) {}
};

class CAssimpModel
{
public:
    CAssimpModel();
    ~CAssimpModel();
    bool LoadModelFromFile(const string& filename);
    void render();
	void render2();

	float fScale;	// scale size to show model properly;
	AABB aabb;		// bounding box
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	struct MeshEntry {
        MeshEntry();
        ~MeshEntry();

        void bindBuffer(const std::vector<Vertex>& Vertices,
						const std::vector<unsigned int>& Indices);
        
        GLuint VB;		//For vertex buffer
        GLuint IB;		//For index buffer
        unsigned int numIndices;
    };
    MeshEntry meshEntry;

private:
    bool isLoaded;
    int iNumMaterials;
};

#endif // CASSIMPMODEL_H
