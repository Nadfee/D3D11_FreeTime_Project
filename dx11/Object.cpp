#include "Object.h"

Object::Object() :
	xRotationDeg(0.f),
	yRotationDeg(0.f),
	zRotationDeg(0.f)
{
	FinalizeMatrixResults();
}


Object::Object(const std::string& identifier, MeshPtr initMesh) :
	mesh(initMesh),
	translationMatrix(Matrix::Identity),
	rotationMatrix(Matrix::Identity),
	scalingMatrix(Matrix::Identity),
	xRotationDeg(0.f),
	yRotationDeg(0.f),
	zRotationDeg(0.f),
	id(identifier)
{
	FinalizeMatrixResults();
}

Object::~Object()
{
}

void Object::SetPosition(const Vector3& newPos)
{
	position = newPos;
	translationMatrix.Translation(Vector3(newPos.x, newPos.y, newPos.z));		// XMFLOAT provides matrices ready for Pre Mul: Vec x Matrix


	FinalizeMatrixResults();
}

void Object::SetRotation(float xDeg, float yDeg, float zDeg)
{
	xRotationDeg = xDeg;
	yRotationDeg = yDeg;
	zRotationDeg = zDeg;

	float xRad = (M_PI / 180.f) * (xDeg);
	float yRad = (M_PI / 180.f) * (yDeg);
	float zRad = (M_PI / 180.f) * (zDeg);

	// Counter clockwise rotation (positive)
	rotationMatrix = Matrix::CreateRotationX(xRad);
	rotationMatrix *= Matrix::CreateRotationY(yRad);
	rotationMatrix *= Matrix::CreateRotationZ(zRad);

	FinalizeMatrixResults();
}

void Object::SetScaling(float scale)
{
	scalingMatrix = Matrix::CreateScale(scale);
	FinalizeMatrixResults();
}

void Object::FinalizeMatrixResults()
{
	mesh->UpdateWorldMatrix(scalingMatrix * rotationMatrix * translationMatrix); // Rotation first in local space
}
