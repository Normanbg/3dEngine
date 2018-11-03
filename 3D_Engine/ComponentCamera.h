#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"
#include "Math.h"

class GameObject;
class Camera;

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	void Look(const float3 & Position, const float3 & Reference, bool RotateAroundReference);
	void LookAt(const float3 & Spot);

	void CalculateViewMatrix();
	float * GetViewMatrix();
	float * GetProjectionMatrix();
	float GetMouseSensit();
	float GetScroolSensit();

public:
	GameObject* myGo;
	Camera* camRes;
	Frustum frustum;

	float3 X, Y, Z, Position, Reference;

private:

private:
	float4x4 ViewMatrix, ViewMatrixInverse;
	float mouseSensitivity = 0.25f;
	float scroolWheelSensitivity = 10.0f;

	
};

#endif // !__COMPONENTCAMERA_H__
