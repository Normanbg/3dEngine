#ifndef __COMPONENTRECTTRANS_H__
#define __COMPONENTRECTTRANS_H__

#include "Component.h"




class ComponentRectTransform :	public Component
{
	struct RectTransform
	{
		float2 position = float2(0, 0);
		float width = 0;
		float height = 0;
		float3* vertex;		
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

	void SetPos(float2 pos);
	void SetWidth(float w);
	void SetHeight(float h);

	void Draw();
	void GenBuffer();

private:
	RectTransform rect;
};

#endif // !__COMPONENTRECTTRANS_H__