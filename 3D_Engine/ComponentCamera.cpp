#include "ComponentCamera.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"



ComponentCamera::ComponentCamera()
{
	camFrustum.type = FrustumType::PerspectiveFrustum;
	camFrustum.pos = float3::zero;
	camFrustum.front = float3::unitZ;
	camFrustum.up = float3::unitY;

	camFrustum.nearPlaneDistance = 1.0f;
	camFrustum.farPlaneDistance = 600.0f;
	camFrustum.verticalFov = DEGTORAD * 60.0f;
}

ComponentCamera::~ComponentCamera()
{
}


bool ComponentCamera::Update()
{
	//float3 newPos(0, 0, 0);
	//float speed = CAMERA_SPEED;

	////Alt+Left click should orbit the object
	//if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	mouseSensitivity = 0.25f;
	//	Reference = { 0,0,0 };//WILL BE THE POSITION WHEN WE HAVE IT!!! 
	//	if (dx != 0)
	//	{
	//		float DeltaX = (float)dx * mouseSensitivity;
	//		
	//		X = VecFunctions::rotatef3(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//		Y = VecFunctions::rotatef3(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//		Z = VecFunctions::rotatef3(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//	}

	//	if (dy != 0)
	//	{
	//		float DeltaY = (float)dy * mouseSensitivity;

	//		Y = VecFunctions::rotatef3(Y, DeltaY, X);
	//		Z = VecFunctions::rotatef3(Z, DeltaY, X);

	//		if (Y.y < 0.0f)
	//		{
	//			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = Z.Cross(X);
	//		}
	//	}

	//	Position = Reference + Z * Position.Length();
	//}
	////While Right clicking, “WASD” fps-like movement While Right clicking, “WASD” fps-like movement and free look enabled
	//if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	speed = CAMERA_SPEED;

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

	//	float3 newPosition = Position - Reference;

	//	if (dx != 0)
	//	{
	//		const float DeltaX = (float)dx * mouseSensitivity;

	//		X = VecFunctions::rotatef3(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//		Y = VecFunctions::rotatef3(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//		Z = VecFunctions::rotatef3(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//	}

	//	if (dy != 0)
	//	{
	//		const float DeltaY = (float)dy * mouseSensitivity;

	//		Y = VecFunctions::rotatef3(Y, DeltaY, X);
	//		Z = VecFunctions::rotatef3(Z, DeltaY, X);

	//		if (Y.y < 0.0f)
	//		{
	//			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = Z.Cross(X);
	//		}
	//	}

	//	Reference = Position - Z * newPosition.Length();
	//}

 //   //FOCUS TO MESHES--------------------------------------------------MISSING

	////-----Zoom
	//if (App->input->GetMouseZ() != 0) {
	//	newPos = float3::zero;
	//	float wheelSensitivity = scroolWheelSensitivity;
	//	float3 distance = Reference - Position;

	//	if (distance.Length() < zoomDistance)
	//		wheelSensitivity = distance.Length() / zoomDistance;
	//	if (App->input->GetMouseZ() > 0)
	//		newPos -= Z * wheelSensitivity;
	//	else
	//		newPos += Z * wheelSensitivity;

	//	Position += newPos;
	//}


	//// Recalculate matrix -------------
	//CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ComponentCamera::CleanUp()
{
}

void ComponentCamera::LookAt(const float3 & Spot){
	float3 dir = Spot - camFrustum.pos;

	float3x3 matrix = float3x3::LookAt(camFrustum.front, dir.Normalized(), camFrustum.up, float3::unitY);

	camFrustum.front = matrix.MulDir(camFrustum.front).Normalized();
	camFrustum.up = matrix.MulDir(camFrustum.up).Normalized();
}

void ComponentCamera::SetNearPlaneDistance(float nearPlaneDist){
	if (nearPlaneDist > camFrustum.farPlaneDistance) {
		OWN_LOG("ERROR: Near plane distance cannot be greater than far plane distance");
	}
	else if (nearPlaneDist < 0.0f) {
		OWN_LOG("ERROR: Near plane distance cannot be smaller than 0.0");
	}
	else {
		camFrustum.nearPlaneDistance = nearPlaneDist;
	}
}

void ComponentCamera::SetFarPlaneDistance(float farPlaneDist){
	if (farPlaneDist < camFrustum.nearPlaneDistance) {
		OWN_LOG("ERROR: Far plane distance cannot be smaller than near plane distance");
	}
	else if (farPlaneDist < 0.0f) {
		OWN_LOG("ERROR: Far plane distance cannot be smaller than 0.0");
	}
	else {
		camFrustum.nearPlaneDistance = farPlaneDist;
	}
}

void ComponentCamera::SetFov(float fov){
	float newAR = camFrustum.AspectRatio();
	camFrustum.verticalFov = DEGTORAD * fov;
	SetAspectRatio(newAR);
}

void ComponentCamera::SetAspectRatio(float aspectRatio){
	camFrustum.horizontalFov = 2.0f * atanf(tanf(camFrustum.verticalFov * 0.5f) * aspectRatio);
}

