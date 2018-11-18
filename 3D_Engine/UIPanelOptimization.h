#ifndef __PANELOPTIMIZATION_H__
#define __PANELOPTIMIZATION_H__

#include "UIPanel.h"

class UIPanelOptimization : public UIPanel
{
public:
	UIPanelOptimization(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelOptimization();

	void Draw();

}; 
#endif//!__PANELOPTIMIZATION_H__