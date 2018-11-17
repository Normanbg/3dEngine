
#ifndef __PANELASSETS_H__
#define __PANELASSETS_H__

#include "UIPanel.h"

#include <vector>
class GameObject;

class UIPanelAssets : public UIPanel
{
public:
	UIPanelAssets(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelAssets();

	void Draw();

	void DrawChilds(std::vector<GameObject*> childs);

	private:
		std::string currDir =ASSETS_PATH;
};
#endif // !__PANELASSETS_H__
