#ifndef __COMPONENTTEXTUI_H__
#define __COMPONENTTEXTUI_H__
#include "Component.h"
#include "ComponentUI.h"
#include "FontManager.h"
#include <string>

#define LABEL_Y_LIMIT 10 

class ComponentTextUI : public ComponentUI, public Component
{
	
	struct Label {
		Font* font = nullptr;
		float3 color = float3(1,1,1);
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
	float GetLabelWidth() const { return labelFrame[3].x - labelFrame[0].x + initOffsetX; }
	float GetLabelHeight() const { return labelFrame[1].y - labelFrame[0].y; }

private: 
	
	void AddCharPanel(char character, bool first = false);
	void FillCharPlanes();
	void EnframeLabel(float3 * points);
	float3 GetCornerLabelPoint(int corner);
	bool ShiftNewLine(float3& cursor,  int& line, int i);
	void SetFontScale(uint scale);
	void SetFont(const char * font);
	void CleanCharPlanes();
	
	std::vector<CharPlane*> charPlanes;
	std::vector<float3> offsetPlanes;
	float2* texCoords = nullptr;
	float3 labelFrame[4];
	float initOffsetX = 0.0f;
	Label label;
	int lineSpacing = 30;
	bool drawCharPanel = true;
	bool drawLabelrect = true;
	
};



#endif // !__COMPONENTTEXTUI_H__