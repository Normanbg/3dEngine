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
	
	LookAt(Reference);
}

ModuleEditorCamera::~ModuleEditorCamera()
{}

// -----------------------------------------------------------------
bool ModuleEditorCamera::Start()
{
	OWN_LOG("Setting up the camera");
	bool ret = true;

	cameraComp->LookAt({ 0,0,0 });
	cameraComp->camRes->frustum.pos = { 0, 4, 20 };
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
	float3 _pos(0, 0, 0);
	float speed = CAMERA_SPEED * dt;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) _pos += cameraComp->frustum.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) _pos -= cameraComp->frustum.front * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		_pos -= cameraComp->frustum.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) _pos += cameraComp->frustum.WorldRight() * speed;

	if (!_pos.IsZero())
		cameraComp->frustum.Translate(_pos);

	BROFILER_CATEGORY("Camera3D_Update", Profiler::Color::Chartreuse);
	//vec3 newPos(0, 0, 0);
	//float speed = CAMERA_SPEED * dt;

	////Alt+Left click should orbit the object
	//if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	mouseSensitivity = 0.25f;
	//	Reference = { 0,0,0 };//WILL BE THE POSITION WHEN WE HAVE IT!!! 
	//	if (dx != 0)
	//	{
	//		float DeltaX = (float)dx * mouseSensitivity;

	//		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//	}

	//	if (dy != 0)
	//	{
	//		float DeltaY = (float)dy * mouseSensitivity;

	//		Y = rotate(Y, DeltaY, X);
	//		Z = rotate(Z, DeltaY, X);

	//		if (Y.y < 0.0f)
	//		{
	//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = cross(Z, X);
	//		}
	//	}

	//	Position = Reference + Z * length(Position);
	//}
	////While Right clicking, “WASD” fps-like movement While Right clicking, “WASD” fps-like movement and free look enabled
	//if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	speed = CAMERA_SPEED * dt;

	//	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//		speed *= 2.0f;

	//	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	//	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) newPos.y -= speed;

	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//	Position += newPos;
	//	Reference += newPos;

	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	mouseSensitivity = 0.25f;

	//	vec3 newPosition = Position - Reference;

	//	if (dx != 0)
	//	{
	//		const float DeltaX = (float)dx * mouseSensitivity;

	//		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//	}

	//	if (dy != 0)
	//	{
	//		const float DeltaY = (float)dy * mouseSensitivity;

	//		Y = rotate(Y, DeltaY, X);
	//		Z = rotate(Z, DeltaY, X);

	//		if (Y.y < 0.0f)
	//		{
	//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = cross(Z, X);
	//		}
	//	}

	//	Reference = Position - Z * length(newPosition);
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F)== KEY_REPEAT){		
	//	FocusToMeshes();		
	//}

	////-----Zoom
	//if (App->input->GetMouseZ() != 0) {
	//	newPos = (0, 0, 0);
	//	float wheelSensitivity = scroolWheelSensitivity;
	//	vec3 distance = Reference - Position;

	//	if (length(distance) < zoomDistance)
	//		wheelSensitivity = length(distance) / zoomDistance;
	//	if (App->input->GetMouseZ() > 0)
	//		newPos -= Z * wheelSensitivity;
	//	else
	//		newPos += Z * wheelSensitivity;

	//	Position += newPos;
	//}

	//
	//// Recalculate matrix -------------
	//CalculateViewMatrix();

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