#ifndef __COMPONENTRECTTRANS_H__
#define __COMPONENTRECTTRANS_H__

#include "Component.h"
#include "ComponentUI.h"


class ComponentRectTransform : public ComponentUI, public Component
{
	struct RectTransform
	{
		float2 localPosition = float2::zero;
		float2 globalPosition = float2::zero;
		float width = 0;
		float height = 0;
		float2 anchor = float2(0,0);
		float3* vertex = nullptr;		
		uint vertexID;

		bool isClicked = false;
	};

public:
	ComponentRectTransform();
	~ComponentRectTransform();

	void DrawInspector() override;

	bool Update() override;
	void CleanUp() override;

	void Load(Config* data) override;
	void Save(Config& data) const override;
	inline void doLoad(Config* data) override { Load(data); }
	inline void doSave(Config& data)const  override { Save(data); }
	
	inline void doUpdate() override { Update(); }
	inline void doCleanUp() override {	CleanUp();}

	const float GetWidth() const { return rect.width; }
	const float GetHeight() const { return rect.height; }
	const uint GetVertexID() const { return rect.vertexID; }
	const float2 GetLocalPos() const { return rect.localPosition; }
	const float2 GetGlobalPos() const { return rect.globalPosition; }
	const float2 GetMid() const { return float2(rect.width / 2, rect.height / 2); };

	void SetWidth(float w);
	void SetHeight(float h);
	void SetGlobalPos(float2 global);
	void SetLocalPos(float2 newLocalMat);

	void SetGlobalMatrixToDraw(float4x4 &globalMatrix);

	void UpdateUIComponents();

	void DrawUI() override;
	void GenBuffer();

	void CheckPicked();

private:

void UpdateLocalPos();

private:
	RectTransform rect;
};

#endif // !__COMPONENTRECTTRANS_H__