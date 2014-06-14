#include "CAssimpModel.h"

CAssimpModel::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	numIndices  = 0;
	//materialIndex = INVALID_OGL_VALUE;
};

CAssimpModel::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &IB);
	}
}

void CAssimpModel::MeshEntry::bindBuffer(const std::vector<Vertex>& vertices,
								   const std::vector<unsigned int>& indices)
{
	numIndices = indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);
}

CAssimpModel::CAssimpModel()
{
	isLoaded = false;
	fScale = 1.0f;
}

CAssimpModel::~CAssimpModel()
{
}


bool CAssimpModel::LoadModelFromFile(const std::string& filepath)
{
	/* load file with assimp and print some stats */
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( filepath,  aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (!scene) {
		cout << "[ERROR] reading mesh " << filepath << endl; 
		return false;
	}

	// get each mesh
	int nvertices = 0;
	int ntriangles = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		nvertices += mesh->mNumVertices;
		ntriangles += mesh->mNumFaces;
	}
	printf ("    %i vertices and %i triangles\n", nvertices, ntriangles);

	vertices.reserve(nvertices);
	indices.reserve(ntriangles);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			aiVector3D& v = mesh->mVertices[j];
			aiVector3D& n = mesh->mNormals[j];
			//aiVector3D pTexCoord = Zero3D; // set zero if not has texture
			vertices.push_back(Vertex(v.x, v.y, v.z, 0.0f, 0.0f, 0.0f, n.x, n.y, n.z));
			aabb.expand(glm::vec3(v.x, v.y, v.z));
		}
		for(unsigned int i=0; i < mesh->mNumFaces; i++) {
			const aiFace& Face = mesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0]);
			indices.push_back(Face.mIndices[1]);
			indices.push_back(Face.mIndices[2]);
		}
	}
	fScale = aabb.max.x - aabb.min.x;
	fScale = max(aabb.max.y - aabb.min.y, fScale);
	fScale = max(aabb.max.z - aabb.min.z, fScale);
	fScale = 1.f/fScale * 5;
	cout << "fScale = " << fScale << endl;	
	//fScale = 60.f;

	glm::vec3 center = aabb.max+aabb.min;
	center.x /= 2.f; center.y /= 2.f; center.z /= 2.f;
	aabb.max -= center;
	aabb.min -= center;
	cout << "center =  ( " << center.x << ", " << center.y << ", " << center.z << " )\n";
	cout << "fscale = " << fScale << endl;
	for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
		it->m_pos -= center;
		it->m_pos.x *= fScale;
		it->m_pos.y *= fScale;
		it->m_pos.z *= fScale;
	}
	//meshEntry.bindBuffer(vertices, indices);

	isLoaded = true;
	return true;
}

void CAssimpModel::render()
{

	if(!isLoaded) return;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, meshEntry.VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);	// position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)));// color
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEntry.IB);

	glDrawElements(GL_TRIANGLES, meshEntry.numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
}

void CAssimpModel::render2()
{

}