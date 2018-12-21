#ifndef __COMPONENTTEXTUI_H__
#define __COMPONENTTEXTUI_H__
#include "Component.h"
#include "ComponentUI.h"
#include "FontManager.h"
#include <string>


class ComponentTextUI : public ComponentUI, public Component
{
	struct Label {
		Font* font = nullptr;
		int scale = 0;
		float3 color = float3(0,0,0);
		float2 textOrigin = float2(0, 0);
		std::string text = std::string("Default Text.");
	};
	class CharPlane {
	public:
		void GenBuffer(float2 size);

		float3* vertex = nullptr;
		uint vertexID =-1;
		uint textureID = -1;
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

	
	void SetText(const char* txt);


private: 
	//void LoadLabel(const char* label = "Insert Text", float scale = 1.0f, const char* font = DEFAULT_FONT);

	void AddCharPanel(char character);

	void FillCharPlanes();
	void EnframeLabel(float3 * points);
	float3 GetCornerLabelPoint(int corner);

	std::vector<std::string> loadedFonts;
	std::vector<CharPlane*> charPlanes;
	std::vector<float3> offsetPlanes;
	float2* texCoords = nullptr;
	float3 labelFrame[4];
	//uint texGPUIndex = -1;
	Label label;
	uint lineSpacing = 0;
	bool drawCharPanel = true;
	bool drawLabelrect = true;
	
};



#endif // !__COMPONENTTEXTUI_H__