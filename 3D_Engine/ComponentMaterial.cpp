#include "ComponentMaterial.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

ComponentMaterial::ComponentMaterial()
{
	type = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::Update()
{
	return true;
}

void ComponentMaterial::CleanUp()
{
	if (HasTexture()) {
		resourceTexture->CleanUp();
	}
	resourceTexture = nullptr;
	myGO = nullptr;
}

void ComponentMaterial::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	
	const char* currentMaterial = NULL;
	if (resourceTexture != nullptr) {
		currentMaterial = resourceTexture->GetName();
	}
	if (ImGui::BeginCombo("Material", currentMaterial))
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);

		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (currentMaterial != nullptr) {
				bool is_selected = (strcmp(currentMaterial, mat[i]->GetName()) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				currentMaterial = mat[i]->GetName();
				SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture));
								
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}
	currentMaterial = nullptr;
	ImGui::Separator();
	if (resourceTexture!= nullptr) {
		ImGui::Text("Resource UUID: %i", resourceTexture->GetUUID());
		ImGui::Text(resourceTexture->GetName());

		ImGui::Text("Texture size:\nWidth: %dpx \nHeight: %dpx ", resourceTexture->width, resourceTexture->height);
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(resourceTexture->gpuID), ImVec2(windowSize, windowSize),ImVec2(0,1), ImVec2(1,0));
		ImGui::SliderFloat("Alpha", &alphaTest, 0, 1.0f);
		ImGui::Checkbox("Blend Color", &doBlendTest);
	}
	ImGui::Spacing();
	ImGui::Text("Color:"); ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())	{//ImguiTip
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Color is only shown if no texture is drawn");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::ColorPicker3("Color##2f", (float*)&colors);
	ImGui::Separator();
}

void ComponentMaterial::SetResource(uuid resource)
{
	resourceTexture = (ResourceTexture*)App->resources->Get(resource);
	resourceTexture->LoadInMemory();
	resourceUUID = resource;
}

const uint ComponentMaterial::GetTexID() const
{
	if (HasTexture()) {
		return resourceTexture->gpuID;
	}
	return -1;
}

const char * ComponentMaterial::GetTextureName() const
{
	if (HasTexture()) {
		return resourceTexture->GetName();
	}
	return nullptr;
}

const bool ComponentMaterial::HasTexture() const
{
	bool ret;
	resourceTexture ? ret = true : ret = false;
	return ret;
}

void ComponentMaterial::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	if (resourceTexture) {
		data.AddString("TexName", resourceTexture->GetName());
		data.AddFloat("Alpha", alphaTest);
		data.AddBool("BlendColor", doBlendTest);
	}
	if (!colors.IsZero()) {
		data.AddFloat3("Colors", colors);
	}
}

void ComponentMaterial::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	colors = data->GetFloat3("Colors", { 0,0,0 });
	std::string matName = data->GetString("TexName", "NoName");
	
	if (matName != "NoName") {
		alphaTest = data->GetFloat("Alpha", 1.0f);
		doBlendTest = data->GetBool("BlendColor", false);
		resourceTexture =(ResourceTexture*) App->resources->Get(App->resources->FindByName(matName.c_str(), Resource::ResType::Texture));
		resourceTexture->LoadInMemory();
	}
	ComponentMesh* linkMesh = nullptr;
	if (linkMesh = myGO->GetComponentMesh()) {
		if (linkMesh->HasMaterial() == false) {
			linkMesh->SetMaterial(this);
		}
	}
}
