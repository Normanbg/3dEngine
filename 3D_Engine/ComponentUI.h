#ifndef __COMPONENT_UI_H__
#define __COMPONENT_UI_H__


#include "Component.h"

class ComponentRectTransform;

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

	virtual void UpdateRectTransform() {};


	virtual void FadeIn() {};
	virtual void FadeOut() {};
	bool fadingIn = false;
	bool fadingOut = false;

	void SetAlpha(float _alpha) { alpha = _alpha; }
	float alpha=1.0f;

	ComponentTypeUI typeUI = NOTYPE;
	bool draw = true;
	ComponentRectTransform* rectTransform = nullptr;


	virtual void ReceiveEvent(const Event &event) {};

};


#endif //!__COMPONENT_UI_H__