#ifndef __COMPONENTWINUI_H__
#define __COMPONENTWINUI_H__

#include "Component.h"
#include "ComponentUI.h"
#include "ResourceTexture.h"

enum WindowState
{
	WINDOW_IDLE,
	WIN_MOUSEOVER,
	DRAGGING
};
class ComponentImageUI;
class ComponentWindowUI :	public ComponentUI, public Component
{
public:
	ComponentWindowUI();
	~ComponentWindowUI();

	bool Start() override;
	bool Update() override;
	void CleanUp() override;

	void DraggWindow();

	inline void doStart() override { Start(); }
	inline void doUpdate() override { Update(); }
	inline void doCleanUp() override { CleanUp(); }

	void Load(Config* data) override;
	void Save(Config& data) const override;
	inline void doLoad(Config* data) override { Load(data); }
	inline void doSave(Config& data)const  override { Save(data); }

	void DrawInspector() override;
	void DrawUI() override;

	void SetDraggable(bool dragg) { draggable = dragg; }
	bool IsDraggable() { return draggable; }

public:
	ComponentImageUI* image = nullptr;
	ResourceTexture* windImage = nullptr;
	WindowState state = WindowState::WINDOW_IDLE;

private: 
	void CheckState();
	bool IsMouseOver();
	bool CheckChildsState();
	void SetResource(uuid resource);

private:
	bool draggable = false;
	bool hasSetToMid = false;
	float2 initMousePos = float2::zero;
	float2 lastPos = float2::zero;
};



#endif // !__COMPONENTWINUI_H__