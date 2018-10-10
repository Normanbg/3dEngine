#ifndef __PANELCONSOLE_H__
#define __PANELCONSOLE_H__

#include "UIPanel.h"

class UIPanelConsole : public UIPanel
{
public:
	UIPanelConsole(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelConsole();

	void Draw();

};
#endif // !__PANELCONSOLE_H__
