#ifndef __PANELSCENE_H__
#define __PANELSCENE_H__

#include "UIPanel.h"

class ComponentMaterial;

class UIPanelScene : public UIPanel
{
public:
	UIPanelScene(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelScene();

	void Draw();

public:
	ImTextureID img;
	ComponentMaterial* playButtonMat;
	ComponentMaterial* pauseButtonMat;
	ComponentMaterial* stopButtonMat;
};
#endif // !__PANELSCENE_H__
