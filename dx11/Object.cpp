#include "Object.h"

Object::Object(MeshPtr initMesh) :
	mesh(initMesh)
{
}

Object::~Object()
{
}

void Object::SetPosition(const Vector3& newPos)
{
	position = newPos;
	mesh->UpdateWorldMatrix(newPos);
}
