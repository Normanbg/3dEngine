#include "ComponentTextUI.h"

#include "mmgr/mmgr.h"




ComponentTextUI::ComponentTextUI()
{
	typeUI = UI_TEXT;
	type = UI;
}


ComponentTextUI::~ComponentTextUI()
{
}

bool ComponentTextUI::Update()
{
	return true;
}

void ComponentTextUI::CleanUp()
{
}

void ComponentTextUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
}

void ComponentTextUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
}

void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
}
