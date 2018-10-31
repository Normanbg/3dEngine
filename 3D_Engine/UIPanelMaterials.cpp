#include "UIPanelMaterials.h"
#include "ModuleTextures.h"


UIPanelMaterials::UIPanelMaterials(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}


UIPanelMaterials::~UIPanelMaterials()
{
}

void UIPanelMaterials::Draw()
{
	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_MenuBar);
	
	ImGui::End();
	DrawChilds(App->textures->materials);
}

void UIPanelMaterials::DrawChilds(std::vector<Material> materials)
{
	for (std::vector<Material>::iterator texIterator = materials.begin(); texIterator != materials.end(); texIterator++)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;
		bool treeNodeOpened = ImGui::TreeNodeEx((*texIterator).path.c_str(), flags);
	}
}
