#include "UIPanelHierarchy.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

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
			if (ImGui::MenuItem("Cube")) {
				App->scene->CreateCube();
			}
			//if (ImGui::MenuItem("Load")) { App->LoadGame(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	DrawChilds(App->scene->root->childrens);
	ImGui::End();
}

void UIPanelHierarchy::DrawChilds(std::vector<GameObject*> childs){
	for (std::vector<GameObject*>::iterator goIterator = childs.begin(); goIterator != childs.end(); goIterator++)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if ((*goIterator)->GetSelected())
			flags |= ImGuiTreeNodeFlags_Selected;
		if ((*goIterator)->childrens.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;
		//TODO::COLOR IF IS ACTIVE--------------
		bool treeNodeOpened = ImGui::TreeNodeEx((*goIterator)->name.c_str(), flags);
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
		{
			//CTRL pressed = addselectedG0
			if ((App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT))
			{
				App->scene->AddGameObjectToSelectedList((*goIterator));
			}
			else
			{
				if (!ImGui::IsItemClicked(1) || !(*goIterator)->GetSelected())
				{
					App->scene->DeselectAll();
					App->scene->AddGameObjectToSelectedList((*goIterator));
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


