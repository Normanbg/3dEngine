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

	void SetNearPlaneDistance(const float nearPlaneDist);
	void SetFarPlaneDistance(const float farPlaneDist);
	void SetPos(const float3 pos);
	void SetFOV(const float _fov);
	void SetAspectRatio(const float new_ar);

	void DebugDraw();

	void Save(Config& data) const;
	void Load(Config* data);

	const float * GetViewMatrix() const;
	const float * GetProjectionMatrix() const;
	const Frustum GetFrustum() const;

public:

	Camera* camRes = nullptr;
};

#endif // !__COMPONENTCAMERA_H__
