#include "Camera.h"

Camera::Camera() :
	position(Vector4(0.f, 0.f, 0.f, 1.f)),
	fovAngle(0.f),
	aspectRatio(0.f),
	nearZ(0.1f),
	farZ(500.f),
	camForward(Vector4(0.f, 0.f, 1.f, 0.f)),
	camUp(Vector4(0.f, 1.f, 0.f, 0.f)),
	camRight(Vector4(1.f, 0.f, 0.f, 0.f)),
	upDir(Vector4(0.f, 1.f, 0.f, 0.f)),
	camYaw(0),
	camPitch(0)
{

}

Camera::Camera(float fovAngleDeg, float aspectRatio, float nearZ, float farZ) :
	position(Vector4(0.f, 0.f, 0.f, 1.f)),
	fovAngle(fovAngleDeg),
	aspectRatio(aspectRatio),
	nearZ(nearZ),
	farZ(farZ),
	camForward(Vector4(0.f, 0.f, 1.f, 0.f)),
	camUp(Vector4(0.f, 1.f, 0.f, 0.f)),
	camRight(Vector4(1.f, 0.f, 0.f, 0.f)),
	upDir(Vector4(0.f, 1.f, 0.f, 0.f)),
	camYaw(0),
	camPitch(0)
{
	float fovAngleRad = (M_PI / 180.f) * (fovAngleDeg);

	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovAngleRad, aspectRatio, nearZ, farZ);
	viewMatrix = DirectX::XMMatrixLookAtLH(Vector4(0.f, 0.f, 0.f, 1.f), Vector4(0.f, 0.f, 1.f, 1.f), Vector4(0.f, 1.f, 0.f, 0.f));
}

Camera::~Camera()
{
}

void Camera::Update(int deltaX, int deltaY, float moveLeftRight, float moveForwardBack, float moveUpDown, float speed, double frameTime)
{
	// Default world
	Vector4 defaultForward = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	Vector4 defaultRight = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 defaultUp = Vector4(0.f, 1.f, 0.f, 0.f);

	// Update the pitch and yaw values for camera
	// Mouse still depends on FPS!
	// If we use deltaTime then Raw Input will be very noticable! (Jagged movement)
	float deltaPitch = deltaY * 0.003f;
	float deltaYaw = deltaX * 0.003f;


	camPitch += deltaPitch;
	camYaw += deltaYaw;

	// Set camera rotation limit (up/down)
	if (camPitch > 89.f * (M_PI / 180.f))
	{
		camPitch = 89.f * (M_PI / 180.f);
	}
	else if (camPitch < -89.f * (M_PI / 180.f))
	{
		camPitch = -89.f * (M_PI / 180.f);
	}

	Matrix camRotationMatrix = Matrix::CreateFromYawPitchRoll(camYaw, camPitch, 0.f);

	// New orientation
	camRight = Vector4::Transform(defaultRight, camRotationMatrix);
	//camUp = Vector4::Transform(defaultUp, camRotationMatrix);
	camUp = defaultUp;
	camForward = Vector4::Transform(defaultForward, camRotationMatrix);


	// Make sure to move with the correct orientation and correct speed (diagonal fix)
	Vector4 moveDirection = moveLeftRight * camRight + moveForwardBack * camForward + moveUpDown * camUp;
	moveDirection.Normalize();

	position += moveDirection * speed * (float)frameTime;

	// Position the target 'infront' of the camera.
	Vector4 camTarget = position + camForward;

	viewMatrix = DirectX::XMMatrixLookAtLH(position, camTarget, camUp);

}
