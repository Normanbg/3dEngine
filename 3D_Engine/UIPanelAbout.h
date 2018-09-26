#ifndef __PANELABOUT_H__
#define __PANELABOUT_H__

#include "UIPanel.h"

class UIPanelAbout :  public UIPanel
{
public:
	UIPanelAbout(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelAbout();

	void Draw();

};
#endif // !__PANELABOUT_H__
