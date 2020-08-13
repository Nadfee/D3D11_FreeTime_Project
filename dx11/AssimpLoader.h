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

	void LoadModel(const std::string& filePath);

private:
	std::vector<std::vector<Vertex>> vertexData;
	std::vector<std::string> textures;

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);




};

