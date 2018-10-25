
#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "UIPanel.h"

class UIPanelHierarchy : public UIPanel
{
public:
	UIPanelHierarchy(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelHierarchy();

	void Draw();

};
#endif // !__PANELHIERARCHY_H__
