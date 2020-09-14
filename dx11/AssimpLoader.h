#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <vector>
#include <string>

#include "Mesh.h"


class AssimpLoader
{
public:
	AssimpLoader();
	~AssimpLoader();

	std::vector<std::vector<Vertex>> GetMeshesVertexData() { return vertexData; }
	std::vector<std::string> GetMeshesTextures() { return textures; }

	void Clear();
	void LoadModel(const std::string& filePath);

private:
	std::vector<std::vector<Vertex>> vertexData;	// HOLY SHIT We are storing ALL THE VERTICES!! MEMORY!!!
	std::vector<std::string> textures;

	std::vector<unsigned int> m_indices;
	std::vector<Vertex> m_vertexData;

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);



};

