#ifndef __MODULE_EDITOR_CAM_H__
#define __MODULE_EDITOR_CAM_H__

#include "Module.h"
#include "Math.h"
#include "Globals.h"

class GameObject;
class ComponentCamera;

enum FrustumContained {
	IS_OUT = 0,
	IS_IN,
	INTERSECT
};

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera(bool start_enabled = true);
	~ModuleEditorCamera();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	FrustumContained ContainsAaBox(const AABB & refBox) const;

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void MoveTo(const vec3 Movement);
	float* GetViewMatrix() ;
	void FocusToMeshes();

public:

	bool free_camera = false;

	vec3 X, Y, Z, Position, Reference;
	float mouseSensitivity = 0.25f;
	float scroolWheelSensitivity = 10.0f;
	float zoomDistance = 20.0f;

	//---------
	//GameObject* editorCam_G0;
	ComponentCamera* edCamera;

private:

	void CalculateViewMatrix();

private:
	bool debug = false;
	vec3 offset_to_player;
	mat4x4 ViewMatrix, ViewMatrixInverse;

};

#endif //__MODULE_EDITOR_CAM_H__