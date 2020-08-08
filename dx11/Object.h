#pragma once
#include "Mesh.h"

#include <memory>

using MeshPtr = std::shared_ptr<Mesh>;



class Object
{
public:
	Object() { };
	Object(MeshPtr initMesh);
	~Object();

	void Render() { mesh->Draw(); };

	void SetPosition(Vector3 newPos);

private:
	MeshPtr mesh;
	Vector3 position;

	Matrix worldMatrix;

};

