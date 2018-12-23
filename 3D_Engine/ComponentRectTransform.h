#ifndef __COMPONENTRECTTRANS_H__
#define __COMPONENTRECTTRANS_H__

#include "Component.h"
#include "ComponentUI.h"


class ComponentRectTransform : public ComponentUI, public Component
{
	friend class ComponentTextUI;
	struct RectTransform
	{
		float2 localPosition = float2::zero;
		float2 globalPosition = float2::zero;
		float width = 0.f;
		float height = 0.f;
		float percentatgeWidth = 0.f;
		float percentatgeHeight = 0.f;
		float2 anchor = float2(0.f,0.f);
		float3* vertex = nullptr;		
		uint vertexID;

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
	const float2 GetMid() const;

	void SetWidth(float w, bool isCanvas = false);
	void MoveCanvasXChilds();
	void MoveCanvasYChilds();
	void SetHeight(float h, bool isCanvas = false);
	void SetGlobalPos(float2 global);
	void SetLocalPos(float2 newLocalMat);
	void SetToMid();

	void SetGlobalMatrixToDraw(float4x4 &globalMatrix);

	void SetCheckState(bool _checkState) { _checkState = checkState;}

	void UpdateUIComponents();

	void DrawUI() override;
	void GenBuffer();



private:

void UpdateLocalPos();
void UpdatePercentatge();
void UpdateSizeWithPercentatge(float lastParentWidth, float lastParentHeight);

protected:
	RectTransform rect;
	bool checkState = false;
};

#endif // !__COMPONENTRECTTRANS_H__