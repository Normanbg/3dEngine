#include "ComponentCamera.h"
#include "GameObject.h"
#include "Camera.h"

ComponentCamera::ComponentCamera()
{
	camRes = new Camera();
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update()
{
	return false;
}

void ComponentCamera::CleanUp()
{
}

void ComponentCamera::DrawInspector()
{
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	float3 direction = Spot - camRes->frustum.pos;

	float3x3 matrix = float3x3::LookAt(camRes->frustum.front, direction.Normalized(), camRes->frustum.up, float3::unitY);

	camRes->frustum.front = matrix.MulDir(camRes->frustum.front).Normalized();
	camRes->frustum.up = matrix.MulDir(camRes->frustum.up).Normalized();
}

float * ComponentCamera::GetViewMatrix()
{
	static float4x4 m;

	m = camRes->frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

float * ComponentCamera::GetProjectionMatrix()
{
	static float4x4 m;

	m = camRes->frustum.ProjectionMatrix().Transposed();
	return (float*)m.v;

}

float ComponentCamera::GetMouseSensit()
{
	return mouseSensitivity;
}

float ComponentCamera::GetScroolSensit()
{
	return scroolWheelSensitivity;
}
