#ifndef __COMPONENTBUTTONUI_H__
#define __COMPONENTBUTTONUI_H__

#include "Component.h"
#include "ComponentUI.h"

class ComponentButtonUI : public ComponentUI, public Component
{
public:
	ComponentButtonUI();
	~ComponentButtonUI();

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


#endif // !__COMPONENTBUTTONUI_H__