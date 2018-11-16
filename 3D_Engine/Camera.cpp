#include "Camera.h"

#include "mmgr/mmgr.h"

Camera::Camera()
{
	SetFOV(60);

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

void Camera::SetPos(float3 pos){
	frustum.pos = pos;
}

const float3 Camera::GetPos() const 
{
	return frustum.pos;
}

const float Camera::GetFOV()const
{
	return fov;
}

const float Camera::GetAspectRatio() const
{
	return frustum.horizontalFov;
}

void Camera::DebugDraw(){
	float3 corners[8];
	frustum.GetCornerPoints(corners);
	DebugDrawBox(corners, Yellow, 5.f);

}
