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
	if (texture) {
		ImGui::Text(texture->name.c_str());


		ImGui::Text("Texture size:\nWidth: %dpx \nHeight: %dpx ", texture->texWidth, texture->texHeight);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(texture->textureID), ImVec2(windowSize, windowSize));
	}
	ImGui::Text("Color:");
	ImGui::ColorPicker3("Color##2f", (float*)&colors);
	ImGui::Separator();
}

void ComponentMaterial::Save(Config & data) const
{
	data.AddUInt("UUID", uuid);
	if (texture)
		data.AddString("TexName", texture->name.c_str());
	if (!colors.IsZero()) {
		data.AddFloat3("Colors", colors);
	}
}

void ComponentMaterial::Load(Config * data)
{
	uuid = data->GetUInt("UUID");
	colors = data->GetFloat3("Colors", { 0,0,0 });
	std::string matName = data->GetString("TexName", "NoName");	
	
	if (matName != "NoName") {
		texture = new Material();
		texture->name = matName;
		std::string path = LIB_TEXTURES_PATH + texture->name + DDS_FORMAT;
		texture->textureID = App->renderer3D->texImporter->LoadTexture(path.c_str(), texture);
		if (App->textures->CheckIfImageAlreadyLoaded(texture->name.c_str()) == -1) {
			App->textures->AddMaterial(texture);
		}
	}
	


	

}
