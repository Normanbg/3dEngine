#include "UIPanelConsole.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"

UIPanelConsole::UIPanelConsole(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelConsole::~UIPanelConsole()
{
}

void UIPanelConsole::Draw() {

	ImGui::Begin(name.c_str(), &active);
	for (std::list<const char*>::iterator it = App->gui->logsBuffer.begin(); it != App->gui->logsBuffer.end(); it++) {
		ImGui::Text(*it);
	}
	ImGui::End();

}
