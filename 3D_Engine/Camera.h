#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Globals.h"
#include "Math.h"

class Camera
{
public:
	Camera();
	~Camera();

	void SetFOV(float _fov);
	void SetAspectRatio(float new_ar);
	void SetPos(float3 pos);
	const float3 GetPos() const;
	const float GetFOV() const;
	const float GetAspectRatio() const;

	void DebugDraw();
public:
	Frustum frustum;

private:
	float fov = 0.f;
};

#endif // !__CAMERA_H__
