#include "UIPanelOptimization.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleGui.h"
#include "ModuleEditorCamera.h"
#include "ImGui/imgui.h"

#include "mmgr/mmgr.h"

UIPanelOptimization::UIPanelOptimization(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelOptimization::~UIPanelOptimization()
{
}

void UIPanelOptimization::Draw() {

	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	if (App->camera->GetFrustumCulling()) {
		if (App->scene->staticsGObjs != 0) {
			ImGui::Text("Static Game Objects: %d", App->scene->staticsGObjs);
			ImGui::Text("Intersections: %d", App->scene->intersections);
		}
		else {
			ImGui::Text("Set a game object to static to see optimization with quadtree.");
		}
	}
	else
		ImGui::Text("No possible optimization, activate Frustum Culling on Configuration->Camera->Tools.");
	ImGui::End();
}
