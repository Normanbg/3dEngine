#ifndef __COMPONENTTRANS_H__
#define __COMPONENTTRANS_H__

#include "Component.h"
#include "MathGeoLib/Geometry/GeometryAll.h"
#include "MathGeoLib/Math/Quat.h"
class ComponentTransformation :	public Component
{
public:
	ComponentTransformation();
	~ComponentTransformation();

	float3 position;
	float3 scale;
	Quat rotation;

	bool Update() override;

	void CleanUp() override;
};

#endif // !__COMPONENTTRANS_H__
