#include "UIPanelGame.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGui.h"

UIPanelGame::UIPanelGame(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelGame::~UIPanelGame()
{
}

void UIPanelGame::Draw() {
	uint flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("Game", &active, flags);

	size = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	pos = float2(ImGui::GetCursorPosX() + ImGui::GetWindowPos().x, ImGui::GetCursorPosY() + ImGui::GetWindowPos().y);
	positionX = pos.x;
	positionY = pos.y;

	if (lastSize.x != size.x || lastSize.y != size.y)
	{
		lastSize.x = size.x; lastSize.y = size.y;
		width = size.x; height = size.y;
		App->renderer3D->OnResize(size.x, size.y, false);
	}

	img = (ImTextureID)App->renderer3D->GetGameFBOTexture();

	ImGui::Image(img, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	App->gui->MouseOnGame(ImGui::IsMouseHoveringWindow());

	if (App->scene->gObjSelected) {
		ImGuizmo::SetDrawlist();
		ImGuizmo::OPERATION gizmoOperation = App->scene->guizmoOp;
		App->scene->DrawGuizmo(gizmoOperation);
	}

	ImGui::End();
}

const ImVec2 UIPanelGame::GetMouseRelativeToGame() const
{
	ImVec2 mousePos = ImGui::GetMousePos();

	ImVec2 realMousePos = ImVec2(mousePos.x - pos.x, mousePos.y - pos.y);
	ImVec2 mouseRelativeToGame = ImVec2(realMousePos.x, size.y - realMousePos.y);

	return mouseRelativeToGame;
}
