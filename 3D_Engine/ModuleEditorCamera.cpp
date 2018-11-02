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
#include "ModuleGui.h"
#include "Brofiler/Brofiler.h"


ModuleEditorCamera::ModuleEditorCamera(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	name = "Camera";
	

	Position = vec3(0.0f, 0.0f, 0.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	cameraComp = new ComponentCamera();
	
	//LookAt(Reference);
}

ModuleEditorCamera::~ModuleEditorCamera()
{}

// -----------------------------------------------------------------
bool ModuleEditorCamera::Start()
{
	OWN_LOG("Setting up the camera");
	bool ret = true;

	cameraComp->LookAt(float3(0,0,0));
	cameraComp->camRes->frustum.pos = { -35, 8, 0 };
	return ret;
}

// -----------------------------------------------------------------
bool ModuleEditorCamera::CleanUp()
{
	OWN_LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleEditorCamera::Update(float dt)
{
	BROFILER_CATEGORY("Camera3D_Update", Profiler::Color::Chartreuse);

	////Alt+Left click should orbit the object
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {
		Orbit(dt);
	}
	
	////While Right clicking, WASDRT fps-like movement & free look enabled
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
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
		float mSpeed = cameraComp->GetScroolSensit();
		if (App->input->GetMouseZ() < 0)
			_pos -= cameraComp->camRes->frustum.front * mSpeed;
		else
			_pos += cameraComp->camRes->frustum.front * mSpeed;

		if (!_pos.IsZero())
			cameraComp->camRes->frustum.Translate(_pos);
	}

	cameraComp->CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

//// tests if a AaBox is within the frustrum
//FrustumContained ModuleEditorCamera::ContainsAaBox(const AABB& refBox) const
//{
//	float3 vCorner[8];
//	int iTotalIn = 0;
//	refBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
//
//	// test all 8 corners against the 6 sides
//	// if all points are behind 1 specific plane, we are out
//	// if we are in with all points, then we are fully in
//	for (int p = 0; p < 6; ++p) {
//		int iInCount = 8;
//		int iPtIn = 1;
//		for (int i = 0; i < 8; ++i) {
//			// test this point against the planes
//			if (edCamera->camFrustum.GetPlane(p).IsOnPositiveSide(vCorner[i]))
//			{
//				iPtIn = 0;
//				--iInCount;
//			}
//		}
//		// were all the points outside of plane p?
//		if(iInCount == 0)
//			return(IS_OUT);
//		// check if they were all on the right side of the plane
//		iTotalIn += iPtIn;
//	}
//	// so if iTotalIn is 6, then all are inside the view
//	if (iTotalIn == 6)
//		return(IS_IN);
//	// we must be partly in then otherwise
//	return(INTERSECT);
//}

void ModuleEditorCamera::MouseMovement(float dt)
{
	float dx = -App->input->GetMouseXMotion() * cameraComp->GetMouseSensit() * dt;
	float dy = -App->input->GetMouseYMotion() *cameraComp->GetMouseSensit() * dt;
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
	float dx = -App->input->GetMouseXMotion() * cameraComp->GetMouseSensit() * dt;
	float dy = -App->input->GetMouseYMotion() *cameraComp->GetMouseSensit() * dt;
	float3 distance = cameraComp->camRes->frustum.pos;
	Quat X(cameraComp->camRes->frustum.WorldRight(), dy);
	Quat Y(cameraComp->camRes->frustum.up, dx);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	cameraComp->camRes->frustum.pos = distance;

	cameraComp->LookAt(float3(0, 0, 0));
}








// -----------------------------------------------------------------
void ModuleEditorCamera::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleEditorCamera::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleEditorCamera::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleEditorCamera::MoveTo(const vec3 Movement)
{
	Position = Movement;
	Reference = Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleEditorCamera::GetViewMatrix() 
{
	return &ViewMatrix;
}


void ModuleEditorCamera::FocusToMeshes(){
	vec3 v;// = App->renderer3D->GetAvgPosFromMeshes();

	vec3 displacement = { 10, 10, 10 };
	vec3 moveVec = { v.x*2.5f, v.y*2.0f,v.z*2.5f };
	MoveTo(displacement + moveVec);
	
	LookAt({ 0,0,0 });
}


// -----------------------------------------------------------------
void ModuleEditorCamera::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}