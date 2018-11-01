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

	float* GetProjMatrix();

public:
	Frustum frustum;

};

#endif // !__CAMERA_H__
