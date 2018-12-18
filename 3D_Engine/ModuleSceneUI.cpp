#include "ModuleSceneUI.h"



ModuleSceneUI::ModuleSceneUI()
{
}


ModuleSceneUI::~ModuleSceneUI()
{
}

void ModuleSceneUI::DrawInGameUI()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float left;
	float right;
	float top;
	float bottom;
	float nearPlane = 1000.f;
	float farPlane = -1000.f;

	//glOrtho()
}

void ModuleSceneUI::AddUIGameObject(GameObject* go)
{
	uiGameObjects.push_back(go);
}
