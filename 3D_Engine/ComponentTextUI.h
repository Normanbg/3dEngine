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

	void Load(Config* data) override;
	void Save(Config& data) const override;
	inline void doLoad(Config* data) override { Load(data); }
	inline void doSave(Config& data)const  override { Save(data); }
};



#endif // !__COMPONENTTEXTUI_H__