#ifndef __COMPONENTTRANS_H__
#define __COMPONENTTRANS_H__

#include "Component.h"
#include "Math.h"
struct Transform
{
	float3 position = float3::zero;
	float3 rotEuler = float3::zero;
	float3 scale = float3::one;
	Quat rotationQuat = Quat::identity; 
};


class ComponentTransformation :	public Component
{
public:
	ComponentTransformation();
	~ComponentTransformation();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	void setGlobalMatrix(const float4x4 newGlobalMat);
	void setLocalMatrix(const float4x4 newLocalMat);
	void SetPos(const float3 _newpos);
	void SetRotQuat(const Quat qNewRot);
	void SetRotEuler(const float3 _newrot);

	void SetScale(float3 _newscale);

	const float3 GetPos() const;
	const float3 GetScale() const;
	const float3 GetEulerRot() const;
	const Quat GetQuatRot() const;
	const float4x4 GetGlobalMatrix() const;
	const float4x4 GetLocalMatrix() const;

	void UpdateLocalMatrix();
	void Save(Config& data) const;
	void Load(Config* data);

public:
	Transform transform;
	float4x4 localMatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif // !__COMPONENTTRANS_H__
