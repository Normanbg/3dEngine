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

	float * GetViewMatrix();
	float * GetProjectionMatrix();

public:
	GameObject* myGo;
	Camera* camRes;
};

#endif // !__COMPONENTCAMERA_H__
