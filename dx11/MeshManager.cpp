#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

void MeshManager::AddMesh(MeshPtr mesh)
{
	std::size_t key = std::hash<MeshPtr>{}(mesh);
	mesh->SetManagerKey(key);
	MeshHash hash = { mesh };

	meshes.insert({ key, hash });
}

bool MeshManager::RemoveMesh(std::size_t identifier)
{
	if (meshes.erase(identifier) == 0)
	{
		return false;
	}

	return true;
}
