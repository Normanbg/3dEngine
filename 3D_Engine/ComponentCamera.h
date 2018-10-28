#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "Math.h"
enum FrustumContained {
	IS_OUT = 0,
	IS_IN, 
	INTERSECT
};
class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	//bool Start();
	bool Update() override;
	void CleanUp()override;
	void DrawInspector()override;

	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	void MoveTo(const float3 Movement);
	void DebugDraw();

	void SetNearPlaneDistance(float nearPlaneDist);
	void SetFarPlaneDistance(float farPlaneDist);
	void SetFov(float fov);
	void SetAspectRatio(float aspectRatio);
	void SetMouseSensitivity(float newMouseSens);
	void SetScroolWheelSensitivity(float newScroolSensitivity);
	void SetDistToReduceZoomVel(float newZoomDistance);

	float GetNearPlaneDistance() const { return camFrustum.nearPlaneDistance; }
	float GetFarPlaneDistance() const { return camFrustum.farPlaneDistance; }
	float* GetViewMatrix();
	float* GetProjMatrix();

public: 
	Frustum camFrustum;

private:
	void CalculateViewMatrix();
	//void FrustumContains(const AaBox& refBox);
private:
	float mouseSensitivity = 0.25f;
	float scroolWheelSensitivity = 10.0f;
	float zoomDistance = 20.0f;

};

#endif // !__COMPONENTCAMERA_H__