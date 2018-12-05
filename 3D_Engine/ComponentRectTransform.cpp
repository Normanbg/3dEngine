#include "ComponentRectTransform.h"


#include "mmgr/mmgr.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 

ComponentRectTransform::ComponentRectTransform()
{
	type = TRANSFORMRECT;
	rect.width = 100;
	rect.height = 100;
	rect.vertex[0] = &float3( rect.position.x, rect.position.y , 0);
	rect.vertex[1] = &float3(rect.position.x+ rect.width, rect.position.y, 0);
	rect.vertex[2] = &float3(rect.position.x , rect.position.y+ rect.height, 0);
	rect.vertex[3] = &float3(rect.position.x, rect.position.y + rect.height, 0);
	rect.vertex[4] = &float3(rect.position.x + rect.width, rect.position.y, 0);
	rect.vertex[5] = &float3(rect.position.x + rect.width, rect.position.y + rect.height, 0);
	GenBuffer();
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

void ComponentRectTransform::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, rect.bufferID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentRectTransform::GenBuffer()
{
	glGenBuffers(1, (GLuint*) &(rect.bufferID));
	glBindBuffer(GL_ARRAY_BUFFER, rect.bufferID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*6, rect.vertex[0], GL_STATIC_DRAW);

}
