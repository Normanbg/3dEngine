#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();
	bool debug = false;

public:
	
	bool free_camera = false;

	vec3 X, Y, Z, Position, Reference;
	

private:
	vec3 offset_to_player;
	mat4x4 ViewMatrix, ViewMatrixInverse;
	float Sensitivity = 0.25f;
	float zoomDistance = 20.0f;
};

#endif 