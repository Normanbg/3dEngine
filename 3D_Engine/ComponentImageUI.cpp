#include "ComponentImageUI.h"
#include "GameObject.h"
#include "ModuleResources.h"
#include "ComponentRectTransform.h"

#include "mmgr/mmgr.h"



ComponentImageUI::ComponentImageUI()
{
	typeUI = UI_IMAGE;
	type = UI;
	
}


ComponentImageUI::~ComponentImageUI()
{
}

bool ComponentImageUI::Start()
{
	static const float uvs[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	texCoords = new float2[4];
	memcpy(texCoords, uvs, sizeof(float2) * 4);

	rectTransform = myGO->GetComponentRectTransform();
	
	return true;
}

bool ComponentImageUI::Update()
{
	if (fadingOut) {
		FadeOut();
	}
	if (fadingIn) {
		FadeIn();
	}
	return true;
}

void ComponentImageUI::CleanUp()
{
	
	if (HasTexture()) {
		resourceTexture->CleanUp();
	}
	rectTransform = nullptr;
	resourceTexture = nullptr;
	myGO = nullptr;
	RELEASE_ARRAY(texCoords);
}

void ComponentImageUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	alpha = data->GetFloat("Alpha", 1.0f);
	std::string matName = data->GetString("UITexName", "NoName");

	if (matName != "NoName") {
		resourceTexture = (ResourceUI*)App->resources->Get(App->resources->FindByName(matName.c_str(), Resource::ResType::UI));
		resourceTexture->LoadInMemory();
	}
}

void ComponentImageUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	data.AddFloat("Alpha", alpha);
	if (resourceTexture) {
		data.AddString("UITexName", resourceTexture->GetName());
	}
}

void ComponentImageUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());

	if (myGO->GetComponentButtonUI() != nullptr || myGO->GetComponentWindowUI() != nullptr) {
		return;
	}
	const char* currentMaterial = NULL;
	if (resourceTexture != nullptr) {
		currentMaterial = resourceTexture->GetName();
	}
	ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);
	if (ImGui::BeginCombo("Material", currentMaterial))
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (currentMaterial != nullptr) {
				const char* n = mat[i]->GetName();
				bool is_selected = (strcmp(currentMaterial, n) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				currentMaterial = mat[i]->GetName();
				SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI));

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	currentMaterial = nullptr;
	ImGui::Separator();
	if (resourceTexture != nullptr) {
		ImGui::Text("Resource UUID: %i", resourceTexture->GetUUID());
		ImGui::Text(resourceTexture->GetName());

		ImGui::Text("Texture size:\nWidth: %dpx \nHeight: %dpx ", resourceTexture->width, resourceTexture->height);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(resourceTexture->gpuID), ImVec2(windowSize, windowSize), ImVec2(0, 1), ImVec2(1, 0));		
	}
}

void ComponentImageUI::DrawUI()
{
	glPushMatrix();
	float4x4 globalMat;
	rectTransform->SetGlobalMatrixToDraw(globalMat);
	glMultMatrixf(globalMat.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, alpha);
	glLineWidth(4.0f);

	if (HasTexture()) {
		
		glBindTexture(GL_TEXTURE_2D, resourceTexture->gpuID);
		glTexCoordPointer(2, GL_FLOAT, 0, &(texCoords[0]));
	}
	glBindBuffer(GL_ARRAY_BUFFER, rectTransform->GetVertexID());
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_QUADS, 0,4);

	glLineWidth(1.0f);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glDisable(GL_BLEND);
	glPopMatrix();

}

const bool ComponentImageUI::HasTexture() const
{
	bool ret;
	resourceTexture ? ret = true : ret = false;
	return ret;
}

void ComponentImageUI::SetResource(uuid resource)
{
	resourceTexture = (ResourceUI*)App->resources->Get(resource);
	resourceTexture->LoadInMemory();
	resourceUUID = resource;
}

const uint ComponentImageUI::GetTexID() const
{
	if (HasTexture()) {
		return resourceTexture->gpuID;
	}
	return -1;
}


void ComponentImageUI::FadeIn()
{
	alpha += DELTA_ALPHA;
	if (alpha >= 1.0f) {
		fadingIn = false;
		alpha = 1.0f;
	}
}

void ComponentImageUI::FadeOut()
{
	alpha -= DELTA_ALPHA;
	if (alpha <= 0.0f) {
		fadingOut = false;
		alpha = 0.0f;
	}
}