#include "ComponentTextUI.h"

#include "mmgr/mmgr.h"




ComponentTextUI::ComponentTextUI()
{
	type = UI_TEXT;
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

void ComponentTextUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
}
