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

	Vector3 GetPosition() { return position; }
	void SetPosition(const Vector3& newPos);

private:
	MeshPtr mesh;
	Vector3 position;

	Matrix worldMatrix;

};

