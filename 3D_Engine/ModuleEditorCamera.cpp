#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include "Camera.h"
#include "ComponentCamera.h"
#include "Quadtree.h"
#include "ModuleGui.h"
#include "Brofiler/Brofiler.h"

#include "mmgr/mmgr.h"


ModuleEditorCamera::ModuleEditorCamera(bool start_enabled) : Module(start_enabled)
{

	name = "Camera";
}

ModuleEditorCamera::~ModuleEditorCamera()
{}

bool ModuleEditorCamera::Init(JSON_Object* data) {
	
	cameraComp = new ComponentCamera();
	return true;
}

// -----------------------------------------------------------------
bool ModuleEditorCamera::Start()
{
	OWN_LOG("Setting up the camera");
	bool ret = true;
	cameraComp->SetPos({ -35, 8, 0 });
	cameraComp->SetFOV(60);
	cameraComp->SetAspectRatio(1.f);
	cameraComp->LookAt(float3(0,0,0));	
	return ret;
}

// -----------------------------------------------------------------
bool ModuleEditorCamera::CleanUp()
{
	OWN_LOG("Cleaning camera");
	cameraComp->CleanUp();
	RELEASE(cameraComp);
	return true;
}

// -----------------------------------------------------------------
update_status ModuleEditorCamera::Update(float dt)
{
	BROFILER_CATEGORY("Camera3D_Update", Profiler::Color::Chartreuse);

	////Alt+Left click should orbit the object
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {
		Orbit(dt);
	}
	
	////While Right clicking, WASDRT fps-like movement & free look enabled
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !App->input->GetKey(SDL_SCANCODE_LALT)) {
		float3 _pos(0, 0, 0);
		float speed = CAMERA_SPEED * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 5.0f;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) _pos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) _pos.y -= speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) _pos += cameraComp->camRes->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) _pos -= cameraComp->camRes->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) _pos -= cameraComp->camRes->frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) _pos += cameraComp->camRes->frustum.WorldRight() * speed;

		if (!_pos.IsZero())
			cameraComp->camRes->frustum.Translate(_pos);	

		MouseMovement(dt);
	}

	//Zoom
	if (App->input->GetMouseZ() != 0) {
		float3 _pos(0, 0, 0);
		float mSpeed = GetScrollSensit();
		if (App->input->GetMouseZ() < 0)
			_pos -= cameraComp->camRes->frustum.front * mSpeed;
		else
			_pos += cameraComp->camRes->frustum.front * mSpeed;

		if (!_pos.IsZero())
			cameraComp->camRes->frustum.Translate(_pos);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		float3 dir = cameraComp->camRes->frustum.pos;
		float dist = 10.f;
		float3 center = float3::zero;

		if (App->scene->gObjSelected && App->scene->gObjSelected->GetComponentMesh()) {
			GameObject* GO = App->scene->gObjSelected;
			center = GO->globalAABB.CenterPoint();
			dir -= center;
			dist = GO->globalAABB.Size().Length();
		}
		cameraComp->SetPos(dir.Normalized() * dist);
		cameraComp->LookAt(center);
	}

	return UPDATE_CONTINUE;
}

// tests if a AaBox is within the frustrum
FrustumContained ModuleEditorCamera::ContainsAaBox(const AABB& refBox) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	refBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array

	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (cameraComp->GetFrustum().GetPlane(p).IsOnPositiveSide(vCorner[i]))
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return(IS_OUT);
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return(IS_IN);
	// we must be partly in then otherwise
	return(INTERSECT);
}

FrustumContained ModuleEditorCamera::ContainsAaBox(const Quadtree& qt) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	qt.quadTreeBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array

	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (cameraComp->GetFrustum().GetPlane(p).IsOnPositiveSide(vCorner[i]))
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
			return(IS_OUT);
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return(IS_IN);
	// we must be partly in then otherwise
	return(INTERSECT);
}

void ModuleEditorCamera::ConfigInfo(){
	float mouseS = GetMouseSensit();
	float scrollS = GetScrollSensit();
	float fov = cameraComp->camRes->GetFOV();
	float ar = cameraComp->camRes->GetAspectRatio();
	float nearPl = cameraComp->camRes->frustum.nearPlaneDistance;
	float farPl = cameraComp->camRes->frustum.farPlaneDistance;

	if (ImGui::DragFloat("Mouse Speed", &mouseS, 0.01f))
		mouseSensitivity = mouseS;
	if (ImGui::DragFloat("Scroll Speed", &scrollS, 0.1f))
		scrollWheelSensitivity = scrollS;
	if (ImGui::DragFloat("Near Plane", &nearPl, 0.5f)) {
		cameraComp->SetNearPlaneDistance(nearPl);
	}
	if (ImGui::DragFloat("Far Plane", &farPl, 0.5f)) {
		cameraComp->SetFarPlaneDistance(farPl);
	}
	if (ImGui::SliderFloat("FOV", &fov, 30.f, 175.f)) {
		cameraComp->camRes->SetFOV(fov);	
	}
	if (ImGui::SliderFloat("Aspect Ratio", &ar, 0.1f, 3.5f)) {
		cameraComp->camRes->SetAspectRatio(ar);
	}
	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::PushStyleColor(ImGuiCol_Header, { DarkCyan.r, DarkCyan.g,DarkCyan.b, DarkCyan.a });
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { Cyan.r, Cyan.g,Cyan.b, Cyan.a });
	if (ImGui::CollapsingHeader("Tools")) {
		ImGui::Checkbox("Frustum Culling", &frustumCulling);
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}


const float ModuleEditorCamera::GetMouseSensit() const
{
	return mouseSensitivity;
}

const float ModuleEditorCamera::GetScrollSensit() const
{
	return scrollWheelSensitivity;
}

const bool ModuleEditorCamera::GetFrustumCulling() const
{
	return frustumCulling;
}


void ModuleEditorCamera::MouseMovement(float dt)
{
	float dx = -App->input->GetMouseXMotion() * GetMouseSensit() * dt;
	float dy = -App->input->GetMouseYMotion() * GetMouseSensit() * dt;
	if (dx != 0)
	{
		Quat rotationX = Quat::RotateY(dx);
		cameraComp->camRes->frustum.front = rotationX.Mul(cameraComp->camRes->frustum.front).Normalized();
		cameraComp->camRes->frustum.up = rotationX.Mul(cameraComp->camRes->frustum.up).Normalized();
	}
	if (dy != 0)
	{
		Quat rotationY = Quat::RotateAxisAngle(cameraComp->camRes->frustum.WorldRight(), dy);
		float3 _up = rotationY.Mul(cameraComp->camRes->frustum.up).Normalized();
		if (_up.y > 0.0f)
		{
			cameraComp->camRes->frustum.up = _up;
			cameraComp->camRes->frustum.front = rotationY.Mul(cameraComp->camRes->frustum.front).Normalized();
		}
	}
}

void ModuleEditorCamera::Orbit(float dt)
{
	float dx = -App->input->GetMouseXMotion() * GetMouseSensit() * dt;
	float dy = -App->input->GetMouseYMotion() * GetMouseSensit() * dt;
	float3 distance = cameraComp->camRes->frustum.pos;
	Quat X(cameraComp->camRes->frustum.WorldRight(), dy);
	Quat Y(cameraComp->camRes->frustum.up, dx);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	cameraComp->camRes->frustum.pos = distance;

	cameraComp->LookAt(float3(0, 0, 0));
}
