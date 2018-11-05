#include "ComponentMaterial.h"
#include "ModuleRenderer3D.h"

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
	texture = nullptr;
}

void ComponentMaterial::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text(texture->name.c_str());
	
	
	ImGui::Text("Texture size:\nWidth: %dpx \nHeight: %dpx ", texture->texWidth, texture->texHeight);
	float windowSize = ImGui::GetWindowContentRegionWidth();
	ImGui::Image((void*)(texture->textureID), ImVec2(windowSize, windowSize));

	ImGui::Separator();
}

void ComponentMaterial::Save(Config & data) const
{
	data.AddUInt("UUID", uuid);
	if (texture)
		data.AddString("TexName", texture->name.c_str());
}

void ComponentMaterial::Load(Config * data)
{
	uuid = data->GetUInt("UUID");
	
	texture = new Material();
	texture->name =	data->GetString("TexName", "NoName");
	std::string path = LIB_TEXTURES_PATH + texture->name + DDS_FORMAT;
	texture->textureID = App->renderer3D->texImporter->LoadTexture(path.c_str(), texture);
	if (App->textures->CheckIfImageAlreadyLoaded(texture->name.c_str()) == -1) {
		App->textures->AddMaterial(texture);
	}
}
