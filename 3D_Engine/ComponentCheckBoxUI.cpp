#include "ComponentCheckBoxUI.h"
#include "ComponentImageUI.h"
#include "GameObject.h"
#include "ModuleResources.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"
#include "ModuleInput.h"

#include "mmgr/mmgr.h"

ComponentCheckBoxUI::ComponentCheckBoxUI()
{
	typeUI = UI_CHECKBOX;
	type = UI;
}


ComponentCheckBoxUI::~ComponentCheckBoxUI()
{
}

bool ComponentCheckBoxUI::Start()
{
	image = myGO->GetComponentImageUI();
	rectTransform = myGO->GetComponentRectTransform();
	rectTransform->SetWidth(20);
	rectTransform->SetHeight(20);
	SetResource(App->resources->FindByName(DEFAULT_IDLE_CH, Resource::ResType::UI), 1);
	SetResource(App->resources->FindByName(DEFAULT_PRESSED_CH, Resource::ResType::UI),0);
	return true;
}

bool ComponentCheckBoxUI::Update()
{
	if (!hasSetToMid) {
		float2 mid = myGO->parent->GetComponentRectTransform()->GetMid();
		rectTransform->SetLocalPos(mid);
		hasSetToMid = true;
	}

	GameObject* canvasGO = App->scene->GetFirstGameObjectCanvas();
	if (App->gui->isMouseOnGame() && myGO != canvasGO)
		CheckState();
	return true;
}

void ComponentCheckBoxUI::CleanUp()
{
	if (HasTexture(unpressedImg)) {
		unpressedImg->CleanUp();
	}
	if (HasTexture(pressedImg)) {
		pressedImg->CleanUp();
	}

	rectTransform = nullptr;
	unpressedImg = nullptr;
	pressedImg = nullptr;
	myGO = nullptr;
	image = nullptr;
}

void ComponentCheckBoxUI::CheckState() {
	if (IsMouseOver()) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state == PRESSED) {
			state = IDLE;
			if (unpressedImg != nullptr)
				ChangeGOImage();
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state != PRESSED) {
			state = PRESSED;
			if (pressedImg != nullptr)
				ChangeGOImage();
		}
	}
}


bool ComponentCheckBoxUI::IsMouseOver()
{
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (rectTransform->GetGlobalPos().x <= mousePos.x && mousePos.x <= rectTransform->GetGlobalPos().x + rectTransform->GetWidth() &&
		rectTransform->GetGlobalPos().y <= mousePos.y && mousePos.y <= rectTransform->GetGlobalPos().y + rectTransform->GetHeight()) {
		return true;
	}
	else
		return false;
}

void ComponentCheckBoxUI::ChangeGOImage()
{
	switch (state)
	{
	case ButtonState::IDLE:
		image->SetResource(App->resources->FindByName(unpressedImg->GetName(), Resource::ResType::UI));
		break;
	case ButtonState::PRESSED:
		image->SetResource(App->resources->FindByName(pressedImg->GetName(), Resource::ResType::UI));
		break;
	default:
		break;
	}
}

const bool ComponentCheckBoxUI::HasTexture(ResourceTexture * res) const
{
	bool ret;
	res ? ret = true : ret = false;
	return ret;
}

void ComponentCheckBoxUI::SetResource(uuid resource, int numRes)
{
	if (resource != 0) {
		switch (numRes)
		{
		case 0:
			unpressedImg = (ResourceTexture*)App->resources->Get(resource);
			unpressedImg->LoadInMemory();
			ChangeGOImage();
			break;
		case 1:
			pressedImg = (ResourceTexture*)App->resources->Get(resource);
			pressedImg->LoadInMemory();
			break;
		}
	}
}

void ComponentCheckBoxUI::Load(Config * data)
{
	hasSetToMid = true;
	SetResource(App->resources->FindByName(data->GetString("Idle", ""), Resource::ResType::UI), 0);
	SetResource(App->resources->FindByName(data->GetString("Press", ""), Resource::ResType::UI), 1);
}

void ComponentCheckBoxUI::Save(Config & data) const
{
	if (unpressedImg)
		data.AddString("Idle", unpressedImg->GetName());
	if (pressedImg)
		data.AddString("Press", pressedImg->GetName());
}

void ComponentCheckBoxUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());

	float windowSize = ImGui::GetWindowContentRegionWidth();

	if (ImGui::CollapsingHeader("Unpressed Image")) {
		const char* idleMaterial = NULL;
		if (unpressedImg != nullptr) {
			idleMaterial = unpressedImg->GetName();
		}
		if (ImGui::BeginCombo("   ", idleMaterial))
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (idleMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(idleMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					idleMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI), 0);

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}

			}
			ImGui::EndCombo();
		}
		if (unpressedImg != nullptr) {
			ImGui::Image((void*)(unpressedImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", unpressedImg->width, unpressedImg->height);
			idleMaterial = nullptr;
		}
	}

	if (ImGui::CollapsingHeader("Pressed Image")) {
		const char* pressedMaterial = NULL;
		if (pressedImg != nullptr) {
			pressedMaterial = pressedImg->GetName();
		}
		if (ImGui::BeginCombo("  ", pressedMaterial))//TO AVOID 
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (pressedMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(pressedMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					pressedMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI), 1);

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}
			}
			ImGui::EndCombo();
		}
		if (pressedImg != nullptr) {
			float windowSize = ImGui::GetWindowContentRegionWidth();
			ImGui::Image((void*)(pressedImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", pressedImg->width, pressedImg->height);
			pressedMaterial = nullptr;
		}
	}
}