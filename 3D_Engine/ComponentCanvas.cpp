#include "ComponentCanvas.h"
#include "GameObject.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"

#include "mmgr/mmgr.h"


ComponentCanvas::ComponentCanvas()
{
	type = CANVAS;

	SetResolution(float2(App->gui->panelGame->width, App->gui->panelGame->height));
}


ComponentCanvas::~ComponentCanvas()
{
}

bool ComponentCanvas::Update()
{
	if (!setWidthHeight) {
		myGO->GetComponentRectTransform()->SetWidth(resolution.x, true);
		myGO->GetComponentRectTransform()->SetHeight(resolution.y, true);
		setWidthHeight = true;
	}

	return true;
}

void ComponentCanvas::CleanUp()
{
	myGO = nullptr;
}

void ComponentCanvas::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text("Resolution: %.0f x %.0f", resolution.x, resolution.y);
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
}

void ComponentCanvas::SetResolution(float2 _resolution)
{
	lastResolution = resolution;
	resolution = _resolution;
	if (lastResolution.x == 0.f && lastResolution.y == 0.f)
	{
		lastResolution = resolution;
	}
	setWidthHeight = false;
}
