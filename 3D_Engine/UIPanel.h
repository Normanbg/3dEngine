#ifndef __UIPANEL_H__
#define __UIPANEL_H__

#include "Globals.h"
#include "Application.h"
#include "Module.h"

#include <string>

class UIPanel
{
public:
	UIPanel(const char* name, float positionX, float positionY, float width, float height, bool active = true): name(name), positionX(positionX), width(width), height(height), active(active){}
	virtual ~UIPanel();

	virtual void Draw(){}

	bool isActive() const { return active; }

public:
	float positionX, positionY, width, height;

protected:
	std::string name;
	bool active = false;
};


#endif // !__UIPANEL_H__
