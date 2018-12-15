#ifndef __COMPONENT_UI_H__
#define __COMPONENT_UI_H__

#include "Component.h"

enum ComponentTypeUI {
	NOTYPE = 0,
	TRANSFORMRECT,
	UI_IMAGE,
	UI_WINDOW,
	UI_BUTTON,
	UI_TEXT,
	UI_INPUT,
	UI_CHECKBOX,
};

class ComponentUI  : public Component
{
public:
	ComponentUI() { type = NO_TYPE; };
	~ComponentUI() {};

	virtual void Draw() {};
	ComponentTypeUI typeUI = NOTYPE;
};


#endif //!__COMPONENT_UI_H__