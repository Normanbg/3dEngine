#include "UIPanelScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"
#include "ComponentMaterial.h"
#include "ModuleInput.h"
#include "ImGui/imgui.h"

#include "mmgr/mmgr.h"

UIPanelScene::UIPanelScene(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelScene::~UIPanelScene()
{
}

void UIPanelScene::Draw() {
	uint flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("Scene", &active, flags);

	pos = float2(ImGui::GetCursorPosX() + ImGui::GetWindowPos().x, ImGui::GetCursorPosY() + ImGui::GetWindowPos().y);
	positionX = pos.x;
	positionY = pos.y;

	size = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	if (lastSize.x != size.x || lastSize.y != size.y)
	{
		lastSize.x = size.x; lastSize.y = size.y;
		width = size.x; height = size.y;

		App->renderer3D->OnResize(size.x, size.y);
	}

	img = (ImTextureID)App->renderer3D->GetSceneFBOTexture();
	ImGui::Image(img, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	App->gui->MouseOnScene(ImGui::IsMouseHoveringWindow());

	if (App->scene->gObjSelected) {
		ImGuizmo::SetDrawlist();
		ImGuizmo::OPERATION gizmoOperation = App->scene->guizmoOp;
		App->scene->DrawGuizmo(gizmoOperation);
	}

	ImGui::End();

	if (App->gui->clearScene)
		ClearScenePopUp();
}

void UIPanelScene::ClearScenePopUp(){
	ImGui::OpenPopup("Do you want to save before clearing?");

	if (ImGui::BeginPopupModal("Do you want to save before clearing?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			App->scene->SaveScene();
			App->scene->ClearScene();
			App->gui->clearScene = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0)))
		{
			App->scene->ClearScene();
			App->gui->clearScene = false; 
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			App->gui->clearScene = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

}

const ImVec2 UIPanelScene::GetMouse() const
{
	ImVec2 mousePos = ImGui::GetMousePos();

	ImVec2 realMousePos = ImVec2(mousePos.x - pos.x, mousePos.y - pos.y);
	ImVec2 mouseNormalized;

	mouseNormalized.x = realMousePos.x / size.x;
	mouseNormalized.y = realMousePos.y / size.y;

	mouseNormalized.x = (mouseNormalized.x - 0.5) *  2;
	mouseNormalized.y = (mouseNormalized.y - 0.5) * -2;

	return mouseNormalized;
}


