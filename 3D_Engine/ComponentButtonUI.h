#ifndef __COMPONENTBUTTONUI_H__
#define __COMPONENTBUTTONUI_H__

#include "Component.h"
#include "ComponentUI.h"
#include "ResourceTexture.h"

#define DELTA_ALPHA 0.01f

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

public:
	ComponentImageUI* image = nullptr;
	ButtonState state = IDLE;
	ResourceTexture* idleImg = nullptr;
	ResourceTexture* hoverImg = nullptr;
	ResourceTexture* pressedImg = nullptr;

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