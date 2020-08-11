#pragma once
#include "Mesh.h"

#include <memory>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

using MeshPtr = std::shared_ptr<Mesh>;



class Object
{
public:
	Object();
	Object(const std::string& identifier, MeshPtr initMesh);
	~Object();

	void SetRender(bool shouldRender) { mesh->Draw(shouldRender); };

	bool ShouldRender() { return mesh->ShouldRender(); };
	Vector3 GetPosition() { return position; };
	const std::string& GetID() const { return id; };
	const std::size_t GetMeshID() const { return mesh->GetID(); };
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& newPos);
	void SetPosition(const Vector4& newPos);
	void SetRotation(float xRad, float yRad, float zRad);


	// Uniform scaling x, y, z
	void SetScaling(float scale);

	bool operator==(Object const& other) const { return id == other.id; };		// For unordered map. Return true if same name ID! (Will replace old object)

private:
	std::string id;
	MeshPtr mesh;
	Vector3 position;
	float xRotationDeg, yRotationDeg, zRotationDeg;

	// These matrices take up a lot of space!
	Matrix translationMatrix;		
	Matrix rotationMatrix;
	Matrix scalingMatrix;

	void FinalizeMatrixResults();

};

