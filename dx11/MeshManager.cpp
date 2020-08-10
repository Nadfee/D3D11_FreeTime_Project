#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

void MeshManager::UpdateMeshes(std::string identifier, MeshPtr mesh)
{
	MeshHash hash = { mesh };

	meshes.insert({ identifier, hash });
}
