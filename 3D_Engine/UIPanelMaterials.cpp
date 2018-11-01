#include "UIPanelMaterials.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GameObject.h"

UIPanelMaterials::UIPanelMaterials(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}


UIPanelMaterials::~UIPanelMaterials()
{
}

void UIPanelMaterials::Draw()
{
	uint flags = 0;
	if (App->textures->materials.empty()) {
		flags = ImGuiTreeNodeFlags_Leaf;
	}
	ImGui::Begin(name.c_str(), &active, flags);
	
	DrawChilds(App->textures->materials);
	ImGui::End();
	
}

void UIPanelMaterials::DrawChilds(std::vector<Material*> materials)
{
	uint flags = 0;
	for (std::vector<Material*>::iterator goIterator = materials.begin(); goIterator != materials.end(); goIterator++)
	{
		Material* mat = *goIterator;
		flags |= ImGuiTreeNodeFlags_Leaf;
		if (ImGui::TreeNodeEx(mat->name.c_str(), flags)) {
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))///need to work on it
			{/*
				GameObject* helper = new GameObject((*texIterator).name.c_str());
				ComponentMaterial* comp = (ComponentMaterial*) helper->AddComponent(MATERIAL);
				comp->texture = &(*texIterator);*/
				App->scene->DeselectAll();
				App->scene->ShowMaterialInspector(mat);
			}
			ImGui::TreePop();
		}
		mat = nullptr;
	}
}
