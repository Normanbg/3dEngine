#include "UIPanelHierarchy.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleScene.h"

UIPanelHierarchy::UIPanelHierarchy(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelHierarchy::~UIPanelHierarchy()
{
}

void UIPanelHierarchy::Draw()
{
	ImGui::Begin(name.c_str(), &active);
	int i = 0;
	for (std::vector<GameObject*>::iterator rootIterator = App->scene->root->childrens.begin(); rootIterator != App->scene->root->childrens.end(); i++, rootIterator++)
	{
		char gameObjNum[30];
		sprintf_s(gameObjNum, 30, "%d.%s", i + 1, (*rootIterator)->name.c_str());
		if (ImGui::CollapsingHeader(gameObjNum)) {
			if ((*rootIterator)->childrens.size() > 0)
			{
				if (ImGui::CollapsingHeader(gameObjNum)) {
					//ImGui::PushID("Transformation" + i); //USED FOR RECURSIVITY ON IMGUI
					for (std::vector<GameObject*>::iterator GOChildsIterator = App->scene->root->childrens.begin(); rootIterator != App->scene->root->childrens.end(); i++, rootIterator++)
					{
						//DO RECURSIVITY, BETTER IN A FUNCTION APART. ISN'T IT?
					}
				}
				else
				{
					ImGui::Spacing();
				}
				//ImGui::PopID(); //USED FOR RECURSIVITY ON IMGUI
			}
		}
	}
	ImGui::End();
}

