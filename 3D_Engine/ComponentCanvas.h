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

public:
	bool editor = false;

private:
	bool setWidthHeight = true;
	float2 resolution;
};

#endif // !__COMPONENTCANVAS_H__
