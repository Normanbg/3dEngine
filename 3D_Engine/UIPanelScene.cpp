#include "UIPanelScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"

UIPanelScene::UIPanelScene(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelScene::~UIPanelScene()
{
}

void UIPanelScene::Draw() {
	uint flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("Scene", &active, flags);
	img = (ImTextureID)App->renderer3D->texture;
	ImGui::Image(img, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
	App->gui->MouseOnScene(ImGui::IsMouseHoveringWindow());
	ImGui::End();

}
