#include "Object.h"

Object::Object(MeshPtr initMesh) :
	mesh(initMesh)
{
}

Object::~Object()
{
}

void Object::SetPosition(Vector3 newPos)
{
	position = newPos;
	mesh->UpdateWorldMatrix(newPos);
}
