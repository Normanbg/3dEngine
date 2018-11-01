#include "Camera.h"



Camera::Camera()
{
	SetFOV(60);

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 259.0f;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetWorldMatrix(float3x4::identity);
}


Camera::~Camera()
{
}

void Camera::SetFOV(float _fov)
{
	float ar = frustum.AspectRatio();
	frustum.verticalFov = DEGTORAD * _fov;
	frustum.horizontalFov = math::Atan(ar * math::Tan(frustum.verticalFov / 2)) * 2;
}

void Camera::SetAspectRatio(float new_ar)
{
	frustum.horizontalFov = math::Atan(new_ar * math::Tan(frustum.verticalFov / 2)) * 2;
}

float * Camera::GetProjMatrix()
{
	static float4x4 m;

	m = frustum.ProjectionMatrix().Transposed();
	/*m.Transpose();*/

	return (float*)m.v;

}
