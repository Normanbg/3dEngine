#include "ComponentWindowUI.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"
#include "GameObject.h"
#include "ModuleResources.h"
#include "ComponentImageUI.h"
#include "ComponentButtonUI.h"

ComponentWindowUI::ComponentWindowUI()
{
	typeUI = UI_WINDOW;
	type = UI;
}


ComponentWindowUI::~ComponentWindowUI()
{
}

bool ComponentWindowUI::Start()
{
	rectTransform = myGO->GetComponentRectTransform();
	rectTransform->SetWidth(100, false);
	rectTransform->SetHeight(100, false);
	return true;
}

bool ComponentWindowUI::Update()
{
	if (!hasSetToMid) {
		float2 mid = myGO->parent->GetComponentRectTransform()->GetMid();
		rectTransform->SetLocalPos(mid);
		hasSetToMid = true;
	}

	if (draggable) {
		CheckState();
	}
	if (state = WindowState::DRAGGING) {
		DraggWindow();
	}
	return true;
}

void ComponentWindowUI::CleanUp()
{
}

void ComponentWindowUI::DraggWindow()
{
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (lastPos.x != mousePos.x || mousePos.y != mousePos.y)
	{
		float2 winPos = lastPos - mousePos;
		lastPos = mousePos;
		rectTransform->SetLocalPos(winPos);
	}
}

void ComponentWindowUI::CheckState() {
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	ComponentRectTransform* rectTrans = myGO->GetComponentRectTransform();
	if (rectTrans->GetGlobalPos().x <= mousePos.x && mousePos.x <= rectTrans->GetGlobalPos().x + rectTrans->GetWidth() &&
		rectTrans->GetGlobalPos().y <= mousePos.y && mousePos.y <= rectTrans->GetGlobalPos().y + rectTrans->GetHeight()) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE) {
			state = WindowState::WIN_MOUSEOVER;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && CheckChildsState()) {
			state = WindowState::DRAGGING;
			initMousePos = mousePos;
		}
		/*else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && state == DRAGGING) {

		}*/
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && state == WindowState::DRAGGING) {
			state = WindowState::WIN_MOUSEOVER;
		}
	}
	else if (state == WIN_MOUSEOVER) {
		state = WindowState::WINDOW_IDLE;
	}
}

bool ComponentWindowUI::CheckChildsState()
{
	std::vector <GameObject*>	childrens = myGO->childrens;
	for (auto it : childrens) {
		if (it->GetComponentButtonUI() != nullptr && it->GetComponentButtonUI()->state != ButtonState::IDLE) {
			return false;
		}
		//....missing to check checkboxes and input
	}
	return true;
}

void ComponentWindowUI::SetResource(uuid resource)
{
	windImage = (ResourceTexture*)App->resources->Get(resource);
	windImage->LoadInMemory();
	ComponentImageUI* image = myGO->GetComponentImageUI();
	if (image != nullptr) {
		image->SetResource(App->resources->FindByName(windImage->GetName(), Resource::ResType::Texture));
	}
}

void ComponentWindowUI::DrawInspector()
{
	float windowSize = ImGui::GetWindowContentRegionWidth();
	ImGui::Checkbox("Draggable", &draggable);
	if (ImGui::CollapsingHeader("Background Image")) {
		const char* windMaterial = NULL;
		if (windImage != nullptr) {
			windMaterial = windImage->GetName();
		}
		if (ImGui::BeginCombo("   ", windMaterial))
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (windMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(windMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					windMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture));

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}
			}
			ImGui::EndCombo();
		}
		if (windImage != nullptr) {
			ImGui::Image((void*)(windImage->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", windImage->width, windImage->height);
			windMaterial = nullptr;
		}
	}
}

void ComponentWindowUI::DrawUI()
{
}

void ComponentWindowUI::Load(Config * data)
{
}

void ComponentWindowUI::Save(Config & data) const
{
}