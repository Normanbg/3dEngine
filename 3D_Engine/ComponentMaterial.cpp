#include "ComponentMaterial.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"

#include "mmgr/mmgr.h"

ComponentMaterial::ComponentMaterial()
{
	type = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::Update()
{
	return true;
}

void ComponentMaterial::CleanUp()
{
	if (HasTexture()) {
		resourceTexture->FreeInMemory();
	}
	resourceTexture = nullptr;
}

void ComponentMaterial::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	
	if (resourceTexture!= nullptr) {
		ImGui::Text("Resource UUID: %i", resourceTexture->GetUUID());
		ImGui::Text(resourceTexture->GetName());

		ImGui::Text("Texture size:\nWidth: %dpx \nHeight: %dpx ", resourceTexture->width, resourceTexture->height);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(resourceTexture->gpuID), ImVec2(windowSize, windowSize));
	}

	ImGui::Text("Color:");
	ImGui::ColorPicker3("Color##2f", (float*)&colors);
	ImGui::Separator();
}

void ComponentMaterial::SetResource(uuid resource)
{
	resourceTexture = (ResourceTexture*)App->resources->Get(resource);
	resourceTexture->LoadInMemory();
}

const uint ComponentMaterial::GetTexID() const
{
	if (HasTexture()) {
		return resourceTexture->gpuID;
	}
	return -1;
}

const char * ComponentMaterial::GetTextureName() const
{
	if (HasTexture()) {
		return resourceTexture->GetName();
	}
	return nullptr;
}

const bool ComponentMaterial::HasTexture() const
{
	bool ret;
	resourceTexture ? ret = true : ret = false;
	return ret;
}

void ComponentMaterial::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	if (resourceTexture) {
		data.AddString("TexName", resourceTexture->GetName());
	}
	if (!colors.IsZero()) {
		data.AddFloat3("Colors", colors);
	}
}

void ComponentMaterial::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	colors = data->GetFloat3("Colors", { 0,0,0 });
	std::string matName = data->GetString("TexName", "NoName");
	
	if (matName != "NoName") {
		resourceTexture =(ResourceTexture*) App->resources->Get(App->resources->FindByName(matName.c_str(), Resource::ResType::Texture));
		resourceTexture->LoadInMemory();
	}
}
