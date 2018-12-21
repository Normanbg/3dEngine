#include "ComponentButtonUI.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"
#include "GameObject.h"
#include "ModuleResources.h"

ComponentButtonUI::ComponentButtonUI()
{
	typeUI = UI_BUTTON;
	type = UI;
}


ComponentButtonUI::~ComponentButtonUI()
{
}

void ComponentButtonUI::UpdateRectTransform()
{
}

void ComponentButtonUI::DrawInspector()
{
	const char* idleMaterial = NULL;
	if (idleImg != nullptr) {
		idleMaterial = idleImg->GetName();
	}
	if (ImGui::BeginCombo("Idle Image", idleMaterial))
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);

		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (idleMaterial != nullptr) {
				const char* n = mat[i]->GetName();
				bool is_selected = (strcmp(idleMaterial, n) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				idleMaterial = mat[i]->GetName();
				SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture), 0);

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}
	if (idleImg != nullptr) {
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(idleImg->gpuID), ImVec2(windowSize / 2, windowSize/2), ImVec2(0, 1), ImVec2(1, 0));
		idleMaterial = nullptr;
	}

	if (ImGui::BeginCombo("Hover Image", idleMaterial))
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);

		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (idleMaterial != nullptr) {
				const char* n = mat[i]->GetName();
				bool is_selected = (strcmp(idleMaterial, n) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				idleMaterial = mat[i]->GetName();
				SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture), 0);

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}
	if (idleImg != nullptr) {
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(idleImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
		idleMaterial = nullptr;
	}

	if (ImGui::BeginCombo("Pressed Image", idleMaterial))
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);

		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (idleMaterial != nullptr) {
				const char* n = mat[i]->GetName();
				bool is_selected = (strcmp(idleMaterial, n) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				idleMaterial = mat[i]->GetName();
				SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture), 0);

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}

		}
		ImGui::EndCombo();
	}
	if (idleImg != nullptr) {
		float windowSize = ImGui::GetWindowContentRegionWidth();
		ImGui::Image((void*)(idleImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
		idleMaterial = nullptr;
	}
}

void ComponentButtonUI::DrawUI()
{
}

bool ComponentButtonUI::Start()
{
	return true;
}

bool ComponentButtonUI::Update()
{
	GameObject* canvasGO = App->scene->GetFirstGameObjectCanvas();
	if (App->gui->isMouseOnGame() && myGO != canvasGO)
		CheckState();

	return true;
}

void ComponentButtonUI::CleanUp()
{
}

void ComponentButtonUI::CheckState() {
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	ComponentRectTransform* rectTrans = myGO->GetComponentRectTransform();
	if (rectTrans->GetGlobalPos().x <= mousePos.x && mousePos.x <= rectTrans->GetGlobalPos().x + rectTrans->GetWidth() &&
		rectTrans->GetGlobalPos().y <= mousePos.y && mousePos.y <= rectTrans->GetGlobalPos().y + rectTrans->GetHeight()) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state == CLICKED) {
			state = MOUSEOVER;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state != CLICKED) {
			state = CLICKED;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE && state != CLICKED) {
			state = MOUSEOVER;
		}
	}
}

void ComponentButtonUI::SetResource(uuid resource, int numRes)
{
	switch (numRes)
	{
	case 0:
		idleImg = (ResourceTexture*)App->resources->Get(resource);
		idleImg->LoadInMemory();
		break;
	case 1:
		hoverImg = (ResourceTexture*)App->resources->Get(resource);
		hoverImg->LoadInMemory();
		break;
	case 2:
		pressedImg = (ResourceTexture*)App->resources->Get(resource);
		pressedImg->LoadInMemory();
		break;
	}
}

void ComponentButtonUI::Load(Config * data)
{
}

void ComponentButtonUI::Save(Config & data) const
{
}

