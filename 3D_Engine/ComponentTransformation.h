#ifndef __COMPONENTTRANS_H__
#define __COMPONENTTRANS_H__

#include "Component.h"
#include "Math.h"
struct Transform
{
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	void setPos(float3 _newpos);
	void setScale(float3 _newscale);
	void setRot(Quat qNewRot);
	void setRot(float3 _newrot);
};

class ComponentTransformation :	public Component
{
public:
	ComponentTransformation();
	~ComponentTransformation();

	bool Update() override;


	void CleanUp() override;
	void Test(float3 pos);

public:
	Transform transform;
	float4x4 matrixLocal, matrixGlobal;
};

#endif // !__COMPONENTTRANS_H__
