#ifndef __COMPONENTRECTTRANS_H__
#define __COMPONENTRECTTRANS_H__

#include "Component.h"
#include "ComponentUI.h"


class ComponentRectTransform :	public ComponentUI
{
	struct RectTransform
	{
		float2 position = float2(0, 0);
		float width = 0;
		float height = 0;
		float2 anchor = float2(0,0);
		//float4x4 localMatrix = float4x4::identity;
		float4x4 globalMatrix = float4x4::identity;
		float3* vertex = nullptr;		
		uint vertexID;
	};

public:
	ComponentRectTransform();
	~ComponentRectTransform();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	const float2 GetPos() const { return rect.position; }
	const float GetWidth() const { return rect.width; }
	const float GetHeight() const { return rect.height; }
	const uint GetVertexID() const { return rect.vertexID; }
	const float4x4 GetGlobalMatrix() const { return rect.globalMatrix; }

	void SetPos(float2 pos);
	void SetWidth(float w);
	void SetHeight(float h);

	void Draw() override;
	void GenBuffer();

	
	bool draw = true;
	

private:
	
	void UpdateGlobalMatrix();

private:
	RectTransform rect;
};

#endif // !__COMPONENTRECTTRANS_H__