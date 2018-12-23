#ifndef __COMPONENTCHECKUI_H__
#define __COMPONENTCHECKUI_H__

#include "Component.h"
#include "ComponentUI.h"
#include "ComponentButtonUI.h"

class ComponentImageUI;

class ComponentCheckBoxUI : public ComponentUI, public Component

{
public:
	ComponentCheckBoxUI();
	~ComponentCheckBoxUI();
	
	void DrawInspector() override;

	bool Start() override;
	bool Update() override;
	void CleanUp() override;

	void CheckState();

	inline void doStart() override { Start(); }
	inline void doUpdate() override { Update(); }
	inline void doCleanUp() override { CleanUp(); }

	void Load(Config* data) override;
	void Save(Config& data) const override;
	inline void doLoad(Config* data) override { Load(data); }
	inline void doSave(Config& data)const  override { Save(data); }

	bool IsMouseOver();

public:
	ComponentImageUI* image = nullptr;
	ButtonState state = IDLE;
	ResourceTexture* unpressedImg = nullptr;
	ResourceTexture* pressedImg = nullptr;

private:
	void ChangeGOImage();
	const bool HasTexture(ResourceTexture * res) const;
	void SetResource(uuid resource, int numRes);

private:
	bool hasSetToMid = false;
};


#endif // !__COMPONENTCHECKUI_H__