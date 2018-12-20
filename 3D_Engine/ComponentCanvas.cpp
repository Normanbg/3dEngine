#include "ComponentCanvas.h"
#include "GameObject.h"


#include "mmgr/mmgr.h"


ComponentCanvas::ComponentCanvas()
{
	type = CANVAS;

	//resolution = float2(512, 384);
}


ComponentCanvas::~ComponentCanvas()
{
}

bool ComponentCanvas::Update()
{
	if (!setWidthHeight) {
		myGO->GetComponentRectTransform()->SetWidth(resolution.x);
		myGO->GetComponentRectTransform()->SetHeight(resolution.y);
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
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
}

void ComponentCanvas::SetResolution(float2 _resolution)
{
	resolution = _resolution;
	setWidthHeight = false;
}
