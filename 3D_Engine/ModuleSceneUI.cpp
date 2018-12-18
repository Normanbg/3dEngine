#include "ModuleSceneUI.h"
#include "ModuleScene.h"
#include "GameObject.h"

ModuleSceneUI::ModuleSceneUI()
{
}


ModuleSceneUI::~ModuleSceneUI()
{
}

void ModuleSceneUI::DrawInGameUI()
{
	GameObject* canvas = App->scene->GetFirstGameObjectCanvas();
	if (canvas) {
		for (auto it : App->scene->uiGameObjects) {
			if (canvas != it) {
				ComponentRectTransform* rectTransform = it->GetComponentRectTransform();
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				float left = canvas->GetComponentRectTransform()->GetPos().x - (rectTransform->GetWidth() / 2);
				float right= canvas->GetComponentRectTransform()->GetPos().x + (rectTransform->GetWidth() / 2);;
				float top = canvas->GetComponentRectTransform()->GetPos().y + (rectTransform->GetHeight() / 2);;
				float bottom = canvas->GetComponentRectTransform()->GetPos().y - (rectTransform->GetHeight() / 2);;
				float zNear = 1000.f;
				float zFar = -1000.f;

				glOrtho(left, right, bottom, top, zNear, zFar);

			}
		}
	}
}

void ModuleSceneUI::AddUIGOtoList(GameObject* go)
{

	uiGameObjects.push_back(go);
}
