#include "UIPanelConsole.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"

#include "mmgr/mmgr.h"

UIPanelConsole::UIPanelConsole(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelConsole::~UIPanelConsole()
{
}

void UIPanelConsole::Draw() {

	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	for (std::vector<std::string>::iterator it = App->gui->logsBuffer.begin(); it != App->gui->logsBuffer.end(); it++) {
		ImGui::Text((*it).c_str());
		//ImGui::SetScrollHere(1.0f); // to scroll the console always to the recent logs
	}
	ImGui::End();
}
