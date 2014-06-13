#include "CAssimpModel.h"

CAssimpModel::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	numIndices  = 0;
	materialIndex = INVALID_OGL_VALUE;
};

CAssimpModel::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void CAssimpModel::MeshEntry::init(const std::vector<Vertex>& vertices,
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
	//for(int i = 0;i < textures.size(); i++)
	//{
	//	free(textures[i]);
	//}
}

CAssimpModel::CAssimpModel(ShaderProgram& p)
{
	prog = &p;
	isLoaded = false;
}

bool CAssimpModel::LoadModelFromFile(const std::string& filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( filepath,  aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!scene) {
		cout<<"Couldn't load model, Error Importing Asset"<<endl;
		return false;
	}

	meshEntries.resize(scene->mNumMeshes);

	for (int i=0; i<scene->mNumMeshes; i++) {
		aiMesh* paiMesh = scene->mMeshes[i];
		int iMeshFaces = paiMesh->mNumFaces;
		cout<<"Mesh "<<i<<" faces:"<<iMeshFaces<<endl;
		initMesh(i, paiMesh);
	}
	cout<<scene->mNumMaterials<<endl;

	
	// TODO: no materials
	//textures.resize(scene->mNumMaterials);
	//if (scene->mNumMaterials > 1) {
	//	initMaterial(scene,filepath);
	//}

	cout<<"scene->mNumMeshes:"<<scene->mNumMeshes<<endl;
	cout<<"scene->mNumMaterials:"<<scene->mNumMaterials<<endl;
	aiMesh* paiMesh = scene->mMeshes[0];
	cout<<"mesh0->mNumFaces:"<<paiMesh->mNumFaces<<endl;
	cout<<"mesh0->mNumVertices:"<<paiMesh->mNumVertices<<endl;



	isLoaded = true;
	return true;
}

void CAssimpModel::initMesh(unsigned int index, const aiMesh* paiMesh)
{
	meshEntries[index].materialIndex = paiMesh->mMaterialIndex;
	cout<<"mesh"<<index<< "mMaterialIndex"<< paiMesh->mMaterialIndex<<endl;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	vertices.reserve(paiMesh->mNumVertices);
	indices.reserve(paiMesh->mNumFaces);

	for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D; // set zero if not has texture

		Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
				 glm::vec2(pTexCoord->x, pTexCoord->y),
				 glm::vec3(pNormal->x, pNormal->y, pNormal->z)
				);
		aabb.expand(glm::vec3(pPos->x, pPos->y, pPos->z));
		// cout<<i<<": "<<v.m_tex.x<<v.m_tex.y<<endl;
		vertices.push_back(v);
	}

	fScale = aabb.max.x - aabb.min.x;
	fScale = max(aabb.max.y - aabb.min.y, fScale);
	fScale = max(aabb.max.z - aabb.min.z, fScale);
	fScale = 1.f/fScale * 5;
	cout << "fScale = " << fScale << endl;	

	// TODO: move outside;
	glm::vec3 center = aabb.max+aabb.min;
	center.x /= 2.f; center.y /= 2.f; center.z /= 2.f;
	aabb.max -= center;
	aabb.min -= center;
	cout << "center =  ( " << center.x << ", " << center.y << ", " << center.z << " )\n"; 
	for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
		it->m_pos -= center;
		it->m_pos.x /= fScale;
		it->m_pos.y /= fScale;
		it->m_pos.z /= fScale;
	}

	for(unsigned int i=0; i<paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		indices.push_back(Face.mIndices[0]);
		indices.push_back(Face.mIndices[1]);
		indices.push_back(Face.mIndices[2]);
	}

	meshEntries[index].init(vertices, indices);
}

void CAssimpModel::render()
{

	if(!isLoaded) return;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0 ; i < meshEntries.size() ; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec2)+sizeof(glm::vec3)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEntries[i].IB);

		const unsigned int materialIndex = meshEntries[i].materialIndex;

		//if (materialIndex < textures.size() && textures[materialIndex]) {
		//	textures[materialIndex]->bind(GL_TEXTURE0);
		//	//prog->setUniform("Tex2", 0);
		//	//cout<<"Draw "<<i<<endl;
		//}


		glDrawElements(GL_TRIANGLES, meshEntries[i].numIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

//bool CAssimpModel::initMaterial(const aiScene* pScene, const string& filePath)
//{
//	string::size_type slashIndex = filePath.find_last_of("/");
//	string dir;
//
//	if (slashIndex == string::npos) {
//		dir = ".";
//	}
//	else if (slashIndex == 0) {
//		dir = "/";
//	}
//	else {
//		dir = filePath.substr(0, slashIndex);
//	}
//
//
//	bool Ret = true;
//	// Initialize the materials
//	for (unsigned int i = 1 ; i < pScene->mNumMaterials ; i++)
//	{
//		const aiMaterial* pMaterial = pScene->mMaterials[i];
//		textures[i] = NULL;
//
//		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
//			aiString path;
//
//			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//				string fullPath = dir + "/" + path.data;
//				textures[i] = new CTexture(GL_TEXTURE_2D, fullPath.c_str());
//
//				if (!textures[i]->load()) {
//					printf("Error loading texture '%s'\n", fullPath.c_str());
//					delete textures[i];
//					textures[i] = NULL;
//					Ret = false;
//				}
//				else {
//					printf("Loaded texture '%s'\n", fullPath.c_str());
//				}
//			}
//		}
//
//		// Load a white texture in case the model does not include its own texture
//		if (textures[i]==NULL) {
//			textures[i] = new CTexture(GL_TEXTURE_2D, "assets/textures/white.png");
//			Ret = textures[i]->load();
//		}
//	}
//
//	return Ret;
//}
