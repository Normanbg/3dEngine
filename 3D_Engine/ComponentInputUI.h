#ifndef __COMPONENTINPUTUI_H__
#define __COMPONENTINPUTUI_H__

#include "Component.h"
#include "ComponentUI.h"


#define DEFAULT_IDLE_IN "UI_Frame_white"
#define DEFAULT_PRESSED_IN "UI_Frame_black"

class ComponentTextUI;
class ComponentButtonUI;

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

	void ReceiveEvent(const Event &event) override;

	bool readInput = false;
	ComponentTextUI* cText = nullptr;
	ComponentButtonUI* cButton = nullptr;
	std::string textInput= std::string();
};


#endif // !__COMPONENTINPUTUI_H__