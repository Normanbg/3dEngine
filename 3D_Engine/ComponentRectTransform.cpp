#include "ComponentRectTransform.h"


#include "mmgr/mmgr.h"

ComponentRectTransform::ComponentRectTransform()
{
	type = TRANSFORMRECT;
}


ComponentRectTransform::~ComponentRectTransform()
{
}

bool ComponentRectTransform::Update()
{
	return true;
}

void ComponentRectTransform::CleanUp()
{
}

void ComponentRectTransform::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());


	float2 _pos = GetPos();
	float _w = GetWidth();
	float _h = GetHeight();
		
	if (ImGui::DragFloat2("Position", (float*)&_pos, 0.1f)) { SetPos(_pos); }
	if (ImGui::DragFloat("Width", (float*)&_w, 0.1f)) { SetWidth(_w); }	
	if (ImGui::DragFloat("Height", (float*)&_h, 0.1f)) { SetHeight(_h); }
}


void ComponentRectTransform::SetPos(float2 pos)
{
	rect.position = pos;
}

void ComponentRectTransform::SetWidth(float w)
{
	rect.width = w;
}

void ComponentRectTransform::SetHeight(float h)
{
	rect.height= h;
}
