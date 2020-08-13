#include "AssimpLoader.h"

AssimpLoader::AssimpLoader()
{
}

AssimpLoader::~AssimpLoader()
{
}

void AssimpLoader::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (scene == nullptr)
	{
		OutputDebugStringW(L"Assimp: File not found!");
		assert(false);
	}

	ProcessNode(scene->mRootNode, scene);


}

void AssimpLoader::ProcessNode(aiNode* node, const aiScene* scene)
{
	// For each mesh in the node, process it!
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}

}

void AssimpLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> tempVertices;
	std::vector<DWORD> indices;

	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vert = { };
		vert.position.x = mesh->mVertices[i].x;
		vert.position.y = mesh->mVertices[i].y;
		vert.position.z = mesh->mVertices[i].z;

		vert.normal.x = mesh->mNormals[i].x;
		vert.normal.y = mesh->mNormals[i].y;
		vert.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vert.uv.x = mesh->mTextureCoords[0][i].x;
			vert.uv.y = mesh->mTextureCoords[0][i].y;
		}

		tempVertices.push_back(vert);

	}

	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}

	}

	std::vector<Vertex> finalData;
	// Organize
	for (int i = 0; i < indices.size(); ++i)
	{
		Vertex vert = { };

		vert = tempVertices[indices[i]];
		finalData.push_back(vert);
	}

	vertexData.push_back(finalData);

	auto mtl = scene->mMaterials[mesh->mMaterialIndex];
	aiString path;
	mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	std::string textPath(path.C_Str());

	if (textPath == "")
	{
		textPath = "default.png";
	}

	textures.push_back(textPath);


}
