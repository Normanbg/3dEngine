#ifndef __PANELINSPECTOR_H__
#define __PANELINSPECTOR_H__

#include "UIPanel.h"

class GameObject;
class Component;
class ComponentUI;

class UIPanelInspector : public UIPanel
{
public:
	UIPanelInspector(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelInspector();

	void Draw();

	void DrawComponent(Component* compDraw);
	void DrawUIComponent(ComponentUI * compDraw);
};
#endif // !__PANELINSPECTOR_H__
