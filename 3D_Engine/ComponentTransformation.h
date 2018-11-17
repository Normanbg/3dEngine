#ifndef __COMPONENTTRANS_H__
#define __COMPONENTTRANS_H__

#include "Component.h"
#include "Math.h"
struct Transform
{
	float3 position = float3::zero;
	float3 rotEuler = float3::zero;
	float3 scale = float3::one;
	Quat rotationQuat = Quat::identity; // should only be one and have functions to transform it 
};


class ComponentTransformation :	public Component
{
public:
	ComponentTransformation();
	~ComponentTransformation();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	void setGlobalMatrix(float4x4 newGlobalMat);
	void setLocalMatrix(float4x4 newLocalMat);
	void setPos(float3 _newpos);
	void setScale(float3 _newscale);
	void setRotQuat(Quat qNewRot);
	void setRotEuler(float3 _newrot);
	float3 getPos() const;
	float3 getScale() const;
	float3 getEulerRot() const;
	Quat getQuatRot() const;
	float4x4 getGlobalMatrix() const;
	float4x4 getLocalMatrix() const;

	void UpdateLocalMatrixfromTrans();
	void UpdateLocalMatrixfromGlobal();
	void Save(Config& data) const;
	void Load(Config* data);

public:
	Transform transform;
	float4x4 localMatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif // !__COMPONENTTRANS_H__
