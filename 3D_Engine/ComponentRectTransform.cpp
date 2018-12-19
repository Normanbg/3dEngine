#include "ComponentRectTransform.h"
#include "ModuleResources.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")  

ComponentRectTransform::ComponentRectTransform()
{
	typeUI = TRANSFORMRECT;
	type = UI;
	rect.width = 1;
	rect.height = 1;
	

	static const float vtx[] = {
		0,rect.globalPosition.x,  rect.globalPosition.y ,
		0,rect.globalPosition.x ,  rect.globalPosition.y + rect.height,
		0,rect.globalPosition.x + rect.width,  rect.globalPosition.y + rect.height,
		0,rect.globalPosition.x + rect.width,  rect.globalPosition.y,	
	};

	
	rect.vertex = new float3[4];	
	memcpy(rect.vertex, vtx, sizeof(float3) * 4);
	
	GenBuffer();
	UpdateLocalPos();
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

void ComponentRectTransform::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	SetGlobalPos(data->GetFloat2("Position", { 0,0 }));
	SetWidth(data->GetFloat("Width", 1));
	SetHeight(data->GetFloat("Height", 1));
}

void ComponentRectTransform::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	data.AddFloat2("Position", GetGlobalPos());
	data.AddFloat("Width", GetWidth());
	data.AddFloat("Height", GetHeight());
}

void ComponentRectTransform::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());

	float2 _pos = GetLocalPos();
	float _w = GetWidth();
	float _h = GetHeight();
	
	if (myGO->GetComponentCanvas()) {
		ImGui::DragFloat2("Position", (float*)&_pos, 0.1f);
		ImGui::DragFloat("Width", (float*)&_w, 0.1f);
		ImGui::DragFloat("Height", (float*)&_h, 0.1f);
	}
	else {
		if (ImGui::DragFloat2("Position", (float*)&_pos, 0.1f)) { SetLocalPos(_pos); }
		if (ImGui::DragFloat("Width", (float*)&_w, 0.1f)) { SetWidth(_w); }
		if (ImGui::DragFloat("Height", (float*)&_h, 0.1f)) { SetHeight(_h); }
	}

	ImGui::Spacing();
	ImGui::Checkbox("Draw Canvas", &draw);
}

void ComponentRectTransform::SetWidth(float w)
{
	rect.width = w;
}

void ComponentRectTransform::SetHeight(float h)
{
	rect.height= h;
}

void ComponentRectTransform::SetGlobalPos(float2 global)
{
	rect.globalPosition = global;
	if (myGO->parent != nullptr && myGO->parent != App->scene->root)
	{
		ComponentRectTransform* parentTrans = myGO->parent->GetComponentRectTransform();
		float2 parentGlobal = parentTrans->GetGlobalPos();
		float2 newLocalPos = rect.globalPosition - parentGlobal;
		SetLocalPos(newLocalPos);
	}
}

void ComponentRectTransform::SetLocalPos(float2 newLocalMat) {
	rect.localPosition = newLocalMat;                                                                                                                                                                                      
}

void ComponentRectTransform::DrawUI()
{
	glPushMatrix();
	float4x4 globalMat;
	SetGlobalMatrixToDraw(globalMat);
	glMultMatrixf(globalMat.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer

	glLineWidth(8.0f);
	glColor3f(0.5f, 0.0f, 0.7f); //pink
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindBuffer(GL_ARRAY_BUFFER, rect.vertexID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_QUADS, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	glLineWidth(3.0f);

	glBegin(GL_LINES);
	glVertex3f(0, rect.anchor.y + 0.1f, rect.anchor.x);
	glVertex3f(0, rect.anchor.y - 0.1f, rect.anchor.x);
	glVertex3f(0, rect.anchor.y, rect.anchor.x + 0.1f);
	glVertex3f(0, rect.anchor.y, rect.anchor.x - 0.1f);
	glEnd();

	glColor3f(1.f, 1.0f, 1.0f);
	glLineWidth(1.0f);
}

void ComponentRectTransform::GenBuffer()
{
	glGenBuffers(1, (GLuint*) &(rect.vertexID));
	glBindBuffer(GL_ARRAY_BUFFER, rect.vertexID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, &rect.vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentRectTransform::UpdateLocalPos() {
	rect.anchor.x = rect.globalPosition.x + rect.width / 2;
	rect.anchor.y = rect.globalPosition.y + rect.height / 2;
	rect.localPosition = float2(rect.globalPosition.x, rect.globalPosition.y);
}

void ComponentRectTransform::SetGlobalMatrixToDraw(float4x4 &globalMatrix) {
	globalMatrix = float4x4::FromTRS(float3(0, rect.globalPosition.y, rect.globalPosition.x), Quat(0, 0, 0, 0), float3(0, rect.height, rect.width));
}