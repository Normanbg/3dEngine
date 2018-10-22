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

	bool Update() override;
	void CleanUp()override;

	void LookAt();

public: 
	Frustum camera;
	vec X, Y, Z, Position, Reference;
};

#endif // !__COMPONENTCAMERA_H__