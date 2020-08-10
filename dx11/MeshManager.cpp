#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

void MeshManager::UpdateMeshes(MeshPtr mesh)
{
	std::size_t key = std::hash<MeshPtr>{}(mesh);
	mesh->SetManagerKey(key);
	MeshHash hash = { mesh };

	meshes.insert({ key, hash });
}
