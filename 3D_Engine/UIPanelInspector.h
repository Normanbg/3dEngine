#ifndef __PANELINSPECTOR_H__
#define __PANELINSPECTOR_H__

#include "UIPanel.h"

class GameObject;
enum ComponentType;

class UIPanelInspector : public UIPanel
{
public:
	UIPanelInspector(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelInspector();

	void Draw();

	void DrawComponent(ComponentType compDraw);
};
#endif // !__PANELINSPECTOR_H__
