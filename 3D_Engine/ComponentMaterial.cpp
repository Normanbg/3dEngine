#include "ComponentMaterial.h"


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
}

void ComponentMaterial::Load(Config * data)
{
	uuid = data->GetUInt("UUID");
}
