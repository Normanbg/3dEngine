#ifndef __COMPONENTINPUTUI_H__
#define __COMPONENTINPUTUI_H__

#include "Component.h"
#include "ComponentUI.h"
class ComponentInputUI : public ComponentUI, public Component
{
public:
	ComponentInputUI();
	~ComponentInputUI();

	void UpdateRectTransform() override;
	void DrawInspector() override;
	void DrawUI() override;

	bool Start() override;
	bool Update() override;
	void CleanUp() override;

	inline void doStart() override { Start(); }
	inline void doUpdate() override { Update(); }
	inline void doCleanUp() override { CleanUp(); }

	void Load(Config* data) override;
	void Save(Config& data) const override;
	inline void doLoad(Config* data) override { Load(data); }
	inline void doSave(Config& data)const  override { Save(data); }
};


#endif // !__COMPONENTINPUTUI_H__