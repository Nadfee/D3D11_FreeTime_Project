#pragma once
#include "Mesh.h"

#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>

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
	void SetRotation(float xRad, float yRad, float zRad);

private:
	MeshPtr mesh;
	Vector3 position;
	float xRotationDeg, yRotationDeg, zRotationDeg;

	// These matrices take up a lot of space!
	Matrix translationMatrix;		
	Matrix rotationMatrix;

	void FinalizeMatrixResults();

};

