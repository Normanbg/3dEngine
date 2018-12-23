#ifndef __COMPONENTBUTTONUI_H__
#define __COMPONENTBUTTONUI_H__

#include "Component.h"
#include "ComponentUI.h"
#include "ResourceTexture.h"
#include "FakeFunctions.h"

#define DELTA_ALPHA 0.01f
#define DEFAULT_IDLE_BT "UI_ButtonIdle"
#define DEFAULT_HOVER_BT "UI_ButtonHover"
#define DEFAULT_PRESSED_BT "UI_ButtonPressed"

enum ButtonState
{
	IDLE,
	MOUSEOVER,
	PRESSED
};

class ComponentImageUI;

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

	void SetResource(uuid resource, int numRes);
	
	bool IsMouseOver();
	
	void PressedCallback();

public:
	ComponentImageUI* image = nullptr;
	ButtonState state = IDLE;
	ResourceTexture* idleImg = nullptr;
	ResourceTexture* hoverImg = nullptr;
	ResourceTexture* pressedImg = nullptr;

	Functions function = NO_FUNCTION;

private:
	void CheckState();
	const bool HasTexture(ResourceTexture * res) const;
	void ChangeGOImage();
	void FadeIn() override;
	void FadeOut() override;





private:
	bool hasSetToMid = false;
};


#endif // !__COMPONENTBUTTONUI_H__