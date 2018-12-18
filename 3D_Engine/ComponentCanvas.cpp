#include "ComponentCanvas.h"
#include "GameObject.h"
#include "mmgr/mmgr.h"


ComponentCanvas::ComponentCanvas()
{
	type = CANVAS;
	standaloneResolution = float2(1064, 728);
	
}


ComponentCanvas::~ComponentCanvas()
{
}

bool ComponentCanvas::Update()
{
	if (!setWidthHeight) {
		myGO->GetComponentRectTransform()->SetWidth(standaloneResolution.x);
		myGO->GetComponentRectTransform()->SetHeight(standaloneResolution.y);
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
