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
	ImGui::Checkbox("Draw CrossHair", &cross);
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

void ComponentCanvas::DrawCrossHair()
{
	ComponentRectTransform* t = myGO->GetComponentRectTransform();
	if (t && cross) {
		
		float2 midPoint = float2(t->GetWidth() / 2, t->GetHeight() / 2);
		glLineWidth(5.0f);
		glBegin(GL_LINES);
		glVertex3f(midPoint.x, midPoint.y + 10.f, 0);
		glVertex3f(midPoint.x, midPoint.y - 10.f, 0);
		glVertex3f(midPoint.x + 10.f, midPoint.y, 0);
		glVertex3f(midPoint.x - 10.f, midPoint.y, 0);
		glEnd();
		glLineWidth(1.0f);
	}
}
