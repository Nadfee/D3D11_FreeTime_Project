#include "Object.h"

Object::Object(MeshPtr initMesh) :
	mesh(initMesh),
	translationMatrix(Matrix(
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 0.f, 0.f, 1.f)
	)),
	rotationMatrix(Matrix(
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 0.f, 0.f, 1.f)
	)),
	xRotationDeg(0.f),
	yRotationDeg(0.f),
	zRotationDeg(0.f)
{

}

Object::~Object()
{
}

void Object::SetPosition(const Vector3& newPos)
{
	position = newPos;
	translationMatrix.Translation(Vector3(newPos.x, newPos.y, newPos.z));

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

void Object::FinalizeMatrixResults()
{
	mesh->UpdateWorldMatrix(rotationMatrix * translationMatrix); // Rotation first in local space
}
