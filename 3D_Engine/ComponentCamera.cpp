#include "ComponentCamera.h"
#include "GameObject.h"
#include "Camera.h"

ComponentCamera::ComponentCamera()
{
	/*frustum.type = PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitY;
	frustum.up = float3::unitZ;
*/
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);
	Position = float3(0.0f, 40.0f, 60.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	camRes = new Camera();
	//LookAt(Reference);
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
	Reference = Spot;

	Z = (Position-Reference).Normalized();
	X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	CalculateViewMatrix();
}

void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

float * ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix[0][0];
}

float * ComponentCamera::GetViewOpenGLViewMatrix()
{
	float4x4 m;

	m = camRes->frustum.ViewMatrix();
	m.Transpose();

	return &m[0][0];
}
