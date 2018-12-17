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



class ComponentUI  
{

public:
	ComponentUI() { };
	~ComponentUI() {};	

	virtual void DrawUI() {};
	virtual void doStart() {}
	virtual void doUpdate() {};
	virtual void doCleanUp() {};

	virtual void doSave(Config& data)const  {};
	virtual void doLoad(Config* data){};
	ComponentTypeUI typeUI = NOTYPE;

};


#endif //!__COMPONENT_UI_H__