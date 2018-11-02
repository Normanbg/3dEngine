#include "ComponentCamera.h"
#include "GameObject.h"
#include "Camera.h"

ComponentCamera::ComponentCamera()
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);
	Position = float3(0.0f, 40.0f, 60.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

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

void ComponentCamera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	float3 direction = Spot - camRes->frustum.pos;

	float3x3 matrix = float3x3::LookAt(camRes->frustum.front, direction.Normalized(), camRes->frustum.up, float3::unitY);

	camRes->frustum.front = matrix.MulDir(camRes->frustum.front).Normalized();
	camRes->frustum.up = matrix.MulDir(camRes->frustum.up).Normalized();
}

void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
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
