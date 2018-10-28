#include "UIPanelInspector.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleScene.h"

UIPanelInspector::UIPanelInspector(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelInspector::~UIPanelInspector()
{
}

void UIPanelInspector::Draw() {
	ImGui::Begin(name.c_str(), &active);

	std::vector<GameObject*> gObjsSelecRecover = App->scene->gObjsSelected;
	if (gObjsSelecRecover.size() == 1) {
		GameObject* test = gObjsSelecRecover.at(0);
		std::vector<Component*> componentsRecover = test->components;
		for (std::vector<Component*>::iterator itComponents = componentsRecover.begin(); itComponents != componentsRecover.end(); itComponents++) {
			DrawComponent((*itComponents)->type);
		}
	}
	ImGui::End();
}

void UIPanelInspector::DrawComponent(ComponentType compDraw)
{
	switch (compDraw)
	{
	case NO_TYPE:
		break;
	case TRANSFORM:
		break;
	case MESH:
		break;
	case MATERIAL:
		break;
	case CAMERA:
		break;
	default:
		break;
	}
}

