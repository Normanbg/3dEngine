#ifndef __COMPONENTTEXTUI_H__
#define __COMPONENTTEXTUI_H__
#include "Component.h"
#include "ComponentUI.h"
#include "ComponentWithResource.h"

class ComponentTextUI : public ComponentUI, public Component
{

	class Font {
	public:
		std::string fontPath = std::string(FONTS_PATH);
		std::string exportTexPath = std::string(LIB_FONTS_PATH);
		float scale = 1.0f;
		char* text = nullptr;
		void ResetFont();
	};
public:
	ComponentTextUI();
	~ComponentTextUI();

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

	void LoadLabel(const char* label = "Insert Text", uint scale = 1, const char* font = "arial.ttf");

	Font font;
	

private: 

	float2* texCoords = nullptr;
	uint texGPUIndex = -1;
};



#endif // !__COMPONENTTEXTUI_H__