#ifndef __PANELGAME_H__
#define __PANELGAME_H__

#include "UIPanel.h"
#include "Math.h"

class ComponentMaterial;


class UIPanelGame : public UIPanel
{
public:
	UIPanelGame(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelGame();

	void Draw();

public:
	ImTextureID img;

private:
	float2 lastSize;
	float2 size;
	float2 pos;
	ImVec2 region;

};
#endif // !__PANELGAME_H__

