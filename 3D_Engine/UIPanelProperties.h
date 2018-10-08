#ifndef __PANELPROPERTIES_H__
#define __PANELPROPERTIES_H__

#include "UIPanel.h"

class UIPanelProperties : public UIPanel
{
public:
	UIPanelProperties(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelProperties();

	void Draw();

};
#endif // !__PANELPROPERTIES_H__
