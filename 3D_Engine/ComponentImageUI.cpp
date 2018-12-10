#include "ComponentImageUI.h"

#include "ModuleResources.h"

#include "mmgr/mmgr.h"



ComponentImageUI::ComponentImageUI()
{
	type = UI_IMAGE;
}


ComponentImageUI::~ComponentImageUI()
{
}

bool ComponentImageUI::Update()
{
	return true;
}

void ComponentImageUI::CleanUp()
{
	if (HasTexture()) {
		resourceTexture->CleanUp();
	}
	resourceTexture = nullptr;
	myGO = nullptr;
}

void ComponentImageUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());

	const char* currentMaterial = NULL;
	if (resourceTexture != nullptr) {
		currentMaterial = resourceTexture->GetName();
	}
	if (ImGui::BeginCombo("Material", currentMaterial))
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);

		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (currentMaterial != nullptr) {
				bool is_selected = (strcmp(currentMaterial, mat[i]->GetName()) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				currentMaterial = mat[i]->GetName();
				SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture));

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}
	currentMaterial = nullptr;
	ImGui::Separator();
	if (resourceTexture != nullptr) {
		ImGui::Text("Resource UUID: %i", resourceTexture->GetUUID());
		ImGui::Text(resourceTexture->GetName());

		ImGui::Text("Texture size:\nWidth: %dpx \nHeight: %dpx ", resourceTexture->width, resourceTexture->height);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(resourceTexture->gpuID), ImVec2(windowSize, windowSize), ImVec2(0, 1), ImVec2(1, 0));
		
	}
}

const bool ComponentImageUI::HasTexture() const
{
	bool ret;
	resourceTexture ? ret = true : ret = false;
	return ret;
}

void ComponentImageUI::SetResource(uuid resource)
{
	resourceTexture = (ResourceTexture*)App->resources->Get(resource);
	resourceTexture->LoadInMemory();
	resourceUUID = resource;
}
const uint ComponentImageUI::GetTexID() const
{
	if (HasTexture()) {
		return resourceTexture->gpuID;
	}
	return -1;
}
