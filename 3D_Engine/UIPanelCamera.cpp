#include "UIPanelCamera.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ComponentCamera.h"
#include "ImGui/imgui.h"

UIPanelCamera::UIPanelCamera(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelCamera::~UIPanelCamera()
{
}

void UIPanelCamera::Draw() {

	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	App->camera->cameraComp->DrawInspector();
	ImGui::End();
}
