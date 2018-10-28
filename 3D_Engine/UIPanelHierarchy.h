
#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "UIPanel.h"

#include <vector>
class GameObject;

class UIPanelHierarchy : public UIPanel
{
public:
	UIPanelHierarchy(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelHierarchy();

	void Draw();

	void DrawChilds(std::vector<GameObject*> childs);
};
#endif // !__PANELHIERARCHY_H__
