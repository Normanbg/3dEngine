#include "Application.h"
#include "Globals.h"
#include "ModuleEditorCamera.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "Camera.h"
#include "mmgr/mmgr.h"

ComponentCamera::ComponentCamera()
{
	camRes = new Camera();
	type = CAMERA;
}

ComponentCamera::~ComponentCamera()
{
	
}

bool ComponentCamera::Update()
{
	if (myGO)
	{
		camRes->frustum.SetWorldMatrix(myGO->transformComp->globalMatrix.Float3x4Part());
		if (myGO->GetSelected())
			camRes->DebugDraw();
	}
	return true;
}

void ComponentCamera::CleanUp()
{
	myGO = nullptr;
	RELEASE(camRes);
}

void ComponentCamera::DrawInspector() {
	float fov = GetFOV();
	float ar = GetAspectRatio();
	float nearPl = GetNearPlane();
	float farPl = GetFarPlane();
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	if (ImGui::DragFloat("Near Plane", &nearPl, 0.5f)) {
		SetNearPlaneDistance(nearPl);
	}
	if (ImGui::DragFloat("Far Plane", &farPl, 0.5f)) {
		SetFarPlaneDistance(farPl);
	}
	if (ImGui::SliderFloat("FOV", &fov, 30.f, 175.f)) {
		SetFOV(fov);
	}
	if (ImGui::SliderFloat("Aspect Ratio", &ar, 0.1f, 3.5f)) {
		SetAspectRatio(ar);
	}
	ImGui::Separator();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	float3 direction = Spot - GetPos();

	float3x3 matrix = float3x3::LookAt(GetFrustum().front, direction.Normalized(), GetFrustum().up, float3::unitY);

	camRes->frustum.front = matrix.MulDir(GetFrustum().front).Normalized();
	camRes->frustum.up = matrix.MulDir(GetFrustum().up).Normalized();
}

void ComponentCamera::SetNearPlaneDistance(const float nearPlaneDist) {
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

void ComponentCamera::SetFarPlaneDistance(const float farPlaneDist) {
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

void ComponentCamera::SetPos(const float3 pos)
{
	camRes->SetPos(pos);
}

void ComponentCamera::SetFOV(const float _fov)
{
	camRes->SetFOV(_fov);
}

void ComponentCamera::SetAspectRatio(const float new_ar)
{
	camRes->SetAspectRatio(new_ar);
}

void ComponentCamera::SetFrustumUp(const float3 up)
{
	camRes->frustum.up = up;
}

void ComponentCamera::SetFrustumFront(const float3 front)
{
	camRes->frustum.front = front;
}

void ComponentCamera::DebugDraw(){
	if (camRes != nullptr)
		camRes->DebugDraw();
}

const float * ComponentCamera::GetViewMatrix() const
{
	static float4x4 m;

	m = camRes->frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

const float * ComponentCamera::GetProjectionMatrix() const
{
	static float4x4 m;

	m = camRes->frustum.ProjectionMatrix().Transposed();
	return (float*)m.v;

}

Frustum ComponentCamera::GetFrustum() const
{
	return camRes->frustum;
}

const float3 ComponentCamera::GetPos() const
{
	return GetFrustum().pos;
}

const float ComponentCamera::GetFOV() const
{
	return camRes->GetFOV();
}

const float ComponentCamera::GetAspectRatio() const
{
	return camRes->GetAspectRatio();
}

const float ComponentCamera::GetNearPlane() const
{
	return GetFrustum().nearPlaneDistance;
}

const float ComponentCamera::GetFarPlane() const
{
	return GetFrustum().farPlaneDistance;
}

void ComponentCamera::Translate(const float3 trans) {
	camRes->frustum.Translate(trans);
}

void ComponentCamera::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	if (camRes) {
		data.AddFloat3("Frustum Position", GetPos());
		data.AddFloat("FOV", GetFOV());
		data.AddFloat("Aspect Ratio", GetAspectRatio());
		data.AddFloat("Near Plane", GetNearPlane());
		data.AddFloat("Far Plane", GetFarPlane());
	}
}

void ComponentCamera::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	if (camRes==nullptr) {
		camRes = new Camera();		
	}
	SetPos(data->GetFloat3("Frustum Position", { 0,0,0 }));
	SetFOV(data->GetFloat("FOV", 0));
	SetAspectRatio(data->GetFloat("Aspect Ratio", 0));
	SetNearPlaneDistance(data->GetFloat("Near Plane", 0));
	SetFarPlaneDistance(data->GetFloat("Far Plane", 0));
}
