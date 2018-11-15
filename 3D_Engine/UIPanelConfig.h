#ifndef __PANELCONFIG_H__
#define __PANELCONFIG_H__

#include "UIPanel.h"

#define MEMORY_UPDATE_READING 10
#define MAX_MEMORY_LOG 100

class UIPanelConfig : public UIPanel
{
public:
	UIPanelConfig(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelConfig();

	void Draw();
private:

	uint updates;
	std::vector<float> memory;
};
#endif // !__PANELCONFIG_H__
