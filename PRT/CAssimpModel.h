#ifndef CASSIMPMODEL_H
#define CASSIMPMODEL_H
#define INVALID_OGL_VALUE 0xFFFFFFFF
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include "shaderprogram.h"
#include "prt.h"
//#include "ctexture.h"
//#include "util.h"
using namespace std;

// bounding box
//struct AABB {
//	glm::vec3 min;
//	glm::vec3 max;
//	void expand(glm::vec3& p) {
//		if (max.x < p.x) max.x = p.x;
//		if (max.y < p.y) max.y = p.y;
//		if (max.z < p.z) max.z = p.z;
//		if (min.x > p.x) min.x = p.x;
//		if (min.y > p.y) min.y = p.y;
//		if (min.z > p.z) min.z = p.z;
//	}
//};

struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;

    Vertex() {}
	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal) 
		: m_pos(pos), m_tex(tex), m_normal(normal) {}
};

class CAssimpModel
{
public:
    CAssimpModel();
    ~CAssimpModel();
    CAssimpModel(ShaderProgram& p);
    bool LoadModelFromFile(const string& filename);
    void render();

	float fScale;	// scale size to show model properly;
	AABB aabb;		// bounding box
private:
    bool isLoaded;
    int iNumMaterials;
    struct MeshEntry {
        MeshEntry();
        ~MeshEntry();

        void init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        
        GLuint VB;		//For vertex buffer
        GLuint IB;		//For index buffer
        unsigned int numIndices;
        unsigned int materialIndex;
    };

    void initMesh(unsigned int Index, const aiMesh* paiMesh);
    //bool initMaterial(const aiScene* pScene, const string& filePath);
    ShaderProgram* prog;
    vector<MeshEntry> meshEntries;
    //vector<CTexture*> textures;
};

#endif // CASSIMPMODEL_H
