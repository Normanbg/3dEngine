#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/Geometry/GeometryAll.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	bool Start();
	bool Update() override;
	void CleanUp()override;

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);
	void MoveTo(const vec Movement);

	float* GetViewMatrix();

public: 
	Frustum camera;
	bool freeCamera = false;

	vec X, Y, Z, Position, Reference;
	float mouseSensitivity = 0.25f;
	float scroolWheelSensitivity = 10.0f;
	float zoomDistance = 20.0f;

private:
	void CalculateViewMatrix();

private:
	bool debug = false;
	vec offset_to_player;
	float4x4 ViewMatrix, ViewMatrixInverse;

};

#endif // !__COMPONENTCAMERA_H__