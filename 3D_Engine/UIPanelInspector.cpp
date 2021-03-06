#include "UIPanelInspector.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleResources.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentUI.h"
#include "ModuleScene.h"

#include "mmgr/mmgr.h"

UIPanelInspector::UIPanelInspector(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelInspector::~UIPanelInspector()
{
}

void UIPanelInspector::Draw() {
	ImGui::Begin(name.c_str(), &active);
	if (App->scene->gObjSelected != nullptr && App->scene->TextureResourceSelected != 0) {
		assert("Error. UI Inspector has material and gameobject to show. Need to deselect before.");
	}
	if (App->scene->gObjSelected != nullptr) {

		GameObject* go = App->scene->gObjSelected;
		/*ImGui::Checkbox("Active", &go->active);
		ImGui::SameLine();*/
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), go->name.c_str());
		ImGui::SameLine();
		ImGui::Text("| ");
		ImGui::SameLine();
		if (ImGui::Checkbox("Static", &go->staticGO))
		{
			if (go->staticGO)
				App->scene->staticsGObjs++;
			else 
				App->scene->staticsGObjs--;
			go->SetChildsStatic(go->staticGO);
			App->scene->SetQuadTree();
		}	
		ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f,1),"UUID: %i", go->GetUUID());
		ImGui::Separator();
		std::vector<Component*> componentsRecover = go->components;
		for (std::vector<Component*>::iterator itComponents = componentsRecover.begin(); itComponents != componentsRecover.end(); itComponents++) {
			DrawComponent((*itComponents));
		}
		std::vector<ComponentUI*> componentsUIRecover = go->componentsUI;
		for (std::vector<ComponentUI*>::iterator itComponents = componentsUIRecover.begin(); itComponents != componentsUIRecover.end(); itComponents++) {
			DrawUIComponent((*itComponents));
		}
		if (go->GetComponentRectTransform() == nullptr)
		{
			if (ImGui::CollapsingHeader("Add Component")) {

				if (!go->GetComponentCamera()) {
					if (ImGui::Button("Camera")) {
						go->AddComponent(CAMERA);
					}
				}

				if (!go->GetComponentMaterial()) {
					if (ImGui::Button("Material")) {
						ComponentMaterial* mat = (ComponentMaterial*)go->AddComponent(MATERIAL);
						ComponentMesh* m = go->GetComponentMesh();
						if (m) {
							m->SetMaterial(mat);
						}
						mat = nullptr;
						m = nullptr;
					}
				}
				if (!go->GetComponentMesh()) {
					if (ImGui::Button("Mesh")) {
						ComponentMesh* m = (ComponentMesh*)go->AddComponent(MESH);
						ComponentMaterial* mat = go->GetComponentMaterial();
						if (mat) {
							m->SetMaterial(mat);
						}
						mat = nullptr;
						m = nullptr;
					}
				}
			}
		}


	}

	if (App->scene->TextureResourceSelected != 0) {


		ResourceTexture* mat =( ResourceTexture *) App->resources->Get(App->scene->TextureResourceSelected);
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), mat->GetName());
		ImGui::Text("Texture size: %dpx x %dpx ", mat->width, mat->height);
		ImGui::Text("ID: %i", mat->gpuID);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(mat->gpuID), size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Separator();

	}
	ImGui::End();
}

void EndButtonDropDown()
{
	ImGui::PopStyleColor(3);
	ImGui::EndPopup();
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
			compDraw->myGO->GetComponentTransform()->DrawInspector();
			ImGui::TreePop();
		}
		break;
	
	case CANVAS:
		if (ImGui::TreeNode("Canvas"))
		{
			compDraw->myGO->GetComponentCanvas()->DrawInspector();
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

void UIPanelInspector::DrawUIComponent(ComponentUI* compDraw)
{
	ComponentTypeUI cType = compDraw->typeUI;
	switch (cType)
	{
	case NOTYPE:
		break;
	case UI_IMAGE:
		if (ImGui::TreeNode("UI Image"))
		{
			ComponentImageUI* comp =(ComponentImageUI*) compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case UI_TEXT:
		if (ImGui::TreeNode("UI Text"))
		{
			ComponentTextUI* comp = (ComponentTextUI*)compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case UI_BUTTON:
		if (ImGui::TreeNode("UI Button"))
		{
			ComponentButtonUI* comp = (ComponentButtonUI*)compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case UI_INPUT:
		if (ImGui::TreeNode("UI Input"))
		{
			ComponentInputUI* comp = (ComponentInputUI*)compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case UI_CHECKBOX:
		if (ImGui::TreeNode("UI Checkbox"))
		{
			ComponentCheckBoxUI* comp = (ComponentCheckBoxUI*)compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case TRANSFORMRECT:
		if (ImGui::TreeNode("TransformRect"))
		{
			ComponentRectTransform* comp = (ComponentRectTransform*)compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	case UI_WINDOW:
		if (ImGui::TreeNode("UI Window"))
		{
			ComponentWindowUI* comp = (ComponentWindowUI*)compDraw;
			comp->DrawInspector();
			ImGui::TreePop();
		}
		break;
	}
}
