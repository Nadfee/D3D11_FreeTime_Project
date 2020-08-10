#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include <string>

#include "Mesh.h"

using MeshPtr = std::shared_ptr<Mesh>;

struct MeshHash
{
	MeshPtr mesh;

	bool operator==(MeshPtr const& other) const { return mesh == other; };
};


class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	const std::unordered_map<std::string, MeshHash>& GetMeshes() { return meshes; };
	void UpdateMeshes(std::string identifier, MeshPtr mesh);

private:
	std::unordered_map<std::string, MeshHash> meshes;
	

};



