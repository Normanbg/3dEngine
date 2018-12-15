#ifndef __COMPONENTTEXTUI_H__
#define __COMPONENTTEXTUI_H__
#include "Component.h"
#include "ComponentUI.h"
#include "ComponentWithResource.h"

class ComponentTextUI : public ComponentUI, public Component
{
public:
	ComponentTextUI();
	~ComponentTextUI();

	void DrawInspector() override;

	bool Update() override;
	void CleanUp() override;	
	inline void doUpdate() override { Update(); }
	inline void doCleanUp() override { CleanUp(); }
};



#endif // !__COMPONENTTEXTUI_H__