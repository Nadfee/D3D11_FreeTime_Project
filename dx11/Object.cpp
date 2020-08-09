#include "Object.h"

Object::Object(MeshPtr initMesh) :
	mesh(initMesh),
	worldMatrix(Matrix(
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 0.f, 0.f, 1.f)
		))
{

}

Object::~Object()
{
}

void Object::SetPosition(const Vector3& newPos)
{
	position = newPos;
	worldMatrix._41 = newPos.x;
	worldMatrix._42 = newPos.y;
	worldMatrix._43 = newPos.z;

	mesh->UpdateWorldMatrix(worldMatrix);
}
