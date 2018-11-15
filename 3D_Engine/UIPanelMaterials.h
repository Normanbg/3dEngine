#ifndef __PANELTEXTURES_H__
#define __PANELTEXTURES_H__
#include "UIPanel.h"

#include <vector>
#include <map>
class Material;
class Resource;

class UIPanelMaterials :	public UIPanel
{
public:
	UIPanelMaterials(const char * name, float positionX, float positionY, float width, float height, bool active);
	~UIPanelMaterials();

	void Draw();
	void DrawChilds(std::vector<Material*> childs);
	void DrawChildsRes(std::map<uuid, Resource* > resources);
};
#endif //__PANELTEXTURES_H__
