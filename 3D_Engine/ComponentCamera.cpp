#include "Application.h"
#include "Globals.h"
#include "ModuleEditorCamera.h"
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

void ComponentCamera::DrawInspector() {
	float mouseS = mouseSensitivity;
	float scrollS = scrollWheelSensitivity;
	float fov = camRes->GetFov();
	float ar = camRes->GetAR();
	float nearPl = camRes->frustum.nearPlaneDistance;
	float farPl = camRes->frustum.farPlaneDistance;
	bool projChanged = false;

	if (ImGui::DragFloat("Mouse Speed", &mouseS, 0.01f))
		mouseSensitivity = mouseS;
	if (ImGui::DragFloat("Scroll Speed", &scrollS, 0.1f))
		scrollWheelSensitivity = scrollS;
	if (ImGui::DragFloat("Near Plane", &nearPl, 0.5f)) {
		SetNearPlaneDistance(nearPl);
		projChanged = true;
	}
	if (ImGui::DragFloat("Far Plane", &farPl, 0.5f)) {
		SetFarPlaneDistance(farPl);
		projChanged = true;
	}
	if (ImGui::DragFloat("FOV", &fov, 0.2f)) {
		camRes->SetFOV(fov);
		projChanged = true;
	}
	if (ImGui::DragFloat("Aspect Ratio", &ar, 0.001f)) {
		camRes->SetAspectRatio(ar);
		projChanged = true;
	}
	if (projChanged)
		App->camera->UpdateProjMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	float3 direction = Spot - camRes->frustum.pos;

	float3x3 matrix = float3x3::LookAt(camRes->frustum.front, direction.Normalized(), camRes->frustum.up, float3::unitY);

	camRes->frustum.front = matrix.MulDir(camRes->frustum.front).Normalized();
	camRes->frustum.up = matrix.MulDir(camRes->frustum.up).Normalized();
}

void ComponentCamera::SetNearPlaneDistance(float nearPlaneDist) {
	if (nearPlaneDist > camRes->frustum.farPlaneDistance) {
		OWN_LOG("ERROR: Near plane distance cannot be greater than far plane distance");
	}
	else if (nearPlaneDist < 0.0f) {
		OWN_LOG("ERROR: Near plane distance cannot be smaller than 0.0");
	}
	else {
		camRes->frustum.nearPlaneDistance = nearPlaneDist;
	}
}

void ComponentCamera::SetFarPlaneDistance(float farPlaneDist) {
	if (farPlaneDist < camRes->frustum.nearPlaneDistance) {
		OWN_LOG("ERROR: Far plane distance cannot be smaller than near plane distance");
	}
	else if (farPlaneDist < 0.0f) {
		OWN_LOG("ERROR: Far plane distance cannot be smaller than 0.0");
	}
	else {
		camRes->frustum.farPlaneDistance = farPlaneDist;
	}
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

float ComponentCamera::GetScrollSensit()
{
	return scrollWheelSensitivity;
}
