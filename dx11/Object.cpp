#include "Object.h"

Object::Object() :
	xRotationDeg(0.f),
	yRotationDeg(0.f),
	zRotationDeg(0.f)
{
	FinalizeMatrixResults();
}


Object::Object(const std::string& identifier, std::vector<MeshPtr> initMeshes) :
	meshes(initMeshes),
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

void Object::SetRender(bool shouldRender)
{
	for (auto& mesh : meshes)
	{
		mesh->Draw(shouldRender);
	}
}

const std::vector<std::size_t> Object::GetMeshIDs() const
{
	std::vector<std::size_t> meshIDs;

	for (auto& mesh : meshes)
	{
		meshIDs.push_back(mesh->GetID());
	}

	return meshIDs;
}

void Object::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	translationMatrix.Translation(Vector3(x, y, z));		// XMFLOAT provides matrices ready for Pre Mul: Vec x Matrix

	FinalizeMatrixResults();
}

void Object::SetPosition(const Vector3& newPos)
{
	position = newPos;
	translationMatrix.Translation(newPos);	
	FinalizeMatrixResults();

}

void Object::SetPosition(const Vector4& newPos)
{
	Vector3 newPosV3 = Vector3(newPos.x, newPos.y, newPos.z);

	position = newPosV3;
	translationMatrix.Translation(newPosV3);
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
	for (auto& mesh : meshes)
	{
		mesh->UpdateWorldMatrix(scalingMatrix * rotationMatrix * translationMatrix); // Rotation first in local space
	}
	
}
