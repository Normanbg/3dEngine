#ifndef __COMPONENTCANVAS_H__
#define __COMPONENTCANVAS_H__

#include "Component.h"
class ComponentCanvas :
	public Component
{
public:
	ComponentCanvas();
	~ComponentCanvas();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	void SetResolution(float2 _resolution);
	void DrawCrossHair();

public:
	bool editor = false;
	float2 lastResolution = float2::zero;

private:
	bool setWidthHeight = true;
	float2 resolution;
	bool cross = true;
};

#endif // !__COMPONENTCANVAS_H__
