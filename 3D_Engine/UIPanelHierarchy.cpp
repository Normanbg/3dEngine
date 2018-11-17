#include "UIPanelHierarchy.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

#include "mmgr/mmgr.h"

UIPanelHierarchy::UIPanelHierarchy(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelHierarchy::~UIPanelHierarchy()
{
}

void UIPanelHierarchy::Draw()
{
	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Game Object")) {
				App->scene->AddGameObject("newGameObject");
			}
			if (ImGui::MenuItem("Cube")) {
				//App->scene->CreateCube();
			}
			//if (ImGui::MenuItem("Load")) { App->LoadGame(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	//FOR DESELECTING GAME OBJECTS
	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringWindow()) {
		App->scene->DeselectAll();
	}
	DrawChilds(App->scene->root->childrens);
	ImGui::End();
}

void UIPanelHierarchy::DrawChilds(std::vector<GameObject*> childs){
	for (std::vector<GameObject*>::iterator goIterator = childs.begin(); goIterator != childs.end(); goIterator++)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if ((*goIterator) == App->scene->gObjSelected)
			flags |= ImGuiTreeNodeFlags_Selected;
		if ((*goIterator)->childrens.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		//TODO::COLOR IF IS ACTIVE--------------

		ImGui::PushID((*goIterator)->GetUUID());
		bool treeNodeOpened = ImGui::TreeNodeEx((*goIterator)->name.c_str(), flags);
		ImGui::PopID();
		
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
		{
			//CTRL pressed = addselectedG0
			if ((App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT))
			{
				App->scene->ShowGameObjectInspector((*goIterator));
			}
			else
			{
				if (!ImGui::IsItemClicked(1) || (*goIterator)!= App->scene->gObjSelected)
				{
					App->scene->DeselectAll();
					App->scene->ShowGameObjectInspector((*goIterator));
				}
			}
		}

		if (treeNodeOpened)
		{
			DrawChilds((*goIterator)->childrens);
			ImGui::TreePop();
		}
	}
}


