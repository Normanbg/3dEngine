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
	if (myGO)
	{
		SetPos(myGO->transformComp->getPos());
	}
	return true;
}

void ComponentCamera::CleanUp()
{
}

void ComponentCamera::DrawInspector() {
	float fov = camRes->GetFOV();
	float ar = camRes->GetAspectRatio();
	float nearPl = camRes->frustum.nearPlaneDistance;
	float farPl = camRes->frustum.farPlaneDistance;
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	if (ImGui::DragFloat("Near Plane", &nearPl, 0.5f)) {
		SetNearPlaneDistance(nearPl);
	}
	if (ImGui::DragFloat("Far Plane", &farPl, 0.5f)) {
		SetFarPlaneDistance(farPl);
	}
	if (ImGui::SliderFloat("FOV", &fov, 30.f, 175.f)) {
		camRes->SetFOV(fov);
	}
	if (ImGui::SliderFloat("Aspect Ratio", &ar, 0.1f, 3.5f)) {
		camRes->SetAspectRatio(ar);
	}
	ImGui::Separator();
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

void ComponentCamera::SetPos(float3 pos)
{
	camRes->SetPos(pos);
}

void ComponentCamera::DebugDraw(){
	if (camRes != nullptr)
		camRes->DebugDraw();
}

float * ComponentCamera::GetViewMatrix() const
{
	static float4x4 m;

	m = camRes->frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

float * ComponentCamera::GetProjectionMatrix() const
{
	static float4x4 m;

	m = camRes->frustum.ProjectionMatrix().Transposed();
	return (float*)m.v;

}

Frustum ComponentCamera::GetFrustum() const
{
	return camRes->frustum;
}

void ComponentCamera::Save(Config & data) const
{
	data.AddUInt("UUID", uuid);
	if (camRes) {
		data.AddFloat3("Frustum Position", camRes->GetPos());
		data.AddFloat("FOV", camRes->GetFOV());
		data.AddFloat("Aspect Ratio", camRes->GetAspectRatio());
	}
}

void ComponentCamera::Load(Config * data)
{
	uuid = data->GetUInt("UUID");
	if (camRes==nullptr) {
		camRes = new Camera();		
	}
	camRes->SetPos(data->GetFloat3("Frustum Position", { 0,0,0 }));
	camRes->SetFOV(data->GetFloat("FOV", 0));
	camRes->SetAspectRatio(data->GetFloat("Aspect Ratio", 0));
}
