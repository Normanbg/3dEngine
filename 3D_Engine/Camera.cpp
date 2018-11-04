#include "Camera.h"

Camera::Camera()
{
	fov = 60;
	SetFOV(fov);

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 400.f;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetWorldMatrix(float3x4::identity);
}


Camera::~Camera()
{
}

void Camera::SetFOV(float _fov) {
	fov = _fov;
	float newAR = frustum.AspectRatio();
	frustum.verticalFov = DEGTORAD * _fov;
	SetAspectRatio(newAR);
}

void Camera::SetAspectRatio(float aspectRatio) {
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio);
}

float Camera::GetFov()
{
	return fov;
}

float Camera::GetAR()
{
	return frustum.horizontalFov;
}
