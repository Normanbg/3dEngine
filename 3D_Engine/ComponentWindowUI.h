#ifndef __COMPONENTWINUI_H__
#define __COMPONENTWINUI_H__

#include "ComponentUI.h"
class ComponentWindowUI :	public ComponentUI, public Component
{
public:
	ComponentWindowUI();
	~ComponentWindowUI();

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

	void DrawInspector() override;
	void DrawUI() override;

};



#endif // !__COMPONENTWINUI_H__