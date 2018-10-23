#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "Math.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	bool Start();
	bool Update() override;
	void CleanUp()override;

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	void MoveTo(const float3 Movement);

public: 
	Frustum camFrustum;

	float3 X, Y, Z, Position, Reference;
	float mouseSensitivity = 0.25f;
	float scroolWheelSensitivity = 10.0f;
	float zoomDistance = 20.0f;


private:
	void CalculateViewMatrix();

private:
	float4x4 ViewMatrix, ViewMatrixInverse;

};

#endif // !__COMPONENTCAMERA_H__