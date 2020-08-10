#pragma once

#include <SimpleMath.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera();
	Camera(float fovAngleDeg, float aspectRatio, float nearZ = 0.1f, float farZ = 500.f);
	~Camera();

	Matrix GetViewMatrix() { return viewMatrix; };
	Matrix GetProjectionMatrix() { return projectionMatrix; };
	Vector4 GetPosition() { return position; };
	Vector4 GetForward() { return camForward; }

	void Update(int deltaX, int deltaY, float moveLeftRight, float moveForwardBack, float moveUpDown, float speed, double frameTime);

private:
	Matrix viewMatrix, projectionMatrix;

	Vector4 camForward, camRight, camUp, upDir;
	Vector4 position;

	float fovAngle, aspectRatio, nearZ, farZ;
	float camPitch, camYaw;



};

