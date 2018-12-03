#pragma once
#include "Component.h"
#include "Math.h"

#include "mmgr/mmgr.h"

struct RectTransform
{
	float2 position = float2(0,0);
	float width = 0;
	float height = 0;
};

class ComponentRectTransform :	public Component
{
public:
	ComponentRectTransform();
	~ComponentRectTransform();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	RectTransform rect;
};

