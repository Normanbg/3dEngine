#ifndef __PANELCAMERA_H__
#define __PANELCAMERA_H__

#include "UIPanel.h"

class UIPanelCamera : public UIPanel
{
public:
	UIPanelCamera(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelCamera();

	void Draw();

};
#endif // !__PANELCAMERA_H__
