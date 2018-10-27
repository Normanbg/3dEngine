#include "UIPanelScene.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"

UIPanelScene::UIPanelScene(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelScene::~UIPanelScene()
{
}
//
//void UIPanelScene::Draw() {
//
//	ImGui::Begin("Camera");
//
//	ImVec2 size;
//	size.x = ImGui::GetWindowContentRegionWidth();
//	size.y = ImGui::GetWindowHeight();
//	ImGui::Image(img, size, ImVec2(0, 1), ImVec2(1, 0));
//	ImGui::End();
//
//}
