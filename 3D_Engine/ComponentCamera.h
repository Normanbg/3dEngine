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
	void SetPos(float3 pos);

	void DebugDraw();

	void Save(Config& data) const;
	void Load(Config* data);

	float * GetViewMatrix() const;
	float * GetProjectionMatrix() const;
	Frustum GetFrustum() const;

public:

	Camera* camRes = nullptr;
};

#endif // !__COMPONENTCAMERA_H__
