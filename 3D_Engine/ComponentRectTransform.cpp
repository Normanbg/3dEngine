#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "UIPanelGame.h"
#include "ModuleInput.h"
#include "ModuleGui.h"

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
		rect.globalPosition.x,  rect.globalPosition.y , 0,
		rect.globalPosition.x + rect.width,  rect.globalPosition.y,	0,
		rect.globalPosition.x + rect.width,  rect.globalPosition.y + rect.height, 0,
		rect.globalPosition.x ,  rect.globalPosition.y + rect.height, 0, 		
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
	GameObject* canvasGO = App->scene->GetFirstGameObjectCanvas();
	if (App->gui->isMouseOnGame() && myGO != canvasGO)
		CheckMousePos();
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
	SetWidth(data->GetFloat("Width", 1),false);

	SetHeight(data->GetFloat("Height", 1), false);
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
		if (ImGui::DragFloat("Width", (float*)&_w, 0.1f)) { SetWidth(_w, false); }
		if (ImGui::DragFloat("Height", (float*)&_h, 0.1f)) { SetHeight(_h, false); }
		if (ImGui::CollapsingHeader("Basic Positions")) {
			if (ImGui::Button("Top Left")) {
				float height = myGO->parent->GetComponentRectTransform()->GetHeight();
				SetLocalPos(float2(0, height));
			}
			ImGui::SameLine();
			if (ImGui::Button("Top center")) {
				float height = myGO->parent->GetComponentRectTransform()->GetHeight();
				float width = myGO->parent->GetComponentRectTransform()->GetWidth() / 2;
				SetLocalPos(float2(width, height));
			}
			ImGui::SameLine();
			if (ImGui::Button("Top Right")) {
				float height = myGO->parent->GetComponentRectTransform()->GetHeight();
				float width = myGO->parent->GetComponentRectTransform()->GetWidth();
				SetLocalPos(float2(width, height));
			}
			if (ImGui::Button("Center Left")) {
				float height = myGO->parent->GetComponentRectTransform()->GetHeight() / 2;
				SetLocalPos(float2(0, height));
			}
			ImGui::SameLine();
			if (ImGui::Button("Center")) {
				float2 mid = myGO->parent->GetComponentRectTransform()->GetMid();
				SetLocalPos(mid);
			}
			ImGui::SameLine();
			if (ImGui::Button("Center Right")) {
				float height = myGO->parent->GetComponentRectTransform()->GetHeight() / 2;
				float width = myGO->parent->GetComponentRectTransform()->GetWidth();
				SetLocalPos(float2(width, height));
			}
			if (ImGui::Button("Down Left")) {
				SetLocalPos(float2(0, 0));
			}
			ImGui::SameLine();
			if (ImGui::Button("Down center")) {
				float width = myGO->parent->GetComponentRectTransform()->GetWidth() / 2;
				SetLocalPos(float2(width, 0));
			}
		
			ImGui::SameLine();
			if (ImGui::Button("Down Right")) {
				float width = myGO->parent->GetComponentRectTransform()->GetWidth();
				SetLocalPos(float2(width, 0));
			}
			ImGui::SameLine();
			
		}
	}

	ImGui::Spacing();
	ImGui::Checkbox("Draw Canvas", &draw);
}

void ComponentRectTransform::SetWidth(float w, bool canvas)
{
	rect.width = w;
	UpdateUIComponents();
}

void ComponentRectTransform::SetHeight(float h, bool canvas)
{
	rect.height= h;
	UpdateUIComponents();
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

	//----Anchor Point
	glBegin(GL_LINES);
	glVertex3f(rect.anchor.x, rect.anchor.y + 0.1f, 0);
	glVertex3f(rect.anchor.x, rect.anchor.y - 0.1f, 0);
	glVertex3f(rect.anchor.x + 0.1f, rect.anchor.y, 0);
	glVertex3f(rect.anchor.x - 0.1f, rect.anchor.y, 0);
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

void ComponentRectTransform::CheckMousePos() {
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (rect.globalPosition.x <= mousePos.x && mousePos.x <= rect.globalPosition.x + rect.width &&
		rect.globalPosition.y <= mousePos.y && mousePos.y <= rect.globalPosition.y + rect.height) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			state = CLICKED;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE && state != CLICKED) {
			state = MOUSEOVER;
		}
	}
}

void ComponentRectTransform::UpdateLocalPos() {
	rect.anchor.x = rect.globalPosition.x + rect.width / 2;
	rect.anchor.y = rect.globalPosition.y + rect.height / 2;
	rect.localPosition = float2(rect.globalPosition.x, rect.globalPosition.y);
}

void ComponentRectTransform::UpdatePercentatge() {
	ComponentRectTransform* parentRect = myGO->parent->GetComponentRectTransform();
	if (parentRect) {
		rect.percentatgeHeight = rect.height / parentRect->GetHeight();
		rect.percentatgeWidth = rect.width / parentRect->GetWidth();
	}
}

void ComponentRectTransform::UpdateSizeWithPercentatge(float lastParentWidth, float lastParentHeight) {
	ComponentRectTransform* parentRect = myGO->parent->GetComponentRectTransform();
	if (parentRect) {
		float lastWidth, lastHeight;
		lastWidth = rect.width;
		lastHeight = rect.height;
		rect.height *= (parentRect->GetHeight() / lastParentHeight);
		rect.width *= (parentRect->GetWidth() / lastParentWidth);

		for (auto it : myGO->childrens) {
			UpdateSizeWithPercentatge(lastWidth, lastHeight);
		}
	}
}

void ComponentRectTransform::UpdateUIComponents()
{
	for (int i = 0; i < myGO->componentsUI.size(); ++i) {
		if (myGO->componentsUI[i] != this) {
			myGO->componentsUI[i]->UpdateRectTransform();
		}
	}
}

void ComponentRectTransform::SetGlobalMatrixToDraw(float4x4 &globalMatrix) {
	ComponentCanvas* canvas = App->scene->GetFirstGameObjectCanvas()->GetComponentCanvas();
	if (canvas->editor) {
		float2 dividedPos = rect.globalPosition / 5;
		float dividedHeight = rect.height / 5;
		float dividedWidth = rect.width / 5;
		globalMatrix = float4x4::FromTRS(float3(dividedPos.x, dividedPos.y, 0), Quat(0, 0, 0, 0), float3(dividedWidth, dividedHeight, 0));
	}
	else
		globalMatrix = float4x4::FromTRS(float3(rect.globalPosition.x, rect.globalPosition.y, 0), Quat(0, 0, 0, 0), float3(rect.width, rect.height, 0));
}
