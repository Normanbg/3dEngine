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
	int h = 0;
	int w = 0;
	App->window->GetSize(w, h);
	img = (ImTextureID)App->renderer3D->texture;
	ImGui::Image(img, ImVec2(w / 2,h / 2), ImVec2(0, 1), ImVec2(1, 0));
	App->gui->MouseOnScene(ImGui::IsMouseHoveringWindow());
	ImGui::End();

}
