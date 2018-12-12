#include "ComponentRectTransform.h"
#include "ModuleResources.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")  

ComponentRectTransform::ComponentRectTransform()
{
	type = TRANSFORMRECT;
	rect.width = 1;
	rect.height = 1;

	static const float vtx[] = {
		0,rect.position.x,  rect.position.y ,
		0,rect.position.x ,  rect.position.y + rect.height,
		0,rect.position.x + rect.width,  rect.position.y + rect.height,
		0,rect.position.x + rect.width,  rect.position.y,	
	};

	
	rect.vertex = new float3[4];	
	memcpy(rect.vertex, vtx, sizeof(float3) * 4);
	
	GenBuffer();
	UpdateGlobalMatrix();
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
	RELEASE_ARRAY(rect.vertex);	
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
	ImGui::Spacing();
	ImGui::Checkbox("Draw Canvas", &draw);
}

void ComponentRectTransform::SetPos(float2 pos)
{
	rect.position = pos;
	UpdateGlobalMatrix();
}

void ComponentRectTransform::SetWidth(float w)
{
	rect.width = w;
	UpdateGlobalMatrix();
}

void ComponentRectTransform::SetHeight(float h)
{
	rect.height= h;
	UpdateGlobalMatrix();
}

void ComponentRectTransform::Draw()
{
		glPushMatrix();
		glMultMatrixf(rect.globalMatrix.Transposed().ptr());

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer

		glLineWidth(8.0f);
		glColor3f(0.5f, 0.0f, 0.7f); //pink
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindBuffer(GL_ARRAY_BUFFER, rect.vertexID);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_QUADS, 0, 4);

		glColor3f(1.f, 1.0f, 1.0f);
		glLineWidth(1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	
}

void ComponentRectTransform::GenBuffer()
{
	glGenBuffers(1, (GLuint*) &(rect.vertexID));
	glBindBuffer(GL_ARRAY_BUFFER, rect.vertexID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, &rect.vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ComponentRectTransform::UpdateGlobalMatrix() {
	rect.globalMatrix = float4x4::FromTRS(float3(0, rect.position.y, rect.position.x), Quat(0,0,0,0), float3(0, rect.height,rect.width ));


}