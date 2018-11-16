#include "UIPanelMaterials.h"
#include "Resource.h"
#include "ModuleResources.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

UIPanelMaterials::UIPanelMaterials(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}


UIPanelMaterials::~UIPanelMaterials()
{
}

void UIPanelMaterials::Draw()
{
	uint flags = 0;
	std::vector<Resource*> textures = App->resources->GetResourcesListType(Resource::ResType::Texture, true);
	
	
	if (textures.empty()) {
		flags = ImGuiTreeNodeFlags_Leaf;
	}//normanbg: I think that is not needed

	ImGui::Begin("Loaded Textures", &active);
	
	DrawTextureChilds(textures);

	//FOR DESELECTING MATERIALS
	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringWindow()) {
		App->scene->DeselectAll();
	}

	ImGui::End();

	ImGui::Begin("Resources", &active);

	DrawResourcesChilds(App->resources->GetResourcesList());
	ImGui::End();
	
}

void UIPanelMaterials::DrawTextureChilds(std::vector<Resource*> materials)
{
	uint flags = 0;
	for (std::vector<Resource*>::iterator goIterator = materials.begin(); goIterator != materials.end(); goIterator++)
	{
		Resource* mat = *goIterator;
		flags |= ImGuiTreeNodeFlags_Leaf;
		if (ImGui::TreeNodeEx(mat->GetName(), flags)) {
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))///need to work on it
			{
				App->scene->DeselectAll();
				App->scene->ShowTextureResourceInspector(mat->GetUUID());
			}
			ImGui::TreePop();
		}
		mat = nullptr;
	}
}

void UIPanelMaterials::DrawResourcesChilds(std::map<uuid, Resource*> resources)
{
	uint flags = 0;
	for (std::map<uuid, Resource*>::iterator goIterator = resources.begin(); goIterator != resources.end(); goIterator++)
	{
		Resource* res = (*goIterator).second;
		flags |= ImGuiTreeNodeFlags_Leaf;
		std::string name = res->GetName();
		if (ImGui::TreeNodeEx(name.c_str(), flags)) {
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("UUID: %d", res->GetUUID());
				ImGui::Text("Directory: %s", res->GetPath());
				ImGui::Text("Reference Count: %d", res->GetCountReferences());
				ImGui::EndTooltip();
			}
			ImGui::TreePop();
		}
		res = nullptr;
	}
}
