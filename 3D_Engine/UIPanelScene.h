#ifndef __PANELSCENE_H__
#define __PANELSCENE_H__

#include "UIPanel.h"
#include "Math.h"

class ComponentMaterial;


class UIPanelScene : public UIPanel
{
public:
	UIPanelScene(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelScene();

	void Draw();
	void ClearScenePopUp();

	const ImVec2 GetMouse() const;

public:
	ImTextureID img;
	//ComponentMaterial* playButtonMat;
	//ComponentMaterial* pauseButtonMat;
	//ComponentMaterial* stopButtonMat;

private:
	float2 lastSize;
	float2 size;
	float2 pos;
	ImVec2 region;

};
#endif // !__PANELSCENE_H__
