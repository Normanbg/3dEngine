#ifndef __PANELTEXTURES_H__
#define __PANELTEXTURES_H__
#include "UIPanel.h"

#include <vector>
#include <map>
class Resource;

class UIPanelMaterials :	public UIPanel
{
public:
	UIPanelMaterials(const char * name, float positionX, float positionY, float width, float height, bool active);
	~UIPanelMaterials();

	void Draw();
	void DrawTextureChilds(std::vector<Resource*> childs);
	void DrawResourcesChilds(std::map<uuid, Resource* > resources);
};
#endif //__PANELTEXTURES_H__
