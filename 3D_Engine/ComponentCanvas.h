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

public:
	float2 resolution;
	bool editor = false;
private:
	bool setWidthHeight = false;
};

#endif // !__COMPONENTCANVAS_H__
