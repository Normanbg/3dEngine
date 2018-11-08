#include "UIPanelScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"
#include "ComponentMaterial.h"
#include "ImGui/imgui.h"

UIPanelScene::UIPanelScene(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
	playButtonMat = new ComponentMaterial();
	pauseButtonMat = new ComponentMaterial();
	stopButtonMat = new ComponentMaterial();
}

UIPanelScene::~UIPanelScene()
{
}

void UIPanelScene::Draw() {
	uint flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	/*playButtonMat->Load()*/
	ImGui::Begin("Scene", &active, flags);
	ImVec2 region_size = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosX(region_size.x / 2 - 30);	
	if (ImGui::Button("Play", { 40, 20 }))
	{
		App->scene->inGame = true;

	}
	ImGui::SameLine();
	if (ImGui::Button("Pause", { 40, 20 }))
	{
		

	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0,0,1 });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.2f,0,1 });
	if (ImGui::Button("Stop", { 40, 20 }))
	{		
		App->scene->inGame = false;
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	size = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	if (lastSize.x != size.x || lastSize.y != size.y)
	{
		lastSize.x = size.x;
		lastSize.y = size.y;

		App->renderer3D->OnResize(size.x, size.y);
	}
	img = (ImTextureID)App->renderer3D->texture;
	ImGui::Image(img, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	App->gui->MouseOnScene(ImGui::IsMouseHoveringWindow());
	ImGui::End();
	
}
