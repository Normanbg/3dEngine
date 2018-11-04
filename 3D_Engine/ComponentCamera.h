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

	void LookAt(const float3 & Spot);

	void SetNearPlaneDistance(float nearPlaneDist);
	void SetFarPlaneDistance(float farPlaneDist);
	void SetFov(float fov);
	void SetAspectRatio(float aspectRatio);

	//void CalculateViewMatrix();
	float * GetViewMatrix();
	float * GetProjectionMatrix();
	float GetMouseSensit();
	float GetScrollSensit();

public:
	GameObject* myGo;
	Camera* camRes;
	Frustum frustum;

private:

private:
	float mouseSensitivity = 0.25f;
	float scrollWheelSensitivity = 10.0f;

	
};

#endif // !__COMPONENTCAMERA_H__
