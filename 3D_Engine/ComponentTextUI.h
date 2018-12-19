#ifndef __COMPONENTTEXTUI_H__
#define __COMPONENTTEXTUI_H__
#include "Component.h"
#include "ComponentUI.h"

#define DEFAULT_FONT "arial.ttf"

class ComponentTextUI : public ComponentUI, public Component
{

	class Font {
	public:
		std::string fontDir = std::string(FONTS_PATH );
		std::string fontSrc = std::string(DEFAULT_FONT);
		std::string exportTexPath = std::string(LIB_FONTS_PATH);
		std::string text = std::string("Insert Text.");
		float scale = 1.0f;		
		void ResetFont();
	};
public:
	ComponentTextUI();
	~ComponentTextUI();

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

	
	
	Font font;
	

private: 
	void LoadLabel(const char* label = "Insert Text", float scale = 1.0f, const char* font = DEFAULT_FONT);

	std::vector<std::string> loadedFonts;
	float2* texCoords = nullptr;
	uint texGPUIndex = -1;
};



#endif // !__COMPONENTTEXTUI_H__