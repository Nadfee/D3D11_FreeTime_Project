#pragma once
#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

using MeshPtr = std::shared_ptr<Mesh>;

class Object
{
public:
	Object();
	Object(const std::string& identifier, std::vector<MeshPtr> initMeshes);
	~Object();

	void SetRender(bool shouldRender);

	bool ShouldRender() { return meshes[0]->ShouldRender(); };		// Just return any meshes shouldRender
	Vector3 GetPosition() { return position; };
	const std::string& GetID() const { return id; };
	const std::vector<std::size_t> GetMeshIDs() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& newPos);
	void SetPosition(const Vector4& newPos);
	void SetRotation(float xRad, float yRad, float zRad);


	// Uniform scaling x, y, z
	void SetScaling(float scale);

	bool operator==(Object const& other) const { return id == other.id; };		// For unordered map. Return true if same name ID! (Will replace old object)

private:
	std::string id;
	std::vector<MeshPtr> meshes;
	Vector3 position;
	float xRotationDeg, yRotationDeg, zRotationDeg;

	// These matrices take up a lot of space!
	Matrix translationMatrix;		
	Matrix rotationMatrix;
	Matrix scalingMatrix;

	void FinalizeMatrixResults();

};

