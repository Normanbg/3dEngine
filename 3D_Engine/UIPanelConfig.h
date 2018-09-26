#ifndef __PANELCONFIG_H__
#define __PANELCONFIG_H__

#include "UIPanel.h"

class UIPanelConfig : public UIPanel
{
public:
	UIPanelConfig(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelConfig();

	void Draw();

};
#endif // !__PANELCONFIG_H__
