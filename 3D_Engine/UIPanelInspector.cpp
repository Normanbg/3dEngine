#include "UIPanelInspector.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleScene.h"

UIPanelInspector::UIPanelInspector(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelInspector::~UIPanelInspector()
{
}

void UIPanelInspector::Draw() {
	ImGui::Begin(name.c_str(), &active);
	if (App->scene->gObjSelected != nullptr && App->scene->materialSelected != nullptr) {
		assert("Error. UI Inspector has material and gameobject to show. Need to deselect before.");
	}
	if (App->scene->gObjSelected != nullptr) {

		GameObject* go = App->scene->gObjSelected;
		ImGui::Checkbox("Active", &go->active);
		ImGui::SameLine();
		ImGui::Text("| Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), go->name.c_str());
		ImGui::SameLine();
		ImGui::Text("| ");
		ImGui::SameLine();
		ImGui::Checkbox("Static", &go->staticGO);
		ImGui::Separator();
		std::vector<Component*> componentsRecover = go->components;
		for (std::vector<Component*>::iterator itComponents = componentsRecover.begin(); itComponents != componentsRecover.end(); itComponents++) {
			DrawComponent((*itComponents));
		}

	}
	if (App->scene->materialSelected != nullptr) {

		Material* mat = App->scene->materialSelected;
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), mat->name.c_str());
		ImGui::Text("Texture size: %dpx x %dpx ", mat->texWidth, mat->texHeight);
		ImGui::Text("ID: %i", mat->textureID);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(mat->textureID), ImVec2(windowSize, windowSize));
		ImGui::Separator();

	}
	ImGui::End();
}

void UIPanelInspector::DrawComponent(Component* compDraw)
{
	ComponentType cType = compDraw->type;
	switch (cType)
	{
	case NO_TYPE:
		break;
	case TRANSFORM:
		if (ImGui::TreeNode("Transform"))
		{
			compDraw->myGO->GetTransformComponent()->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case MESH:
		if (ImGui::TreeNode("Mesh"))
		{
			compDraw->myGO->GetComponentMesh()->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case MATERIAL:
		if (ImGui::TreeNode("Material"))
		{
			compDraw->myGO->GetComponentMaterial()->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case CAMERA:
		if (ImGui::TreeNode("Camera"))
		{
			compDraw->myGO->GetComponentCamera()->DrawInspector();
			ImGui::TreePop();
		}
		break;
	default:
		break;
	}
}

