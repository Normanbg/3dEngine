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
}

void ComponentMaterial::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text(texture->name.c_str());
	
	ImGui::Separator();
}
