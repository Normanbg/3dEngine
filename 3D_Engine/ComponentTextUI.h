#ifndef __COMPONENTTEXTUI_H__
#define __COMPONENTTEXTUI_H__
#include "Component.h"
#include "ComponentUI.h"
#include "ComponentWithResource.h"

class ComponentTextUI : public ComponentUI
{
public:
	ComponentTextUI();
	~ComponentTextUI();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;
};



#endif // !__COMPONENTTEXTUI_H__